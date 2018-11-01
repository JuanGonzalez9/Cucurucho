#ifndef PLATAFORMAS_HPP
#define PLATAFORMAS_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <list>
#include "cfg.hpp"
#include "iocontra.hpp"

using namespace std;

class ImagePlataforma
{
public:
	ImagePlataforma ();
	~ImagePlataforma ();
	void inicializar (const char *camino, SDL_Renderer* renderizador);
	SDL_Texture* textura;
	int ancho, alto, delta;
};

class Plataformas {
public:
	
	Plataformas();
	~Plataformas();	

	void inicializar(SDL_Renderer* renderizador);
	void cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador,int nivel);
	SDL_Texture* crearTexturaParaElFondo(SDL_Texture*,SDL_Renderer*,int w,int h);
	bool hayColision(int otroX, int otroY, int otroW, int otroH, int nivel);
	bool hayColisionSuperior(int otroX, int otroY, int otroW, int otroH, int nivel);
	plataforma hayPlataformaEn(int coordenada, int nivel, int rangoAtras, int rangoAdelante);
	int aproximarPosicionAPlataforma(int otroX, int otroY, int otroW, int otroH, int velocidad, int nivel);

protected:
	ImagePlataforma imagenes[plataforma::cantidad];
	list<plataforma> lista_plataformas_nivel1;
	list<plataforma> lista_plataformas_nivel2;
	list<plataforma> lista_plataformas_nivel3;
};

#endif
