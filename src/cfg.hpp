#ifndef CFG_HPP
#define CFG_HPP

#include <stdexcept>
#include <functional>

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
	std::string gets (const char *path, std::function<bool(std::string & string, bool trusted)> valid);
	int geti (const char *path, std::function<bool(int n, bool trusted)> valid);
	long getl (const char *path, std::function<bool(long n, bool trusted)> valid);
	long long getll (const char *path, std::function<bool(long long n, bool trusted)> valid);
	unsigned long getul (const char *path, std::function<bool(unsigned long n, bool trusted)> valid);
	unsigned long long getull (const char *path, std::function<bool(unsigned long long n, bool trusted)> valid);
	float getf (const char *path, std::function<bool(float n, bool trusted)> valid);
	double getd (const char *path, std::function<bool(double n, bool trusted)> valid);
	long double getld (const char *path, std::function<bool(long double n, bool trusted)> valid);
private:
	template<typename t> class types
	{
	public:
		typedef t (*i_function) (const std::string&, size_t*, int);
		typedef t (*fp_function) (const std::string&, size_t*);
	};
	xmlDoc *doc;
	xmlXPathContextPtr context;
	std::string gets_from_xml (const char *path);
	std::string gets_from_map (const char *path);
	template <class t> t get_i_number (const char *path, typename types<t>::i_function function, std::function<bool(int i, bool trusted)> valid);
	template <typename t> t get_fp_number (const char *path, typename types<t>::fp_function function, std::function<bool(int i, bool trusted)> valid);
	void dump_defaults ();
	xmlNode * add_root_node (xmlDoc *doc, std::string & name);
	xmlNode * add_child_node (xmlDoc *doc, xmlNode *parent_node, std::string & name);
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
