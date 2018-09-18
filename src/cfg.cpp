#include <new>
#include <type_traits>
#include <iostream>
#include <sstream>
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
}

#define como_cadena (x) #x
static const char * cfg_omision_xml =
#include "cfg_omision.xml"
;

#define lanzar(e, msg) \
	do {\
		std::stringstream ss; \
		ss << msg; \
		throw e (ss.str()); \
	} while (false)

void xml_error_handler (void * contexto, const char * format, ...)
{
	/* TODO decidir si implementar bien y hacer un log
	va_list ap;
	va_start (ap, format);
	int size = vsnprintf (nullptr, 0, format, ap);
	va_end (ap);
	va_start (ap, format);
	char * msg = new char[size+1];
	vsnprintf (msg, size+1, format, ap);
	va_end (ap);
	std::cout << msg;
	*/
}

excepcion_configuracion::excepcion_configuracion (const std::string &que_paso):
	std::invalid_argument (que_paso)
{
}

configuracion::configuracion ()
{
	registro.definirTipoLog (LogEventos::error);

	dlopen_handle = dlopen (nullptr, RTLD_LOCAL | RTLD_LAZY);
	if (dlopen_handle == nullptr) {
		lanzar (excepcion_configuracion, "Fallo dlopen: " << dlerror());
	}

	LIBXML_TEST_VERSION
	xmlSetGenericErrorFunc (nullptr, xml_error_handler);

	doc_omision = xmlReadMemory (cfg_omision_xml, strlen (cfg_omision_xml), "sin_nombre", nullptr, 0);
	if (doc_omision == nullptr) {
		// TODO log
		lanzar (excepcion_configuracion, "no pudo crearse el xmlDoc con los valores por omision");
	}
	contexto_omision = xmlXPathNewContext (doc_omision);
	if (contexto_omision == nullptr) {
		// TODO log
		xmlFreeDoc (doc_omision);
		lanzar (excepcion_configuracion, "no pudo crearse el xmlXPathContextPtr del documento con los valores por omision");
	}
	struct stat sb;
	if (stat ("cfg.xml", &sb) == -1) {
		// TODO log
		recrear_archivo_xml ();
		return;
	}

	doc = xmlReadFile ("cfg.xml", NULL, 0);
	if (doc == nullptr) {
		// TODO log
		return;
	}
	contexto = xmlXPathNewContext (doc);
	if (contexto == nullptr) {
		// TODO log
		xmlFreeDoc (doc);
		doc = nullptr;
		return;
	}

	// Obtengo el nivel de depurado almacenado en la clave "//configuracion//debug//level"
	// Primero se valida el dato obtenido en el archivo xml y de fallar se valida la opción por
	// defecto, en este último caso no debería fallar, por lo que si lo hace se produce una excepcion.
	std::string s = cfg.obtener_s ("//configuracion//debug//level", [](std::string s, bool trusted) {
		return s == "ERROR" || s == "INFO" || s == "DEBUG"; // Se valida el dato.
	});
	LogEventos::TipoLog tipo = s == "ERROR" ? LogEventos::error : (s == "INFO" ? LogEventos::info : LogEventos::debug);
	registro.definirTipoLog (tipo);
}

configuracion::~configuracion ()
{
	xmlXPathFreeContext (contexto);
	xmlFreeDoc (doc);
	xmlXPathFreeContext (contexto_omision);
	xmlFreeDoc (doc_omision);
	xmlCleanupParser ();
	if (dlopen_handle != nullptr) {
		 dlclose (dlopen_handle);
	}
}

xmlNode * configuracion::agregar_nodo_raiz (xmlDoc *doc, std::string & nombre)
{
	xmlNode *nodo = xmlNewNode (nullptr, (const xmlChar*)nombre.c_str ());
	if (nodo == nullptr) {
		// TODO log
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
			// TODO log
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
			// TODO log
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
		// TODO log
	}
}

