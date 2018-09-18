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
	maximaVelocidadX = 4;
	maximaVelocidadY = 16;
	gravedad = 1;

	posX = 50;
	posY = 280;
	estado = Quieto;
	saltando = false;

	rectDestino = {posX,posY,80,80};
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
	estado = Caminando;
}

void Personaje::saltar(){
	//esto podria cambiar si tenemos estados como SaltandoYAvanzando o Saltando y otra cosa
	//deberia decir if(esta en una plataforma)
	if( !saltando ){
		velocidadY = - maximaVelocidadY;
		saltando = true;
	}
}

//--------Destructor-------

Personaje::~Personaje() {
	//se destruye en el padre
}

