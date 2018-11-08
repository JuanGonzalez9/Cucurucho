#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,int life) {

	EntidadDibujable();
	vidas = life;
	//rectOrigen = {0,0,126,120};
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
	posX=x;
	posY=y;
}

void Enemigo::perderVida(){
	vidas--;
}

bool Enemigo::derrotado(){
	return (vidas <= 0);
}

bool Enemigo::pasaBorde(int nivel){
	bool pasa=false;
	if (nivel ==2){
		pasa= ((posX+rectOrigen.w)<=0);
	}else{
		pasa= (posY>0);
	}
	return pasa;
}

void Enemigo::empujarAtras(int i, int nivel){
	if (nivel ==2){
		posX= posY+i;
		rectDestino.y=rectDestino.y+i;
	}else{
		posX= posX-i;
		rectDestino.x=rectDestino.x-i;
	}
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}

