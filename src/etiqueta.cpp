#include <iostream>
#include "etiqueta.hpp"
#include "Constantes.h"

extern "C"
{
}

etiqueta::etiqueta (int x, int y, int puntos):
	control (x, y),
	f ("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", puntos),
	//f ("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 32),
	//f ("/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf", 32),
	//f ("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 32),
	//f ("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32),
	//f ("/usr/share/fonts/truetype/tlwg/TlwgMono.ttf", 32),
	color_texto ({170, 170, 170, alpha})
{
}

etiqueta::~etiqueta ()
{
}

void etiqueta::texto (const char *t)
{
	if (t) {
		s.clear ();
		s.append (t);
	}
}

std::string etiqueta::texto () const
{
	return s;
}

void etiqueta::metricas (SDL_Renderer *renderer, int &ancho, int &alto)
{
	f.dibujar (renderer, " ", x, y, color_texto, ancho, alto);
	ancho *= s.length ();
}

void etiqueta::dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer)
{
	// Tanto la obtencion del tamaño del control como la posicion del mismo deberian
	// calcularse solo de ser necesario, realizando un seguimiento de los cambios que
	// sufre el control. Pero para la duración de este proyecto se prefiere por su
	// simplicidad esta solución.
	// Obtengo el ancho y el alto de un espacio, sirve porque uso una fuente mono-espaciada.
	int char_w, texto_h;
	f.dibujar (renderer, " ", x, y, color_texto, char_w, texto_h);
	// const int texto_w = s.length () * char_w;
	w = s.length () * char_w;
	h = texto_h;
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
	// Texto
	if (s.length () > 0) {
		f.dibujar (renderer, s.c_str (), xx, yy, color_texto);
	}
}

void etiqueta::color (const SDL_Color &color)
{
	color_texto = color;
}
