#include <new>
#include <type_traits>
#include <iostream>
#include <map>
#include <algorithm>
#include "cfg.hpp"

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <string.h>
	#include <errno.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <stdlib.h>
}

void configuracion::esperar_vblank ()
{
	if( drm_fd == -1 ) {
		usleep (9000);
	} else {
		vblank.request.type = DRM_VBLANK_RELATIVE ;
		vblank.request.sequence = 1 ;
		if (drmWaitVBlank (drm_fd, &vblank) == -1) {
			close (drm_fd);
			drm_fd = -1;
			std::cout << "No se pudo sincronizarcon vblank\n";
		}
	}
}

#define como_cadena (x) #x
static const char * cfg_omision_xml =
#include "cfg_omision.xml"
;

#define cfgerror(nodo, msg) \
	do { \
		std::stringstream ss; \
		ss << "cfg.xml:" << xmlGetLineNo (nodo) << ": " << msg; \
		registro.registrar (LogEventos::error, ss.str().c_str()); \
	} while (false)
	
#define cfg_carga_error() \
	do { \
		xmlErrorPtr	error = xmlGetLastError (); \
		if (error) { \
			std::stringstream ss; \
			ss << "cfg.xml:" << error->line << ": " << error->message; \
			std::string s = ss.str(); \
			std::replace (s.begin(), s.end(), '\n', '\0'); \
			registro.registrar (LogEventos::error, s.c_str()); \
		} \
	} while (false)

#define lanzar(msg) \
	do {\
		std::stringstream ss; \
		ss << __FILENAME__ << ":" << __LINE__ << ": " << msg; \
		throw excepcion_configuracion (ss.str()); \
	} while (false)

const char * plataforma::caminos[] = {
	"//configuracion//plataformas//piedra//sprite",
	"//configuracion//plataformas//puente//sprite",
	"//configuracion//plataformas//puentesoga//sprite",
	"//configuracion//plataformas//puente2//sprite",
	"//configuracion//plataformas//cascroca1//sprite",
	"//configuracion//plataformas//cascroca2//sprite",
	"//configuracion//plataformas//cascroca3//sprite",
	"//configuracion//plataformas//cascroca4//sprite",
	"//configuracion//plataformas//piedras//sprite",
	"//configuracion//plataformas//piedram//sprite",
	"//configuracion//plataformas//piedral//sprite",
	"//configuracion//plataformas//piedraxl//sprite",
	"//configuracion//plataformas//hielo//sprite",
	"//configuracion//plataformas//hielo1//sprite",
	"//configuracion//plataformas//nieve//sprite",
	"//configuracion//plataformas//nieve_arboles//sprite",
	"//configuracion//plataformas//metal//sprite"
};

excepcion_configuracion::excepcion_configuracion (const std::string &que_paso):
	std::invalid_argument (que_paso)
{
}

int configuracion::random (int desde, int hasta)
{
	if (desde > hasta) {
		int aux = desde;
		desde = hasta;
		hasta = aux;
	}
	long int cantidad = hasta - desde + 1;
	if (cantidad > INT_MAX) {
		cantidad = INT_MAX;
		logerror ("se pidio un numero aleatorio fuera de rango.");
	}
	long int r = ::random () % cantidad;
	return (int) (r + desde);
}

