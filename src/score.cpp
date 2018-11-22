#include <iostream>
#include <sstream>
#include "score.hpp"

static const int fs = 18;  // font size
static const int pl = 20;  // primera linea
static const int sl = 33;  // separacion lineas
static const int ac = 120; // ancho columna
#define av (80 + (jugadores+(jugadores < 4 ? 2 : 1))*ac)
#define px ((av-(jugadores+1)*ac)/2)

score::score (ventana &v, int jugadores):
	dialogo (v),
	pnl (180, 200, av, pl+sl*6+20),
	nivel_1 (px, pl+sl*1, fs),
	nivel_2 (px, pl+sl*2, fs),
	nivel_3 (px, pl+sl*3, fs),
	total (px, pl+sl*4, fs),
	jugador_1 (px+1*120, pl, fs),
	jugador_2 (px+2*120, pl, fs),
	jugador_3 (px+3*120, pl, fs),
	jugador_4 (px+4*120, pl, fs),
	aceptar (10, 10, "Aceptar")
{
	pnl.anclado_x = control::medio;
	pnl.anclado_y = control::medio;
	agregar (&pnl, false);
	
	aceptar.anclado_x = control::opuesto;
	aceptar.anclado_y = control::opuesto;
	aceptar.al_presionar_callback = [this]() {al_aceptar();};
	pnl.agregar (&aceptar, false);

	nivel_1.texto ("Nivel 1");
	pnl.agregar (&nivel_1, false);

	nivel_2.texto ("Nivel 2");
	pnl.agregar (&nivel_2, false);

	nivel_3.texto ("Nivel 3");
	pnl.agregar (&nivel_3, false);

	total.texto ("Total");
	pnl.agregar (&total, false);

	jugador_1.texto ("Jugador 1");
	pnl.agregar (&jugador_1, false);

	if (jugadores >= 2) {
		jugador_2.texto ("Jugador 2");
		pnl.agregar (&jugador_2, false);
	}

	if (jugadores >= 3) {
		jugador_3.texto ("Jugador 3");
		pnl.agregar (&jugador_3, false);
	}

	if (jugadores >= 4) {
		jugador_4.texto ("Jugador 4");
		pnl.agregar (&jugador_4, false);
	}

	popular_enfocables ();
}

score::~score()
{
}

void score::manejar_eventos()
{
	dialogo::manejar_eventos();
}

bool score::manejar_evento (SDL_Event e)
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

void score::al_aceptar ()
{
	corriendo = false;
}

void score::actualizar ()
{
	dialogo::actualizar ();
}

void score::dibujar ()
{
	dialogo::dibujar ();
}

