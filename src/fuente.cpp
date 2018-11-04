#include <iostream>
#include <fuente.hpp>

fuente::fuente (const char *camino, int puntos)
{
	if (TTF_Init () == -1) {
		std::cerr << "No pudo inicializarse SDL_ttf: " << SDL_GetError () << '\n';
		return;
	}
	f = TTF_OpenFont (camino, puntos);
	if (nullptr == f) {
		std::cerr << "No pudo crease la fuente: " << SDL_GetError () << '\n';
		return;
	}
}

fuente::~fuente ()
{
	TTF_CloseFont (f);
}

void fuente::dibujar (SDL_Renderer *renderer, const char *texto, int x, int y, const SDL_Color & color, int &w, int &h)
{
	SDL_Surface *superficie = TTF_RenderText_Blended( f, texto, color);
	if (nullptr == superficie) {
		std::cerr << "No pudo crease la superficie: " << SDL_GetError () << '\n';
		return;
	}

	SDL_Texture *textura = SDL_CreateTextureFromSurface (renderer, superficie);
	if (nullptr == textura) {
		std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
		SDL_FreeSurface (superficie);
		return;
	}

	SDL_Rect area = {x, y, superficie->w, superficie->h};
	int r = SDL_RenderCopy (renderer, textura, nullptr, &area);
	if (0 != r) {
		std::cerr << "No se pudo copiar la textura: " << SDL_GetError () << '\n';
		SDL_FreeSurface (superficie);
		return;
	}
	w = superficie->w;
	h = superficie->h;
                   
	SDL_DestroyTexture (textura);
	SDL_FreeSurface (superficie);
	return;
}

void fuente::dibujar (SDL_Renderer *renderer, const char *texto, int x, int y, const SDL_Color & color)
{
	int w, h;
	dibujar (renderer, texto, x, y, color, w, h);
}