configuracion::configuracion ():
	doc (nullptr),
	contexto (nullptr)
{
	registro.definirTipoLog (LogEventos::error);

	struct timespec ts;
	if (timespec_get (&ts, TIME_UTC)) {
		srandom (ts.tv_nsec ^ ts.tv_sec);
	} else {
		srandom (696969);
	}

	dlopen_handle = dlopen (nullptr, RTLD_LOCAL | RTLD_LAZY);
	if (dlopen_handle == nullptr) {
		lanzar ("fallo dlopen: " << dlerror() << '.');
	}

	LIBXML_TEST_VERSION

	doc_omision = xmlReadMemory (cfg_omision_xml, strlen (cfg_omision_xml), "sin_nombre", nullptr, 0);
	if (doc_omision == nullptr) {
		lanzar ("no pudo crearse el xmlDoc con los valores por omision.");
	}
	contexto_omision = xmlXPathNewContext (doc_omision);
	if (contexto_omision == nullptr) {
		xmlFreeDoc (doc_omision);
		lanzar ("no pudo crearse el xmlXPathContextPtr del documento con los valores por omision.");
	}
	struct stat sb;
	if (stat ("cfg.xml", &sb) == -1) {
		recrear_archivo_xml ();
		logerror("el archivo de configuracion no existia, fue recreado.");
	} else {
		doc = xmlReadFile ("cfg.xml", NULL, 0);
		if (doc == nullptr) {
			cfg_carga_error ();
			logerror("el archivo de configuracion no pudo cargarse, se usan las opciones por omision.");
		} else {
			contexto = xmlXPathNewContext (doc);
			if (contexto == nullptr) {
				cfg_carga_error ();
				logerror("el archivo de configuracion no pudo cargarse, se usan las opciones por omision.");
				xmlFreeDoc (doc);
				doc = nullptr;
			}
		}
	}

	// Obtengo el nivel de depurado almacenado en la clave "//configuracion//debug//level"
	// Primero se valida el dato obtenido en el archivo xml y de fallar se valida la opción por
	// defecto, en este último caso no debería fallar, por lo que si lo hace se produce una excepcion.
	std::string s = cfg.obtener_s ("//configuracion//debug//level", [](std::string s, bool trusted) {
		return s == "ERROR" || s == "INFO" || s == "DEBUG"; // Se valida el dato.
	});
	LogEventos::TipoLog tipo = s == "ERROR" ? LogEventos::error : (s == "INFO" ? LogEventos::info : LogEventos::debug);
	registro.definirTipoLog (tipo);

	drm_fd = open( "/dev/dri/card0", O_RDWR | O_CLOEXEC);
	if (drm_fd == -1) {
		std::cout << "Fallo la apertura de card0\n";
	}
	// Obtengo el numero actual de secuencia. type debe ser DRM_VBLANK_RELATIVE y sequence 0.
	memset (&vblank, 0, sizeof(vblank));
	//vblank.request.sequence = 0 ;

	// Establezo a type para que espere el numero de secuencia calculado en XShmBackend::Wait_for_vblank.
	vblank.request.type = (drmVBlankSeqType)( DRM_VBLANK_ABSOLUTE | DRM_VBLANK_NEXTONMISS ) ;

	atexit (SDL_Quit);
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		logerror("No pudo inicializarse SDL");
		std::cerr << "No pudo inicializarse SDL: " << SDL_GetError () << '\n';
	}
	cursor_texto = SDL_CreateSystemCursor (SDL_SYSTEM_CURSOR_IBEAM);
	cursor_activo = cursor_flecha = SDL_CreateSystemCursor (SDL_SYSTEM_CURSOR_ARROW);
}

configuracion::~configuracion ()
{
	if (cursor_flecha) {
		SDL_FreeCursor (cursor_flecha);
	}
	if (cursor_texto) {
		SDL_FreeCursor (cursor_texto);
	}
	close (drm_fd);
	xmlXPathFreeContext (contexto);
	xmlFreeDoc (doc);
	xmlXPathFreeContext (contexto_omision);
	xmlFreeDoc (doc_omision);
	xmlCleanupParser ();
	if (dlopen_handle != nullptr) {
		 dlclose (dlopen_handle);
	}
}

void configuracion::establecer_cursor_texto ()
{
	if (cursor_texto && cursor_texto != cursor_activo) {
		SDL_SetCursor (cursor_texto);
		cursor_activo = cursor_texto;
	}
}

void configuracion::establecer_cursor_flecha ()
{
	if (cursor_flecha && cursor_flecha != cursor_activo) {
		SDL_SetCursor (cursor_flecha);
		cursor_activo = cursor_flecha;
	}
}

xmlNode * configuracion::agregar_nodo_raiz (xmlDoc *doc, std::string & nombre)
{
	xmlNode *nodo = xmlNewNode (nullptr, (const xmlChar*)nombre.c_str ());
	if (nodo != nullptr) {
		logerror ("no pudo crearse un nuevo nodo, fallo xmlNewNode.");
		return nullptr;
	}
	xmlDocSetRootElement (doc, nodo);
	return nodo;
}

xmlNode * configuracion::agregar_nodo_hijo (xmlDoc *doc, xmlNode *nodo_padre, std::string & nombre)
{
	if (nodo_padre == nullptr ) {
		if ((nodo_padre = xmlDocGetRootElement (doc)) == nullptr) {
			return agregar_nodo_raiz (doc, nombre);
		} else if (strcmp ((const char*)nodo_padre->name, nombre.c_str ()) == 0) {
			return nodo_padre;
		} else {
			logerror ("se intento agregar otro nodo raiz.");
			return nullptr;
		}
	} else {
		for (xmlNode * i = xmlFirstElementChild (nodo_padre); i != nullptr; i = xmlNextElementSibling (nodo_padre)) {
			if (strcmp ((const char*)i->name, nombre.c_str ()) == 0) {
				return i;
			}
		}
		xmlNode *nodo = xmlNewNode (nullptr, (const xmlChar*)nombre.c_str ());
		if (nodo == nullptr) {
			logerror ("no pudo crearse un nuevo nodo, fallo xmlNewNode.");
			return nullptr;
		}
		xmlAddChild (nodo_padre, nodo);
		return nodo;
	}
}

