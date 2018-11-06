#include <iostream>
#include "control.hpp"
#include "contenedor.hpp"
#include "Constantes.h"

extern "C"
{
}

control::control (int x, int y):
	anclado_x (normal),
	anclado_y (normal),
	x (x),
	y (y),
	enfocado (false),
	padre (nullptr)
{
}

control::~control ()
{
}

void control::activar_blend (SDL_Renderer *renderer)
{
	blend_ok = !SDL_GetRenderDrawBlendMode (renderer, &blend_mode);
	SDL_SetRenderDrawBlendMode (renderer, SDL_BLENDMODE_BLEND);
}

void control::restaurar_blend (SDL_Renderer *renderer)
{
	if (blend_ok) {
		SDL_SetRenderDrawBlendMode (renderer, blend_mode);
	}
}

void control::popular (std::list<control*> &enfocables)
{
	if (enfocable ()) {
		std::cout << "Agregado a enfocables: " << this << "\n";
		enfocables.push_back (this);
	}
}

bool control::enfocable ()
{
	return false;
}

void control::foco (bool e)
{
	enfocado = e;
}

bool control::foco () const
{
	return enfocado;
}

bool control::manejar_evento (SDL_Event &e)
{
	return false;
}

void control::establecer_padre (contenedor *c)
{
	padre = c;
}
