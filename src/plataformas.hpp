#ifndef PLATAFORMAS_HPP
#define PLATAFORMAS_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <list>
#include "cfg.hpp"

using namespace std;

class Plataformas {
public:
	
	Plataformas();
	~Plataformas();	

	void inicializar(SDL_Renderer* renderizador);
	void cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador);
	SDL_Texture* crearTexturaParaElFondo(SDL_Texture*,SDL_Renderer*,SDL_Surface*);
	bool hayColision(int otroX, int otroY, int otroW, int otroH);

protected:
	SDL_Texture* texturaPlataforma;
	list<plataforma> lista_plataformas;
};

#endif