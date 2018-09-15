#ifndef PLATAFORMAS_HPP
#define PLATAFORMAS_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>

class Plataformas {
public:
	
	Plataformas();
	~Plataformas();	

	void inicializar(SDL_Renderer* renderizador);
	void cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador);
	SDL_Texture* crearTexturaParaElFondo(SDL_Texture*,SDL_Renderer*,SDL_Surface*);

protected:
	SDL_Texture* texturaPlataforma;
};

#endif