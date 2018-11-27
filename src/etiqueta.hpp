#ifndef ETIQUETA_HPP
#define ETIQUETA_HPP

#include <string>
#include "control.hpp"
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class etiqueta: public control
{
public:
	etiqueta (int x, int y, int puntos);
	virtual ~etiqueta ();
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer);
	void texto (const char *texto);
	std::string texto () const;
	void metricas (SDL_Renderer *renderer, int &ancho, int &alto);
	void color (const SDL_Color &color);
	void borde (int ancho, SDL_Color color);
protected:
	std::string s;
	fuente f;
	SDL_Color color_texto;
};

#endif
