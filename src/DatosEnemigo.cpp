#include "DatosEnemigo.h"

DatosEnemigo::DatosEnemigo() {
	posX = 0;
	posY = 0;
	tipo = Constantes::sinEnemigo;
}

//...........................set......................

void DatosEnemigo::setPosX(int x){
	this->posX = x;
}

void DatosEnemigo::setPosY(int y){
	this->posY = y;
}

void DatosEnemigo::setTipoEnemigo(Constantes::TipoEnemigo tipoDeEnemigo){
	this->tipo = tipoDeEnemigo;
}

/*void DatosPersonaje::setSaltando(bool saltando){
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
}*/


//...........................get......................
int DatosEnemigo::getPosX(){
	return posX;
}

int DatosEnemigo::getPosY(){
	return posY;
}

Constantes::TipoEnemigo DatosEnemigo::getTipoEnemigo(){
	return tipo;
}

/*bool DatosPersonaje::estaSaltando(){
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
}*/

DatosEnemigo::~DatosEnemigo() {
}
