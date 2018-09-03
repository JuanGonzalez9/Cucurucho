#include <new>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <map>
#include "cfg_defaults.cpp"
#include "cfg.hpp"

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
	#include <string.h>
	#include <errno.h>
}

// Uso el idioma Nifty Counter para asegurarme que el objeto se inicialice
// antes de su primer uso y se destruya luego del último uso.
static int nifty_counter;
static typename std::aligned_storage<sizeof (configure), alignof (configure)>::type cfg_buffer;
configure &cfg = reinterpret_cast<configure&> (cfg_buffer);

void xml_error_handler (void * context, const char * format, ...)
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

configure_exception::configure_exception (const std::string &what_arg):
	std::invalid_argument (what_arg)
{
}

configure::configure ()
{
	LIBXML_TEST_VERSION
	xmlSetGenericErrorFunc (nullptr, xml_error_handler);
	struct stat sb;
	if (stat ("cfg.xml", &sb) == -1) {
		// TODO log
		dump_defaults ();
		return;
	}
	doc = xmlReadFile ("cfg.xml", NULL, 0);
	if (doc == nullptr) {
		// TODO log
		return;
	}
	context = xmlXPathNewContext (doc);
	if (context == nullptr) {
		// TODO log
		xmlFreeDoc (doc);
		doc = nullptr;
		return;
	}
}

configure::~configure ()
{
	xmlXPathFreeContext (context);
	xmlFreeDoc (doc);
	xmlCleanupParser ();
}

xmlNode * configure::add_root_node (xmlDoc *doc, std::string & name)
{
	xmlNode *node = xmlNewNode (nullptr, (const xmlChar*)name.c_str ());
	if (node == nullptr) {
		// TODO log
		return nullptr;
	}
	xmlDocSetRootElement (doc, node);
	return node;
}

xmlNode * configure::add_child_node (xmlDoc *doc, xmlNode *parent_node, std::string & name)
{
	if (parent_node == nullptr ) {
		if ((parent_node = xmlDocGetRootElement (doc)) == nullptr) {
			return add_root_node (doc, name);
		} else if (strcmp ((const char*)parent_node->name, name.c_str ()) == 0) {
			return parent_node;
		} else {
			// TODO log
			return nullptr;
		}
	} else {
		for (xmlNode * i = xmlFirstElementChild (parent_node); i != nullptr; i = xmlNextElementSibling (parent_node)) {
			if (strcmp ((const char*)i->name, name.c_str ()) == 0) {
				return i;
			}
		}
		xmlNode *node = xmlNewNode (nullptr, (const xmlChar*)name.c_str ());
		if (node == nullptr) {
			// TODO log
			return nullptr;
		}
		xmlAddChild (parent_node, node);
		return node;
	}
}

void configure::dump_defaults ()
{
	doc = xmlNewDoc ((const xmlChar*)"1.0");
	if (doc == nullptr) {
		// TODO log
		return;
	}
	xmlNode *node;
	std::string s, subs;
	std::string::size_type f, l;
	for (std::map<std::string, std::string>::iterator i = defaults.begin(); i != defaults.end(); ++i) {
		s = i->first;
		f = s.find ("//", 0);
		node = nullptr;
		while (f != std::string::npos && f+2 < s.length()) {
			f += 2;
			l = s.find ("//", f);
			subs = s.substr (f, l-f);
			if ((node = add_child_node (doc, node, subs)) == nullptr) {
				break;
			}
			f = l;
		}
		if (node) {
			xmlNodeAddContent (node, (const xmlChar*)i->second.c_str());
		}
	}
	xmlKeepBlanksDefault (0);
	if (xmlSaveFormatFile ("cfg.xml", doc, 1) == -1)
	{
		// TODO log
	}
	xmlFreeDoc (doc);
	doc = nullptr;
}

std::string configure::gets_from_xml (const char *path)
{
	if (path == nullptr ){
  		// TODO log
		throw configure_exception ("configure::get* espera un camino");
	}
	std::string s;
	if (context != nullptr) {
		xmlXPathObjectPtr result = xmlXPathEvalExpression ((const xmlChar*)path, context);
		if (result && !xmlXPathNodeSetIsEmpty (result->nodesetval)) {
			xmlNode *node = result->nodesetval->nodeTab[0];
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

std::string configure::gets_from_map (const char *path)
{
	if (path == nullptr){
  		// TODO log
		throw configure_exception ("configure::get* espera un camino");
	}
	std::map<std::string, std::string>::iterator i;
	i = defaults.find(path);
	if (i == defaults.end()) {
  		// TODO log
  		std::stringstream ss ;
  		ss << "No agregamos una opcion por defecto para " << path;
		throw configure_exception (ss.str());
	}
	return i->second;
}

std::string configure::gets (const char *path)
{
	std::string s = gets_from_xml (path);
	if (s.empty()) {
		s = gets_from_map (path);
	}
	return s;
}

template <typename t> t configure::get_i_number (const char *path, const char *type_name, t (*f) (const std::string&, size_t*, int))
{
	t n;
	std::string::size_type size;
	// Si no está la opción en el archivo xml o no es un número, evito que se lance un excepción;
	// ya que todavía puedo usar la opción por defecto.
	try{
		std::string s = gets_from_xml (path);
		n = f (s, &size, 10);
		if (size != s.length()) throw 1;
		return n;
	} catch (...){}
	// La opción por defecto debe estar sí o sí en cfg_defaults.cpp
	std::string s = gets_from_map (path);
	n = f (s, &size, 10);
	if (size != s.length())
	{
  		// TODO log
  		std::stringstream ss ;
  		ss << "Opcion por defecto para " << path << " invalida, se esperaba un " << type_name;
		throw configure_exception (ss.str());
	}
	return n;
}

template <typename t> t configure::get_fp_number (const char *path, const char *type_name, t (*f) (const std::string&, size_t*))
{
	t n;
	std::string::size_type size;
	// Si no está la opción en el archivo xml o no es un número, evito que se lance un excepción;
	// ya que todavía puedo usar la opción por defecto.
	try{
		std::string s = gets_from_xml (path);
		n = f (s, &size);
		if (size != s.length()) throw 1;
		return n;
	} catch (...){}
	// La opción por defecto debe estar sí o sí en cfg_defaults.cpp
	std::string s = gets_from_map (path);
	n = f (s, &size);
	if (size != s.length())
	{
  		// TODO log
  		std::stringstream ss ;
  		ss << "Opcion por defecto para " << path << " invalida, se esperaba un " << type_name;
		throw configure_exception (ss.str());
	}
	return n;
}

int configure::geti (const char *path)
{
	return get_i_number<int> (path, "int", std::stoi);
}

long configure::getl (const char *path)
{
	return get_i_number<long> (path, "long", std::stol);
}

long long configure::getll (const char *path)
{
	return get_i_number<long long> (path, "long long", std::stoll);
}

unsigned long configure::getul (const char *path)
{
	return get_i_number<unsigned long> (path, "unsigned long", std::stoul);
}

unsigned long long configure::getull (const char *path)
{
	return get_i_number<unsigned long long> (path, "unsigned long long", std::stoull);
}

float configure::getf (const char *path)
{
	return get_fp_number<float> (path, "float", std::stof);
}

double configure::getd (const char *path)
{
	return get_fp_number<double> (path, "double", std::stod);
}

long double configure::getld (const char *path)
{
	return get_fp_number<long double> (path, "long double", std::stold);
}

configure_init::configure_init ()
{
	if (nifty_counter++ == 0) new (&cfg) configure ();
}

configure_init::~configure_init ()
{
	if (--nifty_counter == 0) (&cfg)->~configure ();
}


