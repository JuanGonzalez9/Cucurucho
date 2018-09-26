#ifndef PLATAFORMAS_HPP
#define PLATAFORMAS_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <list>
#include "cfg.hpp"
#include "iocontra.hpp"

using namespace std;

class Plataformas {
public:
	
	Plataformas();
	~Plataformas();	

	void inicializar(SDL_Renderer* renderizador);
	void cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador);
	SDL_Texture* crearTexturaParaElFondo(SDL_Texture*,SDL_Renderer*,int w,int h);
	bool hayColision(int otroX, int otroY, int otroW, int otroH);
	bool hayColisionSuperior(int otroX, int otroY, int otroW, int otroH);
	int hayPlataformaEn(int otroX);
	int aproximarPosicionAPlataforma(int otroX, int otroY, int otroW, int otroH, int velocidad);

protected:
	SDL_Texture* texturaPlataformaPiedra;
	SDL_Texture* texturaPlataformaPuente;
	SDL_Texture* texturaPlataformaHielo;
	SDL_Texture* texturaPlataformaMetal;
	list<plataforma> lista_plataformas;
};

#endif
