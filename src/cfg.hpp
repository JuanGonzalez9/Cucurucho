#ifndef CFG_HPP
#define CFG_HPP

#include <stdexcept>
#include <functional>
#include <list>

extern "C"
{
	#include <libxml/tree.h>
	#include <libxml/xpath.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <dlfcn.h>
}

class plataforma
{
public:
	typedef enum {piedra, puente, hielo, metal} tipo;
	tipo t;
	int xi, xf, y;
	static bool comparar_plataforma (const plataforma& p1, const plataforma& p2);
};

class configuracion
{
public:
	configuracion ();
	~configuracion ();
	std::string obtener_s (const char *camino, std::function<bool(std::string & cadena, bool omision)> validar);
	int obtener_i (const char *camino, std::function<bool(int n, bool omision)> validar);
	long obtener_l (const char *camino, std::function<bool(long n, bool omision)> validar);
	long long obtener_ll (const char *camino, std::function<bool(long long n, bool omision)> validar);
	unsigned long obtener_ul (const char *camino, std::function<bool(unsigned long n, bool omision)> validar);
	unsigned long long obtener_ull (const char *camino, std::function<bool(unsigned long long n, bool omision)> validar);
	float obtener_f (const char *camino, std::function<bool(float n, bool omision)> validar);
	double obtener_d (const char *camino, std::function<bool(double n, bool omision)> validar);
	long double obtener_ld (const char *camino, std::function<bool(long double n, bool omision)> validar);
	SDL_Texture *obtener_textura (const char *camino, SDL_Renderer *renderer, std::function<bool(SDL_Texture *textura, bool omision)> validar);
	void obtener_plataformas (const char *camino, std::list<plataforma> &l);
private:
	template<typename t> class tipos
	{
	public:
		typedef t (*funcion_i) (const std::string&, size_t*, int);
		typedef t (*funcion_pf) (const std::string&, size_t*);
	};
	xmlDoc *doc, *doc_omision;
	xmlXPathContextPtr contexto, contexto_omision;
	void *dlopen_handle;
	uint8_t *obtener_direccion_de_simbolo (std::string &imagen);
	std::string obtener_s_del_xml (const char *camino, xmlXPathContextPtr *contexto);
	template <class t> t obtener_i_number (const char *camino, typename tipos<t>::funcion_i funcion, std::function<bool(int i, bool omision)> validar);
	template <typename t> t obtener_fp_number (const char *camino, typename tipos<t>::funcion_pf funcion, std::function<bool(int i, bool omision)> validar);
	void recrear_archivo_xml ();
	xmlNode * agregar_nodo_raiz (xmlDoc *doc, std::string & nombre);
	xmlNode * agregar_nodo_hijo (xmlDoc *doc, xmlNode *nodo_padre, std::string & nombre);
	void obtener_plataformas (const char *camino, std::function<void(xmlNode *node)> funcion);
};

class excepcion_configuracion: public std::invalid_argument
{
public:
	explicit excepcion_configuracion (const std::string &que_paso);
};

#ifndef IOCONTRA_HPP
	#include "iocontra.hpp"
#endif

#endif
