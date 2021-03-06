#include <iostream>
#include "editor.hpp"
#include "cfg.hpp"
#include "Constantes.h"

extern "C"
{
}

editor::editor (int x, int y, int largo):
	control (x, y),
	ocultar (false),
	cursor (0),
	cursor_visible (true),
	cursor_conteo (0),
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
	color_texto ({200, 200, 200, alpha}),
	//color_borde ({80, 80, 80, 255}),
	//color_borde_foco ({100, 100, 100, 255}),
	color_borde ({20, 20, 20, 0}),
	color_borde_foco ({20, 20, 20, 0}),
	color_fondo ({0, 0, 0, 40}),
	color_cursor ({150, 150, 150, alpha}),
	color_etiqueta ({100, 100, 100, alpha})
{
	limitar (largo);
}

editor::~editor ()
{
}

bool editor::enfocable ()
{
	return true;
}

void editor::foco (bool e)
{
	cursor_conteo = 0;
	cursor_visible = e;
	control::foco (e);
}

bool editor::manejar_evento (SDL_Event &e)
{
	switch (e.type){
		case SDL_TEXTINPUT:
			escribir (e.text.text[0]);
			return true;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					cursor_izquierda ();
					return true;
				case SDLK_RIGHT:
					cursor_derecha ();
					return true;
				case SDLK_HOME:
					cursor_principio ();
					return true;
				case SDLK_END:
					cursor_fin ();
					return true;
				case SDLK_BACKSPACE:
					borrar_izquierda ();
					return true;
				case SDLK_DELETE:
					borrar_derecha ();
					return true;
			};
	            break;
	}
	return control::manejar_evento (e);
}

void editor::limitar (unsigned int largo)
{
	this->largo = largo;
	if (entrada.length () > largo) {
		entrada.resize (largo);
		if (cursor > entrada.length()) {
			cursor = entrada.length();
		}
		std::cout << "limitar: " << entrada.length () << " " << largo << "\n";
	}
	if (etiqueta.length () > largo) {
		etiqueta.resize (largo);
	}
}

void editor::texto (const char *t, const char *e)
{
	if (t) {
		entrada.clear ();
		entrada.append (t);
		cursor = entrada.length();
	}
	if (e) {
		etiqueta.clear ();
		etiqueta.append (e);
	}
	limitar (largo);
	cursor = 0;
	mostrar ();
}

std::string editor::texto () const
{
	return entrada;
}

void editor::escribir (char c)
{
	if ((c >= 32 && c <= 127)) { // Caracteres ASCII imprimibles
		if( entrada.length () >= largo) {
			// TODO emitir sonido
		} else {
			entrada.insert (cursor, 1, c);
			cursor++;
			limitar (largo);
			mostrar ();
		}
	}
}

void editor::cursor_izquierda ()
{
	if (cursor > 0) {
		cursor--;
		mostrar ();
	}
}

void editor::cursor_derecha ()
{
	if (cursor < entrada.length ()) {
		cursor++;
		mostrar ();
	}
}

void editor::cursor_principio ()
{
	if (cursor > 0) { // El cursor no esta al principio
		cursor = 0;
		mostrar ();
	}
}

void editor::cursor_fin ()
{
	if (cursor < entrada.length ()) { // El cursor no esta al final
		cursor = entrada.length ();
		mostrar ();
	}
}

void editor::borrar_izquierda ()
{
	if (cursor > 0) { // Hay algo a la izquierda del cursor
		cursor--;
		entrada.erase (cursor, 1);
		mostrar ();
		std::cout << "borrar: " << entrada.length () << " " << largo << "\n";
	}
}

void editor::borrar_derecha ()
{
	if (cursor < entrada.length ()) { // Hay texto a la derecha del cursor
		entrada.erase (cursor, 1);
		mostrar ();
      }
}

void editor::mostrar ()
{
	#if 0
	std::cout << entrada << "\n";
	for (int i = 0; i < cursor; i++) {
		std::cout << " ";
	}
	std::cout << "^\n";
	#endif
}

void editor::actualizar_cursor ()
{
	cursor_conteo++;
	if (cursor_visible) {
		if (cursor_conteo > 80) {
			cursor_visible = false;
			cursor_conteo = 0;
		}
	} else {
		if (cursor_conteo > 30) {
			cursor_visible = true;
			cursor_conteo = 0;
		}
	}
}

void editor::al_mover (int x, int y)
{
	cfg.establecer_cursor_texto ();
}

void editor::al_presionar (int x, int y)
{
	x -= (borde_x + padding_x + char_w/2);
	cursor = x/char_w;
	if (cursor < 0) {
		cursor = 0;
	}
	if (cursor > entrada.length ()) {
		cursor = entrada.length ();
	}
}

void editor::dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer)
{
	activar_blend (renderer);
	// Tanto la obtencion del tamaño del control como la posicion del mismo deberian
	// calcularse solo de ser necesario, realizando un seguimiento de los cambios que
	// sufre el control. Pero para la duración de este proyecto se prefiere por su
	// simplicidad esta solución.
	// Obtengo el ancho y el alto de un espacio, sirve porque uso una fuente mono-espaciada.
	f.dibujar (renderer, " ", x, y, color_texto, char_w, char_h);
	const int texto_w = largo * char_w;
	const int texto_h = char_h;
	w = largo * char_w + 2*borde_x + 2*padding_x;
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
	SDL_SetRenderDrawColor (renderer, color_fondo.r, color_fondo.g, color_fondo.b, color_fondo.a);
	SDL_RenderFillRect (renderer, &r);
	// Borde superior
	if (enfocado) {
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
	r = {xx, yy, borde_x, h};
	SDL_RenderFillRect (renderer, &r);
	// Borde derecho
	r = {xx + w - borde_x, yy, borde_x, h};
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
	if (entrada.length () > 0) {
		if (ocultar) {
			std::string a;
			a.resize (entrada.length (), '*');
			f.dibujar (renderer, a.c_str (), xx + borde_x + padding_x, yy + borde_y + padding_y, activo ? color_texto : color_etiqueta);
		} else {
			f.dibujar (renderer, entrada.c_str (), xx + borde_x + padding_x, yy + borde_y + padding_y, activo ? color_texto : color_etiqueta);
		}
	} else {
		f.dibujar (renderer, etiqueta.c_str (), xx + borde_x + padding_x, yy + borde_y + padding_y, color_etiqueta);
	}
	// Cursor
	actualizar_cursor ();
	if (enfocado) {
		if (cursor_visible && activo) {
			SDL_SetRenderDrawColor (renderer, color_cursor.r, color_cursor.g, color_cursor.b, color_cursor.a);
			r = {xx + borde_x + padding_x + cursor * char_w - 1, yy + borde_y + padding_y, 2, texto_h};
			SDL_RenderDrawRect (renderer, &r);
		}
	} else {
		cursor_conteo = 0;
	}
	restaurar_blend (renderer);
}

