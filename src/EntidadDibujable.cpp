/*
 * EntidadDibujable.cpp
 *
 *  Created on: Sep 16, 2018
 *      Author: juan
 */

#include "EntidadDibujable.h"
#include "iocontra.hpp"

// ----------------- CONSTRUCTOR -----------------------------
EntidadDibujable::EntidadDibujable() {

	textura = NULL;

	// inicialmente ocupan toda la pantalla
	setRectOrigen(0,0,800,600);
	setRectDestino(0,0,800,600);
}
// ------------ GETTER & SETTER----------------------------------------------
// crearTextura
void EntidadDibujable::obtenerTextura(const char* path,SDL_Renderer* renderer){
	textura = cfg.obtener_textura (path, renderer);
}

void EntidadDibujable::asignarTextura(SDL_Texture* tex){
	textura = tex;
}

SDL_Rect EntidadDibujable::getRectaDestino(){
	return rectDestino;
}

SDL_Rect EntidadDibujable::getRectaOrigen(){
	return rectOrigen;
}

void EntidadDibujable::setRectDestino(int x,int y,int w,int h){
	rectDestino = {x,y,w,h};
}

void EntidadDibujable::setRectOrigen(int x,int y,int w,int h){
	rectOrigen = {x,y,w,h};
}

SDL_Texture* EntidadDibujable::getTextura(){
	return textura;
}

//-------------------------METODOS--------------------------------------------
void EntidadDibujable::dibujar(SDL_Renderer* ren){
	SDL_RenderCopy(ren, textura, &rectOrigen , &rectDestino);
}

void EntidadDibujable::dibujarFondo(SDL_Renderer* ren){
	SDL_RenderCopy(ren, textura, &rectOrigen , nullptr);
}

void EntidadDibujable::avanzarOrigen(int velocidad){
	rectOrigen.x += velocidad;
}

void EntidadDibujable::avanzarOrigenY(int velocidad){
	rectOrigen.y += velocidad;
}

void EntidadDibujable::retrocederOrigen(int velocidad){
	rectOrigen.x -= velocidad;
}
// ---------------DESTRUCTOR -------------------------------------
EntidadDibujable::~EntidadDibujable() {
	if(textura != NULL) SDL_DestroyTexture(textura);
}