void configuracion::recrear_archivo_xml ()
{
	xmlKeepBlanksDefault (0);
	if (xmlSaveFormatFile ("cfg.xml", doc_omision, 1) == -1)
	{
		logerror ("no se puede guardarse el archivo de configuracion.");
	}
}

bool configuracion::obtener_s_del_xml (const char *camino, xmlXPathContextPtr contexto, std::string &s)
{
	s.clear ();
	if (contexto != nullptr) {
		xmlXPathObjectPtr resultado = xmlXPathEvalExpression ((const xmlChar*)camino, contexto);
		if (resultado && !xmlXPathNodeSetIsEmpty (resultado->nodesetval)) {
			unodo = resultado->nodesetval->nodeTab[0];
			xmlNode *child_node = unodo->children;
			if (child_node && child_node->type == XML_TEXT_NODE) {
				s = (const char*)xmlNodeGetContent(child_node);
				const char *ws = " \t\n\r\v\f";
				s.erase(s.find_last_not_of(ws) + 1);
				s.erase(0, s.find_first_not_of(ws));
				return true;
			}
		}
	}
	return false;
}

bool configuracion::obtener_padre (std::string &camino)
{
	std::size_t pos = camino.rfind("//");
	if (pos == std::string::npos) {
		lanzar ("camino: '" << camino << "' invalido.");
	}
	if (camino.length () == 2) {
		return false;
	} else if (pos > 0) {
		camino.resize (pos);
	} else {
		camino.resize (2);
	}
	return true;
}

xmlNode *configuracion::obtener_nodo_mas_profundo (const char *camino, xmlXPathContextPtr contexto)
{
	if (contexto != nullptr) {
		std::string s = camino;
		do {
			xmlXPathObjectPtr resultado = xmlXPathEvalExpression ((const xmlChar*)s.c_str(), contexto);
			if (resultado && !xmlXPathNodeSetIsEmpty (resultado->nodesetval)) {
				return resultado->nodesetval->nodeTab[0];
			}
		} while (obtener_padre (s));
	}
	return nullptr;
}

std::string configuracion::obtener_s (const char *camino, std::function<bool(std::string & s, bool omision)> validar)
{
	std::string s;
	bool r = obtener_s_del_xml (camino, contexto, s);
	if (!r || !validar (s, false)) {
		if (!r) {
			if ((unodo = obtener_nodo_mas_profundo (camino, contexto))) {
				cfgerror (unodo, "falta el valor de '" << camino << "'.");
			} else {
				logerror ("falta el valor de '" << camino << "'.");
			}
		} else {
			cfgerror (unodo, "el valor '" << s << "' es invalido para <" << unodo->name << ">.");
		}
		r = obtener_s_del_xml (camino, contexto_omision, s);
		if (!r || !validar (s, true)) {
			lanzar ("opcion por defecto '" << s << "' para " << camino << " invalida.");
		} else {
			logerror ("se usa el valor por omision '" << s << "' para '" << camino << "'.");
		}
	}
	return s;
}

uint8_t *configuracion::obtener_direccion_de_simbolo (std::string &s)
{
	uint8_t *direccion = (uint8_t*)dlsym (dlopen_handle, s.c_str ());
	if (direccion == nullptr) {
		lanzar ("fallo dlsym: " << dlerror() << '.');
	}
	return direccion;
}

bool plataforma::comparar_plataforma (const plataforma& p1, const plataforma& p2)
{
	return p1.y < p2.y;
}

void configuracion::obtener_plataformas (const char *camino, std::function<void(xmlNode *node)> funcion)
{
	xmlXPathContextPtr ctx = contexto;
	if (ctx == nullptr) {
		ctx = contexto_omision;
	}
	xmlXPathObjectPtr resultado = xmlXPathEvalExpression ((const xmlChar*)camino, ctx);
	if (resultado && !xmlXPathNodeSetIsEmpty (resultado->nodesetval)) {
		for (xmlNode *node = resultado->nodesetval->nodeTab[0]->children; node; node = node->next) {
			if (node->type == XML_ELEMENT_NODE && strcmp ((const char*)node->name, "plataforma") == 0) {
				funcion (node);
			}
		}
	}
}

