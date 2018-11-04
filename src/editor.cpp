#include <iostream>
#include "editor.hpp"

extern "C"
{
}

editor::editor (int x, int y, const SDL_Color & color):
	cursor (0),
	x (x),
	y (y),
	f ("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 26),
	//f ("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 32),
	//f ("/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 32),
	//fuente_editor ("/usr/share/fonts/truetype/tlwg/TlwgMono.ttf", 32),
	color (color)
{
}

editor::~editor ()
{
}

void editor::manejar_evento (SDL_Event &e)
{
	switch (e.type){
		case SDL_TEXTINPUT:
			escribir (e.text.text[0]);
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					cursor_izquierda ();
					break;
				case SDLK_RIGHT:
					cursor_derecha ();
					break;
				case SDLK_HOME:
					cursor_principio ();
					break;
				case SDLK_END:
					cursor_fin ();
					break;
				case SDLK_BACKSPACE:
					borrar_izquierda ();
					break;
				case SDLK_DELETE:
					borrar_derecha ();
					break;
			};
	            break;
		default:
			break;
	}
}

void editor::escribir (const char *t)
{
	texto.insert (cursor, t);
	cursor = texto.length();
	mostrar ();
}

void editor::escribir (char c)
{
	if ((c >= 32 && c <= 127)) { // Caracteres ASCII imprimibles
		texto.insert (cursor, 1, c);
		cursor++;
		mostrar ();
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
	if (cursor < texto.length ()) {
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
	if (cursor < texto.length ()) { // El cursor no esta al final
		cursor = texto.length ();
		mostrar ();
	}
}

void editor::borrar_izquierda ()
{
	if (cursor > 0) { // Hay algo a la izquierda del cursor
		cursor--;
		texto.erase (cursor, 1);
		mostrar ();
	}
}

void editor::borrar_derecha ()
{
	if (cursor < texto.length ()) { // Hay texto a la derecha del cursor
		texto.erase (cursor, 1);
		mostrar ();
      }
}

void editor::mostrar ()
{
	std::cout << texto << "\n";
	for (int i = 0; i < cursor; i++) {
		std::cout << " ";
	}
	std::cout << "^\n";
}

void editor::dibujar (SDL_Renderer *renderer)
{
	int char_w, char_h;
	f.dibujar (renderer, " ", x, y, color, char_w, char_h);
	if (texto.length () > 0) {
		f.dibujar (renderer, texto.c_str (), x, y, color);
	}
	SDL_SetRenderDrawColor (renderer, 100, 100, 100, 255);
	SDL_Rect r = {x + cursor * char_w - 1, y, 2, char_h};
	SDL_RenderDrawRect (renderer, &r);

#if 0
	int w, h, cx = x - 1;
	if (texto.length () > 0) {
		if (cursor == 0) { // No hay texto a la izquierda del cursor
			f.dibujar (renderer, texto.c_str (), x, y, color, w, h);
		} else if (cursor == texto.length ()) { // No hay texto a la derecha del cursor
			f.dibujar (renderer, texto.c_str (), x, y, color, w, h);
			cx += w;
		} else { // Hay texto a la izquierda y a la derecha del cursor
			std::string s = texto.substr (0, cursor);
			f.dibujar (renderer, s.c_str (), x, y, color, w, h);
			cx += w;
			s = texto.substr (cursor);
			f.dibujar (renderer, s.c_str (), x + w, y, color, w, h);
		}
	}
	f.dibujar (renderer, " ", x, y, color, w, h);
	SDL_SetRenderDrawColor (renderer, 100, 100, 100, 255);
	SDL_Rect r = {cx, y, 2, h};
	SDL_RenderDrawRect (renderer, &r);
#endif
}

