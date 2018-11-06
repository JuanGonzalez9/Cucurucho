#include <iostream>
#include <panel.hpp>

panel::panel (int x, int y, int w, int h):
	contenedor (x, y),
	color_fondo ({0, 0, 0, alpha}),
	w (w),
	h (h)
{
}

panel::~panel ()
{
}

bool panel::manejar_evento (SDL_Event e)
{
	return contenedor::manejar_evento (e);
}

void panel::dibujar(int cx, int cy, int cw, int ch, SDL_Renderer *renderer)
{
	activar_blend (renderer);	
	int xx = cx, yy = cy;
	switch (anclado_x) {
		case medio:
			xx += (cw - w)/2;
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
			break;
		case opuesto:
			yy += ch - h - y;
			break;
		default: // case normal:
			yy += y;
			break;
	};
	SDL_SetRenderDrawColor (renderer, color_fondo.r, color_fondo.g, color_fondo.b, color_fondo.a);
	SDL_Rect r = {xx, yy, w, h};
	SDL_RenderFillRect (renderer, &r);
	restaurar_blend (renderer);
	contenedor::dibujar (xx, yy, w, h, renderer);
}

void panel::redimensionar (int w, int h)
{
	this->w = w;
	this->h = h;
}

