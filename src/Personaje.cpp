/*
 * Personaje.cpp
 *
 *  Created on: Sep 18, 2018
 *      Author: juan
 */

#include "Personaje.h"

Personaje::Personaje() {
	EntidadDibujable();
	velocidadY = 0;
	velocidadX = 0;
	maximaVelocidadX = 3;
	maximaVelocidadY = 14;
	gravedad = 1;

	posX = 50;
	posY = 280;
	estado = Quieto;
	direccionDisparo = Centro;
	saltando = false;
	disparando = false;

	rectDestino = {posX,posY,34,72};

	spritesJugador = new Sprite();
}

//--------GET----SET

Personaje::Estado Personaje::getEstado(){
	return estado;
}

int Personaje::getPosX(){
	return posX;
}

int Personaje::getPosY(){
	return posY;
}

//-------Metodos----------

void Personaje::actualizar(){

	if(posY >= 270 && velocidadY > 0){
		//entra al if si esta cayendo y esta cerca de la plataforma

		posY = 280; //deberia decir la altura de la plataforma
		velocidadY = 0;
		saltando = false;
	}
	posX += velocidadX;
	posY += velocidadY;
	rectDestino.x = posX;
	rectDestino.y = posY;

	if(velocidadX == 0 && !saltando) estado = Quieto;
	if(saltando) velocidadY += gravedad;
	velocidadX = 0;
}

void Personaje::avanzar(){
	velocidadX = maximaVelocidadX;
	estado = Caminando;
}

void Personaje::retroceder(){
	velocidadX = - maximaVelocidadX;
	estado = Retrocediendo;
}

void Personaje::disparar(){
	disparando = true;
}

void Personaje::dejarDeDisparar(){
	disparando = false;
}

void Personaje::apuntarAbajo(){
	direccionDisparo = Abajo;
}

void Personaje::apuntarArriba(){
	direccionDisparo = Arriba;
}

void Personaje::dejarDeApuntar(){
	direccionDisparo = Centro;
}

void Personaje::saltar(){
	//esto podria cambiar si tenemos estados como SaltandoYAvanzando o Saltando y otra cosa
	//deberia decir if(esta en una plataforma)
	if( !saltando ){
		velocidadY = - maximaVelocidadY;
		saltando = true;
	}
}

void Personaje::dibujar(SDL_Renderer* renderer){
	SDL_Rect rect_origen;
	if(saltando){
		rect_origen = spritesJugador->getFrameSaltando();
	}
	else{
		switch(estado){
			case Caminando:
				if(disparando){
					if(direccionDisparo == Centro)
						rect_origen = spritesJugador->getFrameDisparandoCaminando();
					if(direccionDisparo == Arriba)
						rect_origen = spritesJugador->getFrameDisparandoArribaCaminando();
					if(direccionDisparo == Abajo)
						rect_origen = spritesJugador->getFrameDisparandoAbajoCaminando();
				}
				else
					rect_origen = spritesJugador->getFrameCaminando();
				break;
			case Retrocediendo:
				if(disparando){
					if(direccionDisparo == Centro)
						rect_origen = spritesJugador->getFrameDisparandoCaminando();
					if(direccionDisparo == Arriba)
						rect_origen = spritesJugador->getFrameDisparandoArribaCaminando();
					if(direccionDisparo == Abajo)
						rect_origen = spritesJugador->getFrameDisparandoAbajoCaminando();
				}
				else
					rect_origen = spritesJugador->getFrameCaminando();
				SDL_RenderCopyEx(renderer,textura,& rect_origen,&rectDestino,180.0,NULL,SDL_FLIP_VERTICAL);
				return;
			default:
				if(disparando){
					if(direccionDisparo == Centro)
						rect_origen = spritesJugador->getFrameDisparando();
					if(direccionDisparo == Arriba)
						rect_origen = spritesJugador->getFrameDisparandoArriba();
					if(direccionDisparo == Abajo)
						rect_origen = spritesJugador->getFrameDisparandoAbajo();
				}
				else
					rect_origen = spritesJugador->getFrameQuieto();
				break;
		}
	}

	SDL_RenderCopy(renderer, textura, & rect_origen , &rectDestino);
}

//--------Destructor-------

Personaje::~Personaje() {
	//se destruye en el padre
}

