/*
 * Enemigo.cpp
 *
 *  Created on: Sep 22, 2018
 *      Author: juan
 */

#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,int life) {

	EntidadDibujable();
	vidas = life;
	//rectOrigen = {0,0,126,120};
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};

}

void Enemigo::perderVida(){
	vidas--;
}

bool Enemigo::derrotado(){
	return (vidas <= 0);
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}

