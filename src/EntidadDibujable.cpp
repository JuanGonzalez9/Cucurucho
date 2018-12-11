/*
 * EntidadDibujable.cpp
 *
 *  Created on: Sep 16, 2018
 *      Author: juan
 */

#include "EntidadDibujable.h"
#include "iocontra.hpp"

// ----------------- CONSTRUCTOR -----------------------------
EntidadDibujable::EntidadDibujable():
	invertir_x(false),
	invertir_y(false)
{

	textura = NULL;

	// inicialmente ocupan toda la pantalla
	setRectOrigen(0,0,800,600);
	setRectDestino(0,0,800,600);
}
// ------------ GETTER & SETTER----------------------------------------------
// crearTextura
void EntidadDibujable::obtenerTextura(const char* path,SDL_Renderer* renderer){
	if(textura != NULL) SDL_DestroyTexture(textura);
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

void EntidadDibujable::setRectOrigenX(int x){
	rectOrigen.x = x;
}

void EntidadDibujable::setRectOrigenY(int y){
	rectOrigen.y = y;
}

void EntidadDibujable::setPos(int x,int y){
	rectDestino.x = x;
	rectDestino.y = y;
}

SDL_Texture* EntidadDibujable::getTextura(){
	return textura;
}

//-------------------------METODOS--------------------------------------------
void EntidadDibujable::dibujar(SDL_Renderer* ren){
	SDL_RendererFlip f = SDL_FLIP_NONE;
	if (invertir_x && invertir_y) {
		f = (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	} else if (invertir_x) {
		f = SDL_FLIP_HORIZONTAL;
	} else if (invertir_y) {
		f = SDL_FLIP_VERTICAL;
	}
	SDL_RenderCopyEx(ren,textura, &rectOrigen, &rectDestino, 0.0, nullptr, f);
	//SDL_RenderCopy(ren, textura, &rectOrigen , &rectDestino);
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

int EntidadDibujable::getRectDestX(){
	return(rectDestino.x);
}

void EntidadDibujable::increaseRectDestX(int x){
	rectDestino.x +=x;
}

int EntidadDibujable::getRectDestY(){
	return(rectDestino.y);
}

void EntidadDibujable::increaseRectDestY(int y){
	rectDestino.y +=y;
}





bool EntidadDibujable::collision(SDL_Rect rect1,SDL_Rect rect2){
	if(rect1.y >= rect2.y + rect2.h)
		return false;
	if(rect1.x >= rect2.x + rect2.w)
		return false;
	if(rect1.y + rect1.h <= rect2.y)
		return false;
	if(rect1.x + rect1.w <= rect2.x)
		return false;
	return true;
}

void EntidadDibujable::actualizarRectOrigenOriginal(){
	SDL_QueryTexture (textura, nullptr, nullptr, &rectOrigen.w, &rectOrigen.h);
}

// ---------------DESTRUCTOR -------------------------------------
EntidadDibujable::~EntidadDibujable() {
	if(textura != NULL) SDL_DestroyTexture(textura);
}

