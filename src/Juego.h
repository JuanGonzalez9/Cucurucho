/*
 * Juego.h
 *
 *  Created on: Sep 1, 2018
 *      Author: juan
 */

#ifndef JUEGO_H_
#define JUEGO_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Juego {
public:
	Juego();
	virtual ~Juego();

	void inicializar(const char* titulo,int posX,int posY,int ancho,int alto);
	void actualizar();
	void renderizar();
	void manejarEventos();
	void limpiar();

	bool jugando();


private:
	int sdlInicializado;
	bool estaJugando;
	SDL_Window* ventana;
	SDL_Renderer* renderer;
};

#endif /* JUEGO_H_ */
