#include <iostream>
#include <sstream>
#include "mensaje.hpp"

mensaje::mensaje (ventana &v):
	dialogo (v),
	pnl (180, 200, 440, 200),
	aceptar (10, 10, "Aceptar")
{
	pnl.anclado_x = control::medio;
	pnl.anclado_y = control::medio;
	agregar (&pnl, false);
	
	aceptar.anclado_x = control::opuesto;
	aceptar.anclado_y = control::opuesto;
	aceptar.al_presionar_callback = [this]() {al_aceptar();};
	pnl.agregar (&aceptar, false);

	popular_enfocables ();
}

mensaje::~mensaje()
{
}

void mensaje::vaciar ()
{
	std::list<etiqueta*>::const_iterator i;
	for (i = etiquetas.begin (); i != etiquetas.end (); ++i) {
		etiqueta *e = *i;
		delete (e);
	}
	etiquetas.clear ();
}

void mensaje::correr(const std::string &msj)
{
	vaciar ();
	pnl.vaciar ();
	std::istringstream is (msj.c_str());
	std::string s;
	int y = 15;
	int max_ancho = 0, ancho, alto;
	while (getline (is, s, '\n')) {
		std::cout << s << "\n";
		etiqueta *e = new etiqueta (0, y, 16);
		e->texto (s.c_str ());
		pnl.agregar (e, false);
		e->anclado_x = control::medio;
		e->color ({200, 200, 200, alpha});
		e->metricas (v.renderer (), ancho, alto);
		y += alto + 8;
		if (max_ancho < ancho) {
			max_ancho = ancho;
		}
		pnl.agregar (&aceptar, false);
		etiquetas.push_back (e);
	}
	pnl.redimensionar (max_ancho + 40, y + 10 + 40);
	dialogo::correr();
}

void mensaje::manejar_eventos()
{
	dialogo::manejar_eventos();
}

bool mensaje::manejar_evento (SDL_Event e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_RETURN:
					al_aceptar ();
					return true;
			};
	}
	return dialogo::manejar_evento (e);
}

void mensaje::al_aceptar ()
{
	corriendo = false;
}

void mensaje::actualizar ()
{
	dialogo::actualizar ();
}

void mensaje::dibujar ()
{
	dialogo::dibujar ();
}

