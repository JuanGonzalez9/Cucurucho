#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "temporizador.hpp"
#include "plataformas.hpp"
#include "EntidadDibujable.h"
#include "Personaje.h"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <stdlib.h>
}

class juego
{
public:
	juego ();
	~juego ();
	void manejar_eventos ();
	void actualizar ();
	void dibujar ();
	void presentar ();
	bool jugando ();
protected:
	bool termino;
	int us, cuadros, d1, d2, d3;
	temporizador t_ciclo, t_fps;
	SDL_Event e;
	SDL_Rect rect_origen_fondo3;
	SDL_Window *ventana;
	SDL_Renderer *renderer;
	SDL_Surface *imagen_fondo3;
	SDL_Texture *textura_fondo3, *textura_objetivo;
	Plataformas plataformas;

	//probando
	EntidadDibujable* fondo1;
	EntidadDibujable* fondo2;
	Personaje* boby;

	bool apretandoDerecha(const Uint8* state);
	bool apretandoIzquierda(const Uint8* state);
	bool apretandoArriba(const Uint8* state);

};

#endif
