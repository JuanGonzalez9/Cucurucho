#include <iostream>
#include "boton.hpp"
#include "cfg.hpp"
#include "Constantes.h"

extern "C"
{
}

boton::boton (int x, int y, const char *s):
	control (x, y),
	padding_x (7),
	padding_y (5),
	borde_x (3),
	borde_y (3),
	f ("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18),
	//f ("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 32),
	//f ("/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/tlwg/TlwgMono.ttf", 32),
	//color_borde ({80, 80, 80, 255}),
	//color_borde_foco ({100, 100, 100, 255}),
	color_borde ({20, 20, 20, 40}),
	color_borde_foco ({70, 70, 70, 60}),
	color_borde_resaltado ({80, 80, 80, 80}),
	color_fondo ({0, 0, 0, 40}),
	color_fondo_foco ({50, 50, 50, 60}),
	color_fondo_resaltado ({60, 60, 60, 80}),
	color_texto ({200, 200, 200, alpha})
{
	this->s = s;
}

boton::~boton ()
{
}

void boton::texto (const char *t)
{
	if (t) {
		s.clear ();
		s.append (t);
	}
}

std::string boton::texto () const
{
	return s;
}

bool boton::enfocable ()
{
	return true;
}

void boton::foco (bool e)
{
	control::foco (e);
}

bool boton::manejar_evento (SDL_Event &e)
{
	switch (e.type){
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					al_presionar (x, y);
					return true;
			};
	            break;
	}
	return control::manejar_evento (e);
}


void boton::al_presionar (int x, int y)
{
	if (al_presionar_callback) {
		al_presionar_callback ();
	}
}

void boton::dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer)
{
	activar_blend (renderer);
	// Tanto la obtencion del tamaño del control como la posicion del mismo deberian
	// calcularse solo de ser necesario, realizando un seguimiento de los cambios que
	// sufre el control. Pero para la duración de este proyecto se prefiere por su
	// simplicidad esta solución.
	// Obtengo el ancho y el alto de un espacio, sirve porque uso una fuente mono-espaciada.
	f.dibujar (renderer, " ", x, y, color_texto, char_w, char_h);
	const int texto_w = s.length () * char_w;
	const int texto_h = char_h;
	w = s.length () * char_w + 2*borde_x + 2*padding_x;
	h = texto_h + 2*borde_y + 2*padding_y;
	// Calculo posicion
	int xx = cx, yy = cy;
	switch (anclado_x) {
		case medio:
			xx += (cw - w)/2;
			x = xx - cx; // Como no utilizo el x lo guardo
			break;
		case opuesto:
			xx += cw - w - x;
			break;
		default: // case normal:
			xx += x;
			break;
	};
	switch (anclado_y) {
		case medio:
			yy += (ch - h)/2;
			y = yy - cy;
			break;
		case opuesto:
			yy += ch - h - y;
			break;
		default: // case normal:
			yy += y;
			break;
	};
	// Relleno
	SDL_Rect r = {xx + borde_x, yy + borde_y, texto_w + 2*padding_x, texto_h + 2*padding_y};
	if (resaltado) {
		SDL_SetRenderDrawColor (renderer, color_fondo_resaltado.r, color_fondo_resaltado.g, color_fondo_resaltado.b, color_fondo_resaltado.a);
	} else if (enfocado) {
		SDL_SetRenderDrawColor (renderer, color_fondo_foco.r, color_fondo_foco.g, color_fondo_foco.b, color_fondo_foco.a);
	} else {
		SDL_SetRenderDrawColor (renderer, color_fondo.r, color_fondo.g, color_fondo.b, color_fondo.a);
	}
	SDL_RenderFillRect (renderer, &r);
	// Borde superior
	if (resaltado) {
		SDL_SetRenderDrawColor (renderer, color_borde_resaltado.r, color_borde_resaltado.g, color_borde_resaltado.b, color_borde_resaltado.a);
	} else if (enfocado) {
		SDL_SetRenderDrawColor (renderer, color_borde_foco.r, color_borde_foco.g, color_borde_foco.b, color_borde_foco.a);
	} else {
		SDL_SetRenderDrawColor (renderer, color_borde.r, color_borde.g, color_borde.b, color_borde.a);
	}
	r = {xx, yy, w, borde_y};
	SDL_RenderFillRect (renderer, &r);
	// Borde inferior
	r = {xx, yy + h - borde_y, w, borde_y};
	SDL_RenderFillRect (renderer, &r);
	// Borde izquierdo
	r = {xx, yy + borde_y, borde_x, h - 2*borde_y};
	SDL_RenderFillRect (renderer, &r);
	// Borde derecho
	r = {xx + w - borde_x, yy + borde_y, borde_x, h - 2*borde_y};
	SDL_RenderFillRect (renderer, &r);
	// Rectangulo de foco
	/*
	if (enfocado) {
		r = {xx + 1, yy + 1, w - 2, h - 2};
		SDL_SetRenderDrawColor (renderer, color_borde_foco.r, color_borde_foco.g, color_borde_foco.b, color_borde_foco.a);
		SDL_RenderDrawRect (renderer, &r);
	}
	*/
	// Texto
	if (s.length () > 0) {
		f.dibujar (renderer, s.c_str (), xx + borde_x + padding_x, yy + borde_y + padding_y, color_texto);
	}
	restaurar_blend (renderer);
}

