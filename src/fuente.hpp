#ifndef FUENTE_HPP
#define FUENTE_HPP

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

class fuente
{
public:
	fuente (const char *camino, int puntos);
	~fuente ();
	void dibujar (SDL_Renderer *renderer, const char *texto, int x, int y, const SDL_Color &color, int &w, int &h);
	void dibujar (SDL_Renderer *renderer, const char *texto, int x, int y, const SDL_Color &color);
protected:
	TTF_Font *f;
};

#endif
