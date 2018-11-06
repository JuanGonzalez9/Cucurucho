#include <iostream>
#include "dialogo.hpp"
#include "Constantes.h"

dialogo::dialogo (ventana &v):
	contenedor_principal (v),
	c (0),
	d (1),
	ba (10),
	bx (0),
	bd (1)
{
}

dialogo::~dialogo ()
{
}

void dialogo::actualizar()
{
	// ATENCION: acá solo se actualiza la logica de la animación.
	c += d;
	if (c > 255) {
		d = -1;
		c = 254;
	} else if (c < 0) {
		d = 1;
		c = 1;
	}

	bx += bd;
	if (bx + ba - 1 >= MUNDO_ANCHO) {
		bd = -1;
		bx = MUNDO_ANCHO - ba - 1;
	} else if (bx < 0) {
		bd = 1;
		bx = 1;
	}
}

void dialogo::dibujar ()
{
	// ATENCION: acá solo se dibuja la animación.
	SDL_SetRenderDrawColor (v.renderer (), c, c, c, 255);
	SDL_RenderClear (v.renderer ());

	SDL_Rect r = {bx, 0, ba, MUNDO_ALTO};
	SDL_SetRenderDrawColor (v.renderer (), 255 - c, 255 - c, 255 - c, 255);
	SDL_RenderFillRect (v.renderer (), &r);

	contenedor_principal::dibujar ();
}

