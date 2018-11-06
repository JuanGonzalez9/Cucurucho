#ifndef PANEL_HPP
#define PANEL_HPP

#include "contenedor.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

class panel: public contenedor
{
public:
	panel (int x, int y, int w, int h);
	virtual ~panel ();

	virtual bool manejar_evento (SDL_Event e);
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer);
	void redimensionar (int w, int h);
	
	SDL_Color color_fondo;
protected:
	int w, h;
};

#endif
