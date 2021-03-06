#include <iostream>
#include <contenedor.hpp>

contenedor::contenedor (int x, int y):
	control (x, y)
{
}

contenedor::~contenedor ()
{
	vaciar ();
}

void contenedor::popular (std::list<control*> &enfocables)
{
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		control *c = (*i)->c;
		c->popular (enfocables);
	}
}

bool contenedor::manejar_evento (SDL_Event event)
{
	return control::manejar_evento (event);
}

void contenedor::dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer)
{
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		//((elemento*)*i)->c->dibujar (cx, cy, cw, ch, renderer);
		((elemento*)*i)->c->dibujar (cx+this->x, cy+this->y, this->w, this->h, renderer);
	}
}

void contenedor::agregar (control *c, bool liberar)
{
	elemento * e = new elemento {c, liberar};
	controles.push_back (e);
	c->establecer_padre (this);
}

void contenedor::remover (control *c)
{
	std::list<elemento*>::const_iterator i;
	for (i = controles.begin (); i != controles.end (); ++i) {
		elemento *e = *i;
		if (e->c == c) {
			if (e->liberar) {
				delete (e->c);
			}
			delete (e);
			controles.erase (i);
			return;
		}
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















