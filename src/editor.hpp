#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <string>
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class editor
{
public:
	editor (int x, int y, const SDL_Color & color);
	~editor ();
	void manejar_evento (SDL_Event &e);
	void dibujar (SDL_Renderer *renderer);
	void escribir (const char *texto);
protected:
	std::string texto;
	unsigned int cursor;
	int x, y;
	fuente f;
	SDL_Color color;
	void escribir (char c);
	void cursor_izquierda ();
	void cursor_derecha ();
	void cursor_principio ();
	void cursor_fin ();
	void borrar_izquierda ();
	void borrar_derecha ();
	void mostrar ();
};

#endif
