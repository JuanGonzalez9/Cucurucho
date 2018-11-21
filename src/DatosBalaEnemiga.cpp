#include "DatosBalaEnemiga.h"

DatosBalaEnemiga::DatosBalaEnemiga() {
	posX = 0;
	posY = 0;
	tipo = Constantes::sinBala;
}

//...........................set......................

void DatosBalaEnemiga::setPosX(int x){
	this->posX = x;
}

void DatosBalaEnemiga::setPosY(int y){
	this->posY = y;
}

void DatosBalaEnemiga::setTipoArma(Constantes::TipoArma tipoDeBala){
	this->tipo = tipoDeBala;
}


//...........................get......................
int DatosBalaEnemiga::getPosX(){
	return posX;
}

int DatosBalaEnemiga::getPosY(){
	return posY;
}

Constantes::TipoArma DatosBalaEnemiga::getTipoArma(){
	return tipo;
}

DatosBalaEnemiga::~DatosBalaEnemiga() {
}
