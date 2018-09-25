/*
 * EntidadDibujable.h
 *
 *  Created on: Sep 16, 2018
 *      Author: juan
 */

#ifndef ENTIDADDIBUJABLE_H_
#define ENTIDADDIBUJABLE_H_

#include <string>
#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
using namespace std;

class EntidadDibujable {
public:
	EntidadDibujable();
	virtual ~EntidadDibujable();

	// GETTER SETTER
	void crearTextura(const char* path,SDL_Renderer* renderer);
	void obtenerTextura(const char* path,SDL_Renderer* renderer);
	void asignarTextura(SDL_Texture* textura);
	SDL_Rect getRectaOrigen();
	SDL_Rect getRectaDestino();
	SDL_Texture* getTextura();
	void setRectOrigen(int x,int y,int w,int h);
	void setRectDestino(int x,int y,int w,int h);


	// METODOS
	void dibujarFondo(SDL_Renderer* renderer);
	void dibujar(SDL_Renderer* renderer);
	// estos dos modifican la posicion en x
	void avanzarOrigen(int velocidad);
	void retrocederOrigen(int velocidad);

protected:

	SDL_Texture* textura;
	SDL_Rect rectOrigen;
	SDL_Rect rectDestino;
};

#endif /* ENTIDADDIBUJABLE_H_ */
