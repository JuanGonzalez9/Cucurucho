#include <iostream>
#include "contenedor_principal.hpp"
#include "Constantes.h"

contenedor_principal::contenedor_principal (ventana &v):
	contenedor (0, 0),
	v (v),
	corriendo (false)
{
}

contenedor_principal::~contenedor_principal ()
{
}

void contenedor_principal::popular_enfocables ()
{
	enfocables.clear ();
	contenedor::popular (enfocables) ;

	std::list<control*>::const_iterator i = enfocables.begin ();
	if (i != enfocables.end ()) {
		control *e = *i;
		enfocar (e);
	}
}

void contenedor_principal::correr()
{
	v.mostrar ();
	corriendo = true;
	while (corriendo) {
		//std::cout << "1\n";
		manejar_eventos();
		//std::cout << "2\n";
		actualizar();
		//std::cout << "3\n";
		dibujar();
		//std::cout << "4\n";
		v.presentar();
		//std::cout << "5\n";
	}
}

void contenedor_principal::manejar_eventos()
{
	SDL_Event event;
	while (SDL_PollEvent (&event) != 0) {
		manejar_evento (event);
	}
}

bool contenedor_principal::manejar_evento (SDL_Event e)
{
	switch (e.type){
		case SDL_QUIT:
			corriendo = false;
			return true;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_TAB) {
				if (e.key.keysym.mod & KMOD_SHIFT) {
					enfocar_anterior ();
				} else {
					enfocar_siguiente ();
				}
				return true;
			} else if (e.key.keysym.sym == SDLK_UP) {
				enfocar_anterior ();
				return true;
			} else if (e.key.keysym.sym == SDLK_DOWN) {
				enfocar_siguiente ();
				return true;
			}
	}
	std::list<control*>::const_iterator i;
	for (i = enfocables.begin (); i != enfocables.end (); ++i) {
		control *c = *i;
		if (c->foco () && c->manejar_evento (e)) {
			return true;
		}
	}
	return contenedor::manejar_evento (e);
}

void contenedor_principal::dibujar()
{
	contenedor::dibujar (x, y, MUNDO_ANCHO, MUNDO_ALTO, v.renderer ());
}

void contenedor_principal::enfocar (control *c)
{
	std::list<control*>::const_iterator i;
	for (i = enfocables.begin (); i != enfocables.end (); ++i) {
		control *e = *i;
		if (e->foco () && e != c) {
			e->foco (false);
		}
		if (e == c) {
			e->foco (true);
		}
	}
}

void contenedor_principal::enfocar_siguiente ()
{
	control *c = nullptr;
	std::list<control*>::const_iterator i;
	// Busco el enfocado actualmente
	for (i = enfocables.begin (); i != enfocables.end (); ++i) {
		control *e = *i;
		if (e->foco ()) {
			std::cout << "Encontre el enfocado\n";
			c = e;
			break;
		}
	}
	if (!c) {
		return;
	}
	// Enfoco el siguiente de existir
	++i;
	if (i != enfocables.end ()) {
		control *e = *i;
		std::cout << "Enfoco el siguiente\n";
		c->foco (false);
		e->foco (true);
	} else {
		// Enfoco el primero
		i = enfocables.begin ();
		if (i != enfocables.end ()) {
			control *e = *i;
			if (e == c) { // Hay un solo control y ya tiene el foco
				std::cout << "El control ya tiene el foco\n";
			} else {
				std::cout << "Enfoco el primero\n";
				c->foco (false);
				e->foco (true);
			}
		}
	}
}

void contenedor_principal::enfocar_anterior ()
{
	control *c = nullptr;
	std::list<control*>::reverse_iterator i;
	// Busco el enfocado actualmente
	for (i = enfocables.rbegin (); i != enfocables.rend (); ++i) {
		control *e = *i;
		if (e->foco ()) {
			std::cout << "Encontre el enfocado\n";
			c = e;
			break;
		}
	}
	if (!c) {
		return;
	}
	// Enfoco el siguiente de existir
	++i;
	if (i != enfocables.rend ()) {
		control *e = *i;
		std::cout << "Enfoco el siguiente\n";
		c->foco (false);
		e->foco (true);
	} else {
		// Enfoco el primero
		i = enfocables.rbegin ();
		if (i != enfocables.rend ()) {
			control *e = *i;
			if (e == c) { // Hay un solo control y ya tiene el foco
				std::cout << "El control ya tiene el foco\n";
			} else {
				std::cout << "Enfoco el primero\n";
				c->foco (false);
				e->foco (true);
			}
		}
	}
}

