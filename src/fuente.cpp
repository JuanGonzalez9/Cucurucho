#include <iostream>
#include <fuente.hpp>

fuente::fuente (const char *camino, int puntos):
	fo (nullptr),
	camino (camino),
	puntos (puntos)
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
	if (fo) {
		TTF_CloseFont (fo);
	}
}

void fuente::borde (int ancho, SDL_Color color_borde)
{
	if (fo == nullptr) {
		fo = TTF_OpenFont (camino.c_str(), puntos);
		if (nullptr == fo) {
			std::cerr << "No pudo crease la fuente: " << SDL_GetError () << '\n';
			return;
		}
	}
	ancho_borde = ancho;
	this->color_borde = color_borde;
	TTF_SetFontOutline(fo, ancho);
}

void fuente::dibujar (SDL_Renderer *renderer, const char *texto, int x, int y, const SDL_Color & color, int &w, int &h)
{
	if (fo != nullptr) {
		SDL_Surface *superficie = TTF_RenderText_Blended( fo, texto, color_borde);
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

		SDL_Rect area = {x-ancho_borde, y-ancho_borde, superficie->w, superficie->h};
		int r = SDL_RenderCopy (renderer, textura, nullptr, &area);
		if (0 != r) {
			std::cerr << "No se pudo copiar la textura: " << SDL_GetError () << '\n';
			SDL_FreeSurface (superficie);
			return;
		}
		             
		SDL_DestroyTexture (textura);
		SDL_FreeSurface (superficie);
	}
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

