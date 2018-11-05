#include <iostream>
#include "control.hpp"
#include "Constantes.h"

extern "C"
{
}

control::control (int x, int y):
	anclado_x (normal),
	anclado_y (normal),
	x (x),
	y (y),
	enfocado (false)
{
}

control::~control ()
{
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

