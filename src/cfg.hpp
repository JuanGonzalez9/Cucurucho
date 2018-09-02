#ifndef CFG_HPP
#define CFG_HPP

#include <stdexcept>

extern "C"
{
	#include <libxml/tree.h>
	#include <libxml/xpath.h>
}

class configure
{
public:
	configure ();
	~configure ();
	std::string gets (const char *path);
	int geti (const char *path);
	long getl (const char *path);
	long long getll (const char *path);
	unsigned long getul (const char *path);
	unsigned long long getull (const char *path);
	float getf (const char *path);
	double getd (const char *path);
	long double getld (const char *path);
private:
	xmlDoc *doc;
	xmlXPathContextPtr context;
	std::string gets_from_xml (const char *path);
	std::string gets_from_map (const char *path);
	template <class t> t get_i_number (const char *path, const char *type_name, t (*i_function) (const std::string&, size_t*, int));
	template <typename t> t get_fp_number (const char *path, const char *type_name, t (*fp_function) (const std::string&, size_t*));
	void dump_default ();
};

class configure_exception: public std::invalid_argument
{
public:
	explicit configure_exception (const std::string &what_arg);
};

extern configure &cfg;

static class configure_init
{
public:
	configure_init ();
	~configure_init ();
} cfg_init;

#endif