std::string configuracion::obtener_s_del_xml (const char *camino, xmlXPathContextPtr *contexto)
{
	std::string s;
	if (*contexto != nullptr) {
		xmlXPathObjectPtr resultado = xmlXPathEvalExpression ((const xmlChar*)camino, *contexto);
		if (resultado && !xmlXPathNodeSetIsEmpty (resultado->nodesetval)) {
			xmlNode *node = resultado->nodesetval->nodeTab[0];
			xmlNode *child_node = node->children;
			if (child_node && child_node->type == XML_TEXT_NODE) {
				s = (const char*)xmlNodeGetContent(child_node);
				const char *ws = " \t\n\r\v\f";
				s.erase(s.find_last_not_of(ws) + 1);
				s.erase(0, s.find_first_not_of(ws));
			}
		}
	}
	return s;
}

std::string configuracion::obtener_s (const char *camino, std::function<bool(std::string & s, bool omision)> validar)
{
	std::string s = obtener_s_del_xml (camino, &contexto);
	if (s.empty() || !validar (s, false)) {
		s = obtener_s_del_xml (camino, &contexto_omision);
		if (s.empty() || !validar (s, true)) {
			// TODO log
			s.clear();
			lanzar (excepcion_configuracion, "Opcion por defecto para " << camino << " invalida");
		}
	}
	return s;
}