void configuracion::obtener_plataformas (const char *camino, std::list<plataforma> &l, const plataforma & plataforma_omision)
{
	obtener_plataformas (camino, [&l](xmlNode *nodo){
		plataforma p;
		const int t = 0x1;
		const int xi = 0x2;
		const int xf = 0x4;
		const int y = 0x8;
		int leidos = 0;
		for (xmlNode *nodo_hijo = nodo->children; nodo_hijo; nodo_hijo = nodo_hijo->next) {
			if (nodo_hijo->type == XML_ELEMENT_NODE) {
				std::string s = (const char*)xmlNodeGetContent(nodo_hijo);
				if (strcmp ((const char*)nodo_hijo->name, "tipo") == 0) {
					leidos |= t;
					if (s == "metal") {
						p.t = plataforma::metal;
					} else if (s == "piedra") {
						p.t = plataforma::piedra;
					} else if (s == "hielo") {
						p.t = plataforma::hielo;
					} else if (s == "hielo1") {
						p.t = plataforma::hielo1;
					} else if (s == "nieve") {
						p.t = plataforma::nieve;
					} else if (s == "puente2") {
						p.t = plataforma::puente2;
					} else if (s == "puentesoga") {
						p.t = plataforma::puentesoga;
					} else if (s == "piedras") {
						p.t = plataforma::piedras;
					} else if (s == "piedram") {
						p.t = plataforma::piedram;
					} else if (s == "piedral") {
						p.t = plataforma::piedral;
					} else if (s == "piedraxl") {
						p.t = plataforma::piedraxl;
					} else if (s == "cascroca1") {
						p.t = plataforma::cascroca1;
					} else if (s == "cascroca2") {
						p.t = plataforma::cascroca2;
					} else if (s == "cascroca3") {
						p.t = plataforma::cascroca3;
					} else if (s == "cascroca4") {
						p.t = plataforma::cascroca4;
					} else if (s == "puente") {
						p.t = plataforma::puente;
					} else {
						cfgerror (nodo_hijo, "se esperaba metal, piedra, hielo, puente o puente2 en el nodo <" << nodo_hijo->name << ">.");
					}
				} else {
					try{
						if (strcmp ((const char*)nodo_hijo->name, "xi") == 0) {
							leidos |= xi;
						} else if (strcmp ((const char*)nodo_hijo->name, "xf") == 0) {
							leidos |= xf;
						} else if (strcmp ((const char*)nodo_hijo->name, "y") == 0) {
							leidos |= y;
						}

						std::string::size_type largo;
						int n = std::stoi (s, &largo, 10);
						if (largo != s.length()) {
							throw (1);
						}

						if (strcmp ((const char*)nodo_hijo->name, "xi") == 0) {
							p.xi = n;
						} else if (strcmp ((const char*)nodo_hijo->name, "xf") == 0) {
							p.xf = n;
						} else if (strcmp ((const char*)nodo_hijo->name, "y") == 0) {
							p.y = n;
						}
					} catch (...) {
						cfgerror (nodo_hijo, "se esperaba un entero en el nodo <" << nodo_hijo->name << ">.");
					}
				}
			}
		}
		if (leidos == 0xf) {
			if (p.xi > p.xf) {
				int aux = p.xi;
				p.xi = p.xf;
				p.xf = aux;
			}
			if (p.xi >= 0 && p.xf >= 0 && p.xf < 10000) {
				l.push_back (p);
			} else {
				cfgerror (nodo, "se ignora plataforma fuera del area permitida (0,0,9999,9999).");
			}
		} else {
			if ((leidos & t) == 0) {
				cfgerror (nodo, "a la plataforma dada le falta el nodo <tipo>.");
			}
			if ((leidos & xi) == 0) {
				cfgerror (nodo, "a la plataforma dada le falta el nodo <xi>.");
			}
			if ((leidos & xf) == 0) {
				cfgerror (nodo, "a la plataforma dada le falta el nodo <xf>.");
			}
			if ((leidos & y) == 0) {
				cfgerror (nodo, "a la plataforma dada le falta el nodo <y>.");
			}
		}
	});
	l.push_back (plataforma_omision);
	l.sort (plataforma::comparar_plataforma);
}

SDL_Texture *configuracion::obtener_textura (const char *camino, SDL_Renderer *renderer)
{
	SDL_Texture *textura = nullptr;
	std::string s = obtener_s (camino, [renderer, &textura] (std::string & s, bool omision) {
		SDL_Surface *superficie = IMG_Load (s.c_str ());
		if (superficie == nullptr) {
			return omision? true : false;
		}
		textura = SDL_CreateTextureFromSurface (renderer, superficie);
		if (textura == nullptr) {
			SDL_FreeSurface (superficie);
			return omision? true : false;
		}
		SDL_FreeSurface (superficie);
		return true;
	});
	return nullptr != textura ? textura : configuracion::obtener_textura_por_omision (s, renderer);
}

