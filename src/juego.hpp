#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "temporizador.hpp"
#include "plataformas.hpp"

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
	int us, cuadros, fps, d1, d2, d3;
	temporizador t_ciclo, t_fps;
	SDL_Event e;
	SDL_Rect rect_origen_fondo1, rect_origen_fondo2,  rect_origen_fondo3, rect_origen_bob, rect_destino_bob;
	SDL_Window *ventana;
	SDL_Renderer *renderer;
	SDL_Surface *imagen_fondo1, *imagen_fondo2, *imagen_fondo3, *imagen_bob;
	SDL_Texture *textura_fondo1, *textura_fondo2, *textura_fondo3, *textura_bob, *textura_objetivo;
	Plataformas plataformas;
};

#endif
