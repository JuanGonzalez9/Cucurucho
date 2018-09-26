#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "temporizador.hpp"
#include "plataformas.hpp"
#include "EntidadDibujable.h"
#include "Personaje.h"
#include "Enemigo.h"
#include "Bullet.h"
#include <vector>
#include "iocontra.hpp"
using namespace std;

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
	bool termino, cambioNivel;
	int us, cuadros, d1, d2, d3, nivel;
	int mundo_w, mundo_h;
	temporizador t_ciclo, t_fps;
	SDL_Event e;
	SDL_Rect rect_origen_fondo3;
	SDL_Window *ventana;
	SDL_Renderer *renderer;
	SDL_Texture *textura_fondo3, *textura_objetivo, *textura_bala;
	Plataformas plataformas;

	//balas
	bool collision(SDL_Rect rect1,SDL_Rect rect2);
	int direccionDeBala;

	//probando
	EntidadDibujable fondo1, fondo2;
	Personaje boby;
	Enemigo* darthBob;

	vector<Bullet*> bullets;

	bool apretandoDerecha(const Uint8* state);
	bool apretandoIzquierda(const Uint8* state);
	bool apretandoArriba(const Uint8* state);
	bool apretandoAbajo(const Uint8* state);
	bool apretandoDisparo(const Uint8* state);
	bool apretandoSalto(const Uint8* state);
	bool apretandoAgacharse(const Uint8* state);
	bool apretandoNivel(const Uint8* state);
	bool apretandoFlotar(const Uint8* state);


};

#endif
