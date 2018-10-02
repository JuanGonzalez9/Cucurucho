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
	void cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador,int nivel);
	SDL_Texture* crearTexturaParaElFondo(SDL_Texture*,SDL_Renderer*,int w,int h);
	bool hayColision(int otroX, int otroY, int otroW, int otroH, int nivel);
	bool hayColisionSuperior(int otroX, int otroY, int otroW, int otroH, int nivel);
	int hayPlataformaEn(int otroX, int nivel);
	int aproximarPosicionAPlataforma(int otroX, int otroY, int otroW, int otroH, int velocidad, int nivel);

protected:
	SDL_Texture* texturaPlataformaPiedra;
	SDL_Texture* texturaPlataformaPuente;
	SDL_Texture* texturaPlataformaPuenteSoga;
	SDL_Texture* texturaPlataformaPuente2;
	SDL_Texture* texturaPlataformaCascRoca1;
	SDL_Texture* texturaPlataformaCascRoca2;
	SDL_Texture* texturaPlataformaCascRoca3;
	SDL_Texture* texturaPlataformaCascRoca4;
	SDL_Texture* texturaPlataformaPiedraS;
	SDL_Texture* texturaPlataformaPiedraM;
	SDL_Texture* texturaPlataformaPiedraL;
	SDL_Texture* texturaPlataformaPiedraXL;
	SDL_Texture* texturaPlataformaHielo;
	SDL_Texture* texturaPlataformaHielo1;
	SDL_Texture* texturaPlataformaNieve;
	SDL_Texture* texturaPlataformaMetal;
	list<plataforma> lista_plataformas_nivel1;
	list<plataforma> lista_plataformas_nivel2;
	list<plataforma> lista_plataformas_nivel3;
};

#endif
