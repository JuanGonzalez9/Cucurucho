#ifndef FUENTE_HPP
#define FUENTE_HPP

#include <string>

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
	void borde (int ancho, SDL_Color color);
protected:
	TTF_Font *f, *fo;
	std::string camino;
	int puntos, ancho_borde;
	SDL_Color color_borde;
};

#endif
