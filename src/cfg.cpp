#include <new>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <map>
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
// Uso el idioma Nifty Counter para asegurarme que el objeto se inicialice
// antes de su primer uso y se destruya luego del último uso.
static int nifty_counter;
static typename std::aligned_storage<sizeof (configuracion), alignof (configuracion)>::type cfg_buffer;
configuracion &cfg = reinterpret_cast<configuracion&> (cfg_buffer);

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
	LIBXML_TEST_VERSION
	xmlSetGenericErrorFunc (nullptr, xml_error_handler);
					 
	doc_omision = xmlReadMemory (cfg_omision_xml, strlen (cfg_omision_xml), "sin_nombre", nullptr, 0);
	if (doc_omision == nullptr) {
		// TODO log
		throw excepcion_configuracion ("no pudo crearse el xmlDoc con los valores por omision");
	}
	contexto_omision = xmlXPathNewContext (doc_omision);
	if (contexto_omision == nullptr) {
		// TODO log
		xmlFreeDoc (doc_omision);
		throw excepcion_configuracion ("no pudo crearse el xmlXPathContextPtr del documento con los valores por omision");
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
}

configuracion::~configuracion ()
{
	xmlXPathFreeContext (contexto);
	xmlFreeDoc (doc);
	xmlXPathFreeContext (contexto_omision);
	xmlFreeDoc (doc_omision);
	xmlCleanupParser ();
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
			std::stringstream ss ;
			ss << "Opcion por defecto para " << camino << " invalida";
			s.clear();
			throw excepcion_configuracion (ss.str());
		}
	}
	return s;
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
  		std::stringstream ss ;
  		ss << "Opcion por defecto para " << camino << " invalida";
		throw excepcion_configuracion (ss.str());
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
  		std::stringstream ss ;
  		ss << "Opcion por defecto para " << camino << " invalida";
		throw excepcion_configuracion (ss.str());
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

inicializador_configuracion::inicializador_configuracion ()
{
	if (nifty_counter++ == 0) new (&cfg) configuracion ();
}

inicializador_configuracion::~inicializador_configuracion ()
{
	if (--nifty_counter == 0) (&cfg)->~configuracion ();
}