uint8_t *configuracion::obtener_direccion_de_simbolo (std::string &s)
{
	uint8_t *direccion = (uint8_t*)dlsym (dlopen_handle, s.c_str ());
	if (direccion == nullptr) {
		lanzar (excepcion_configuracion, "Fallo dlsym: " << dlerror());
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

void configuracion::obtener_plataformas (const char *camino, std::list<plataforma> &l)
{
	obtener_plataformas (camino, [&l](xmlNode *node){
		plataforma p;
		const int t = 0x1;
		const int xi = 0x2;
		const int xf = 0x4;
		const int y = 0x8;
		int leidos = 0;
		for (xmlNode *nodo_hijo = node->children; nodo_hijo; nodo_hijo = nodo_hijo->next) {
			if (nodo_hijo->type == XML_ELEMENT_NODE) {
				std::string s = (const char*)xmlNodeGetContent(nodo_hijo);
				if (strcmp ((const char*)nodo_hijo->name, "tipo") == 0) {
					if (s == "pasto") {
						leidos |= t;
						p.t = plataforma::pasto;
					} else if (s == "piedra") {
						leidos |= t;
						p.t = plataforma::piedra;
					} else if (s == "hielo") {
						leidos |= t;
						p.t = plataforma::hielo;
					} else {
						// TODO log
					}
				} else {
					try{
						std::string::size_type largo;
						int n = std::stoi (s, &largo, 10);
						if (largo != s.length()) {
							throw (1);
						}
						if (strcmp ((const char*)nodo_hijo->name, "xi") == 0) {
							leidos |= xi;
							p.xi = n;
						} else if (strcmp ((const char*)nodo_hijo->name, "xf") == 0) {
							leidos |= xf;
							p.xf = n;
						} else if (strcmp ((const char*)nodo_hijo->name, "y") == 0) {
							leidos |= y;
							p.y = n;
						}
						if (leidos == 0xf) {
							l.push_back (p);
						} else {
							if ((leidos & t) == 0) {
								// TODO log
							}
							if ((leidos & xi) == 0) {
								// TODO log
							}
							if ((leidos & xf) == 0) {
								// TODO log
							}
							if ((leidos & y) == 0) {
								// TODO log
							}
						}
					} catch (...){}
				}
			}
		}
	});
	l.sort (plataforma::comparar_plataforma);
}

SDL_Texture *configuracion::obtener_textura (const char *camino, SDL_Renderer *renderer, std::function<bool(SDL_Texture *textura, bool omision)> validar)
{
	SDL_Texture *textura = nullptr;
	std::string s = obtener_s (camino, [renderer, &textura] (std::string & s, bool omision) {
		SDL_Surface *superficie = IMG_Load (s.c_str ());
		if (superficie == nullptr) {
			// TODO log
			return omision? true : false;
		}
		textura = SDL_CreateTextureFromSurface (renderer, superficie);
		if (textura == nullptr) {
			// TODO log
			SDL_FreeSurface (superficie);
			return omision? true : false;
		}
		SDL_FreeSurface (superficie);
		return true;
	});
	if (textura == nullptr) {
		/* imagenes/foo.png  ->  _binary_ ______ imagenes_foo_png _start
		 *                                ../../ imagenes/foo.png _start
		 */
		std::replace (s.begin(), s.end(), '/', '_');
		std::replace (s.begin(), s.end(), '.', '_');
		std::replace (s.begin(), s.end(), ' ', '_');
		s.insert (0, "_binary____");
		std::string se = s;
		s.append ("_start");
		se.append ("_end");
		uint8_t *comienzo = obtener_direccion_de_simbolo (s);
		uint8_t *fin = obtener_direccion_de_simbolo (se);
		if (fin == nullptr) {
			lanzar (excepcion_configuracion, "Fallo dlsym: " << dlerror());
		}

		SDL_RWops *rw = SDL_RWFromMem ( comienzo, (size_t)(fin-comienzo) );
		if (rw == nullptr) {
			// TODO log
			lanzar (excepcion_configuracion, "Fallo SDL_RWFromMem: " << SDL_GetError());
		}
		SDL_Surface *superficie = IMG_Load_RW (rw, 1);
		if (superficie == nullptr) {
			// TODO log
			lanzar (excepcion_configuracion, "Fallo IMG_Load_RW: " << SDL_GetError());
		}
		textura = SDL_CreateTextureFromSurface ( renderer, superficie);
		if (textura == nullptr) {
			// TODO log
			lanzar (excepcion_configuracion, "Fallo SDL_CreateTextureFromSurface: " << SDL_GetError());
		}
	}
	return textura;
}

template <typename t> t configuracion::obtener_i_number (const char *camino, typename tipos<t>::funcion_i f, std::function<bool(int i, bool omision)> validar)
{
	t n;
	std::string::size_type largo;
	// Si no está la opción en el archivo xml o no es un número, evito que se lance un excepción;
	// ya que todavía puedo usar la opción por defecto.
	try{
		std::string s = obtener_s_del_xml (camino, &contexto);
		n = f (s, &largo, 10);
		if (largo == s.length() && validar (n, false)) {
			return n;
		}
	} catch (...){}
	// La opción por defecto debe estar sí o sí en cfg_omision.xml
	std::string s = obtener_s_del_xml (camino, &contexto_omision);
	n = f (s, &largo, 10);
	if (largo != s.length() || !validar (n, true))
	{
  		// TODO log
		lanzar (excepcion_configuracion, "Opcion por defecto para " << camino << " invalida");
	}
	return n;
}

template <typename t> t configuracion::obtener_fp_number (const char *camino, typename tipos<t>::funcion_pf f, std::function<bool(int i, bool omision)> validar)
{
	t n;
	std::string::size_type largo;
	// Si no está la opción en el archivo xml o no es un número, evito que se lance un excepción;
	// ya que todavía puedo usar la opción por defecto.
	try{
		std::string s = obtener_s_del_xml (camino, &contexto);
		n = f (s, &largo);
		if (largo == s.length() && validar (n, false)) {
			return n;
		}
	} catch (...){}
	// La opción por defecto debe estar sí o sí en cfg_defaults.cpp
	std::string s = obtener_s_del_xml (camino, &contexto_omision);
	n = f (s, &largo);
	if (largo != s.length() || !validar (n, true))
	{
  		// TODO log
		lanzar (excepcion_configuracion, "Opcion por defecto para " << camino << " invalida");
	}
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
