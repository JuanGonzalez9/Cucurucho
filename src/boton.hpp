#ifndef BOTON_HPP
#define BOTON_HPP

#include <string>
#include <functional>
#include "control.hpp"
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class boton: public control
{
public:
	boton (int x, int y, const char *texto);
	virtual ~boton ();
	virtual bool manejar_evento (SDL_Event &e);
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer);
	virtual void foco (bool enfocado);
	virtual bool enfocable ();
	virtual void al_presionar (int x, int y);
	std::function<void()> al_presionar_callback;
protected:
	std::string texto;
	int padding_x, padding_y, borde_x, borde_y, char_w, char_h;
	fuente f;
	SDL_Color color_borde, color_borde_foco, color_fondo, color_texto;
};

#endif
