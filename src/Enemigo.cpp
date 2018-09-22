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
	rectOrigen = {0,0,126,120};
	rectDestino = {x,y,126,120};

}

void Enemigo::perderVida(){
	vidas--;
	cout<<"tengo vidas: "<<vidas<<endl;
}

bool Enemigo::derrotado(){
	return (vidas <= 0);
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}

