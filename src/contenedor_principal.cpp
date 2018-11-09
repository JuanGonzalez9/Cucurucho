#include <iostream>
#include "contenedor_principal.hpp"
#include "cfg.hpp"
#include "Constantes.h"

contenedor_principal::contenedor_principal (ventana &v):
	contenedor (0, 0),
	v (v),
	corriendo (false),
	control_resaltado (nullptr)
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

void contenedor_principal::sincronizada (std::function<void()> funcion)
{
	std::unique_lock<std::mutex> lock(mutex_sinc);
	//std::cout << "Espero ejecucion de funcion sincronizada\n";
	funcion_sinc = funcion;
	ejecutada_sinc = false;
	condicion_sinc.wait (lock, [this]{return this->ejecutada_sinc;});
	//std::cout << "Fin: Espero ejecucion de funcion sincronizada\n";
}

void contenedor_principal::ejecutar_sincronizada ()
{
	std::unique_lock<std::mutex> lock(mutex_sinc);
	if (funcion_sinc) {
		//std::cout << "Ejecuto funcion sincronizada\n";
		funcion_sinc ();
		funcion_sinc = nullptr;
		ejecutada_sinc = true;
		condicion_sinc.notify_all ();
		//std::cout << "Fin: Ejecuto funcion sincronizada\n";
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
	ejecutar_sincronizada ();
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
			break;
		case SDL_MOUSEMOTION:
			resaltar (e.button.x, e.button.y);
			return true;
		case SDL_MOUSEBUTTONUP:
			enfocar (e.button.x, e.button.y);
			return true;
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

void contenedor_principal::resaltar (int x, int y)
{
	if (control_resaltado) {
		control_resaltado->resaltado = false;
		control_resaltado = nullptr;
	}
	std::list<control*>::reverse_iterator i;
	for (i = enfocables.rbegin (); i != enfocables.rend (); ++i) {
		control *c = *i;
		int rx = x, ry = y;
		c->relativas (rx, ry);
		if (c->contiene (rx, ry)) {
			c->al_mover (rx, ry);
			control_resaltado = c;
			control_resaltado->resaltado = true;
			return;
		}
	}
	cfg.establecer_cursor_flecha ();
}

void contenedor_principal::enfocar (int x, int y)
{
	std::list<control*>::reverse_iterator i;
	for (i = enfocables.rbegin (); i != enfocables.rend (); ++i) {
		control *c = *i;
		int rx = x, ry = y;
		c->relativas (rx, ry);
		if (c->contiene (rx, ry)) {
			enfocar (c);
			c->al_presionar (rx, ry);
			return;
		}
	}
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
		c->foco (false);
		e->foco (true);
	} else {
		// Enfoco el primero
		i = enfocables.begin ();
		if (i != enfocables.end ()) {
			control *e = *i;
			if (e != c) {
				c->foco (false);
				e->foco (true);
			}
			// else Hay un solo control y ya tiene el foco
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
		c->foco (false);
		e->foco (true);
	} else {
		// Enfoco el primero
		i = enfocables.rbegin ();
		if (i != enfocables.rend ()) {
			control *e = *i;
			if (e != c) {
				c->foco (false);
				e->foco (true);
			}
			// else Hay un solo control y ya tiene el foco
		}
	}
}


