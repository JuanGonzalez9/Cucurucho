#ifndef CONTENEDOR_HPP
#define CONTENEDOR_HPP

#include <list>
#include "ventana.hpp"
#include "control.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

struct elemento
{
	control * c;
	bool liberar;
};

class contenedor
{
public:
	contenedor (ventana &v);
	virtual ~contenedor ();
	virtual void correr();
	virtual void manejar_eventos();
	virtual void manejar_evento (SDL_Event e);
	virtual void actualizar() {}
	virtual void dibujar();

	void agregar (control *c, bool liberar);
	void vaciar ();
	void enfocar (control *c);
	void enfocar_siguiente ();
	void enfocar_anterior ();
protected:
	ventana &v;
	bool corriendo;
	std::list<elemento*> controles;
};

#endif