SDL_Texture *configuracion::obtener_textura_grisada (const char *camino, SDL_Renderer *renderer)
{
	SDL_Texture *textura = nullptr;
	std::string s = obtener_s (camino, [this, renderer, &textura] (std::string & s, bool omision) {
		try {
			std::string sc = s;
			textura = configuracion::obtener_textura_por_omision (sc, renderer);
			return true;
		} catch (...) {
			if (omision) {
				throw;
			} else {
				return false;
			}
		}
	});
	return textura;
}

SDL_Texture *configuracion::obtener_textura_por_omision (std::string & s, SDL_Renderer *renderer)
{
	logerror ("la imagen '" << s << "' no pudo cargarse, se usa la imagen por omision.");

	/* imagenes/foo.png  ->  _binary_ ______ imagenes_foo_png _start
	 *                                ../../ imagenes/foo.png _start
	 */
	std::replace (s.begin(), s.end(), '/', '_');
	std::replace (s.begin(), s.end(), '.', '_');
	std::replace (s.begin(), s.end(), ' ', '_');
	s.insert (0, "_binary_imagen_gris_");
	std::string se = s;
	s.append ("_start");
	se.append ("_end");
	uint8_t *comienzo = obtener_direccion_de_simbolo (s);
	uint8_t *fin = obtener_direccion_de_simbolo (se);
	if (fin == nullptr) {
		lanzar ("fallo dlsym: " << dlerror());
	}

	SDL_RWops *rw = SDL_RWFromMem ( comienzo, (size_t)(fin-comienzo) );
	if (rw == nullptr) {
		lanzar ("fallo SDL_RWFromMem: " << SDL_GetError());
	}
	SDL_Surface *superficie = IMG_Load_RW (rw, 1);
	if (superficie == nullptr) {
		lanzar ("fallo IMG_Load_RW: " << SDL_GetError());
	}
	SDL_Texture *textura = SDL_CreateTextureFromSurface ( renderer, superficie);
	if (textura == nullptr) {
		lanzar ("fallo SDL_CreateTextureFromSurface: " << SDL_GetError());
	}
	return textura;
}

template <typename t> t configuracion::obtener_i_number (const char *camino, typename tipos<t>::funcion_i f, std::function<bool(int i, bool omision)> validar)
{
	t n;
	std::string s = obtener_s (camino, [&n, f, validar] (std::string & s, bool omision) {
		std::string::size_type largo;
		try{
			n = f (s, &largo, 10);
			return largo == s.length() && validar (n, false);
		} catch (...){
			return false;
		}
	});
	return n;
}

template <typename t> t configuracion::obtener_fp_number (const char *camino, typename tipos<t>::funcion_pf f, std::function<bool(int i, bool omision)> validar)
{
	t n;
	std::string s = obtener_s (camino, [&n, f, validar] (std::string & s, bool omision) {
		std::string::size_type largo;
		try{
			n = f (s, &largo);
			return largo == s.length() && validar (n, false);
		} catch (...){
			return false;
		}
	});
	return n;
}

int configuracion::obtener_i (const char *camino, std::function<bool(int, bool)> validar)
{
	return obtener_i_number<int> (camino, std::stoi, validar);
}

long configuracion::obtener_l (const char *camino, std::function<bool(long, bool)> validar)
{
	return obtener_i_number<long> (camino, std::stol, validar);
}

long long configuracion::obtener_ll (const char *camino, std::function<bool(long long, bool)> validar)
{
	return obtener_i_number<long long> (camino, std::stoll, validar);
}

unsigned long configuracion::obtener_ul (const char *camino, std::function<bool(unsigned long, bool)> validar)
{
	return obtener_i_number<unsigned long> (camino, std::stoul, validar);
}

unsigned long long configuracion::obtener_ull (const char *camino, std::function<bool(unsigned long long, bool)> validar)
{
	return obtener_i_number<unsigned long long> (camino, std::stoull, validar);
}

float configuracion::obtener_f (const char *camino, std::function<bool(float, bool)> validar)
{
	return obtener_fp_number<float> (camino, std::stof, validar);
}

double configuracion::obtener_d (const char *camino, std::function<bool(double, bool)> validar)
{
	return obtener_fp_number<double> (camino, std::stod, validar);
}

long double configuracion::obtener_ld (const char *camino, std::function<bool(long double, bool)> validar)
{
	return obtener_fp_number<long double> (camino, std::stold, validar);
}
