#include <iostream>
#include <contenedor.hpp>

contenedor::contenedor (ventana &v):
	v (v),
	corriendo (false)
{
}

contenedor::~contenedor ()
{
	vaciar ();
}

void contenedor::correr()
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

void contenedor::manejar_eventos()
{
	SDL_Event event;
	while (SDL_PollEvent (&event) != 0) {
		bool manejado = false;
		std::list<elemento*>::const_iterator i;
		for (i = controles.begin (); i != controles.end (); ++i) {
			elemento *e = *i;
			if (e->c->foco ()) {
				manejado = e->c->manejar_evento (event);
				break;
			}
		}
		if (!manejado) {
			manejar_evento (event);
		}
	}
}

void contenedor::manejar_evento (SDL_Event e)
{
	switch (e.type){
		case SDL_QUIT:
			corriendo = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_TAB) {
				std::cout << "SDLK_TAB\n";
				if (e.key.keysym.mod & KMOD_SHIFT) {
					enfocar_anterior ();
				} else {
					enfocar_siguiente ();
				}
			}
			break;
	}
	
}

void contenedor::dibujar ()
{
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		((elemento*)*i)->c->dibujar (v.renderer ());
	}
}

void contenedor::agregar (control *c, bool liberar)
{
	elemento * e = new elemento {c, liberar};
	controles.push_back (e);
	if (!c->enfocable ()) {
		enfocar (c);
	}
}

void contenedor::vaciar ()
{
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		elemento *e = *i;
		if (e->liberar) {
			delete (e->c);
		}
		delete (e);
	}
	controles.clear ();
}

void contenedor::enfocar (control *c)
{
	if (!c->enfocable ()) {
		return;
	}
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		elemento *e = *i;
		if (e->c->foco () && e->c != c) {
			e->c->foco (false);
		}
		if (e->c == c) {
			e->c->foco (true);
		}
	}
}

void contenedor::enfocar_siguiente ()
{
	control *c = nullptr;
	std::list<elemento*>::const_iterator i;
	// Busco el enfocado actualmente
	for (i = controles.begin (); i != controles.end (); ++i) {
		elemento *e = *i;
		if (e->c->foco ()) {
			std::cout << "encontre el enfocado\n";
			c = e->c;
			break;
		}
	}
	if (!c) {
		return;
	}
	// Busco el siguiente enfocable a partir del siguiente
	++i;
	while (i != controles.end ()) {
		elemento *e = *i;
		if (e->c->enfocable ()) {
			std::cout << "encontre el siguiente enfocable 1\n";
			c->foco (false);
			e->c->foco (true);
			return;
		}
		++i;
	}
	// Busco el siguiente enfocable a partir del primero
	for (i = controles.begin (); i != controles.end (); ++i) {
		elemento *e = *i;
		if (e->c == c) { // Dio una vuelta sin encontrar otro enfocable
			std::cout << "no hay siguiente enfocable 1\n";
			return;
		}
		if (e->c->enfocable ()) {
			std::cout << "encontre el siguiente enfocable 2\n";
			c->foco (false);
			e->c->foco (true);
			return;
		}
	}
}

void contenedor::enfocar_anterior ()
{
	control *c = nullptr;
	std::list<elemento*>::reverse_iterator i;
	// Busco el enfocado actualmente
	for (i = controles.rbegin (); i != controles.rend (); ++i) {
		elemento *e = *i;
		if (e->c->foco ()) {
			c = e->c;
			break;
		}
	}
	if (!c) {
		return;
	}
	// Busco el anterior enfocable a partir del anterior
	++i;
	while (i != controles.rend ()) {
		elemento *e = *i;
		if (e->c->enfocable ()) {
			c->foco (false);
			e->c->foco (true);
			return;
		}
		++i;
	}
	// Busco el anterior enfocable a partir del ultimo
	for (i = controles.rbegin (); i != controles.rend (); ++i) {
		elemento *e = *i;
		if (e->c == c) { // Dio una vuelta sin encontrar otro enfocable
			return;
		}
		if (e->c->enfocable ()) {
			c->foco (false);
			e->c->foco (true);
			return;
		}
	}
}
















