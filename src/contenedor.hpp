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

class contenedor: public control
{
public:
	contenedor (int x, int y);
	virtual ~contenedor ();
	virtual bool manejar_evento (SDL_Event e);
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer);
	virtual void popular (std::list<control*> &enfocables);

	void agregar (control *c, bool liberar);
	void vaciar ();
protected:
	bool corriendo;
	std::list<elemento*> controles;
};

#endif
