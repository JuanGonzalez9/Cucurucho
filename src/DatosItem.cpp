#include "DatosItem.h"

DatosItem::DatosItem() {
	posX = 0;
	posY = 0;
	tipo = Constantes::sinItem;
}

//...........................set......................

void DatosItem::setPosX(int x){
	this->posX = x;
}

void DatosItem::setPosY(int y){
	this->posY = y;
}

void DatosItem::setTipoItem(Constantes::TipoItem tipoDeItem){
	this->tipo = tipoDeItem;
}


//...........................get......................
int DatosItem::getPosX(){
	return posX;
}

int DatosItem::getPosY(){
	return posY;
}

Constantes::TipoItem DatosItem::getTipoItem(){
	return tipo;
}

DatosItem::~DatosItem() {
}
