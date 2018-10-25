#include "DatosPersonaje.h"

DatosPersonaje::DatosPersonaje() {
	posX = 0;
	posY = 0;
	saltando = false;
	disparando = false;
	mirandoALaDerecha = false;
	activo = true;
	grisado = false;
	estado = Constantes::Quieto;
	direccionDisparo = Constantes::Centro;
}

//...........................set......................

void DatosPersonaje::setPosX(int x){
	this->posX = x;
}

void DatosPersonaje::setPosY(int y){
	this->posY = y;
}

void DatosPersonaje::setSaltando(bool saltando){
	this->saltando = saltando;
}

void DatosPersonaje::setDisparando(bool disparando){
	this->disparando = disparando;
}

void DatosPersonaje::setMirandoALaDerecha(bool mirando){
	this->mirandoALaDerecha = mirando;
}

void DatosPersonaje::setActivo(bool act){
	this->activo = act;
}

void DatosPersonaje::setGrisado(bool gris){
	this->grisado = gris;
}

void DatosPersonaje::setEstado(Constantes::Estado est){
	this->estado = est;
}

void DatosPersonaje::setDireccionDisparo(Constantes::DireccionDisparo dir){
	this->direccionDisparo = dir;
}


//...........................get......................
int DatosPersonaje::getPosX(){
	return posX;
}

int DatosPersonaje::getPosY(){
	return posY;
}

bool DatosPersonaje::estaSaltando(){
	return saltando;
}

bool DatosPersonaje::estaDisparando(){
	return disparando;
}

bool DatosPersonaje::estaMirandoALaDerecha(){
	return mirandoALaDerecha;
}

bool DatosPersonaje::estaActivo(){
	return activo;
}

bool DatosPersonaje::estaGrisado(){
	return grisado;
}

Constantes::Estado DatosPersonaje::getEstado(){
	return estado;
}

Constantes::DireccionDisparo DatosPersonaje::getDireccionDisparo(){
	return direccionDisparo;
}

DatosPersonaje::~DatosPersonaje() {
}

