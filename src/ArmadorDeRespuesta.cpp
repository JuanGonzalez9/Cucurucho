/*
 * ArmadorDeRespuesta.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: juan
 */

#include "ArmadorDeRespuesta.h"

ArmadorDeRespuesta::ArmadorDeRespuesta() {
	nivel = 0;
	fondo1 = 0;
	fondo2 = 0;
	fondo3 = 0;

	int longitudStringBalas = MAX_BALAS * TAMANIO_POS_BALAS;
	string aux(longitudStringBalas,'0');
	strBalas = aux;
	resetBalas = aux;
	balasActuales = 0;
}

// --------------- setter getter
void ArmadorDeRespuesta::setNivel(int nivel){
	this->nivel = nivel;
}

void ArmadorDeRespuesta::setFondo1(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo1 = x;
	}
	else {
		fondo1 = y;
	}
}

void ArmadorDeRespuesta::setFondo2(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo2 = x;
	}
	else {
		fondo2 = y;
	}
}

void ArmadorDeRespuesta::setFondo3(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo3 = x;
	}
	else {
		fondo3 = y;
	}
}

void ArmadorDeRespuesta::setPosPersonaje(int x,int y){
	posPersonajeX = x;
	posPersonajeY = y;
}

/*void ArmadorDeRespuesta::setPosPersonaje2(int x,int y){
	posPersonajeX2 = x;
	posPersonajeY2 = y;
}*/

void ArmadorDeRespuesta::setSaltando(bool saltando){
	this->saltando = saltando;
}

/*void ArmadorDeRespuesta::setSaltando2(bool saltando){
	this->saltando2 = saltando;
}*/

void ArmadorDeRespuesta::setDisparando(bool disp){
	this->disparando = disp;
}

/*void ArmadorDeRespuesta::setDisparando2(bool disp){
	this->disparando2 = disp;
}*/

void ArmadorDeRespuesta::setMirandoALaDerecha(bool mirando){
	this->mirandoALaDerecha = mirando;
}

/*void ArmadorDeRespuesta::setMirandoALaDerecha2(bool mirando){
	this->mirandoALaDerecha2 = mirando;
}*/

void ArmadorDeRespuesta::setActivo(bool active){
	this->activo = active;
}

/*void ArmadorDeRespuesta::setActivo2(bool active){
	this->activo2 = active;
}*/

void ArmadorDeRespuesta::setGrisado(bool grisado){
	this->grisado = grisado;
}

/*void ArmadorDeRespuesta::setGrisado2(bool grisado){
	this->grisado2 = grisado;
}*/

void ArmadorDeRespuesta::setEstado(Constantes::Estado estado){
	this->estado = estado;
}

/*void ArmadorDeRespuesta::setEstado2(Constantes::Estado estado){
	this->estado2 = estado;
}*/

void ArmadorDeRespuesta::setDireccionDisparo(Constantes::DireccionDisparo dir){
	this->dirDisparo = dir;
}

/*void ArmadorDeRespuesta::setDireccionDisparo2(Constantes::DireccionDisparo dir){
	this->dirDisparo2 = dir;
}*/

void ArmadorDeRespuesta::setCantidadDeBalas(int balas){
	this->cantidadDeBalas = balas;
	balasActuales = 0;
}



//------------ METODOS ------------
string ArmadorDeRespuesta::fondoToString(int f){
	string respuesta = "0000";
	string nuevo = to_string(f);

	//para que la respuesta ocupe exactamente 4 caracteres
	respuesta.replace(4 - nuevo.size(),nuevo.size(),nuevo);

	return respuesta;
}

string ArmadorDeRespuesta::posicionToString(int p){

	string respuesta = "000";
	string nuevo = to_string(p);

	//para que la respuesta ocupe exactamente 4 caracteres
	respuesta.replace(3 - nuevo.size(),nuevo.size(),nuevo);

	return respuesta;
}

string ArmadorDeRespuesta::pasarAStringDeTamanio(int tamanio,int valor){

	string respuesta(tamanio,'0');

	string nuevo = to_string(valor);
	respuesta.replace(tamanio - nuevo.size(),nuevo.size(),nuevo);

	return respuesta;
}

void ArmadorDeRespuesta::sumarBalas(vector<Bullet*> nuevasBalas){
	for(unsigned i = 0; i < nuevasBalas.size();i++){
		string nuevoX = pasarAStringDeTamanio(RESPUESTA_POSX + 1,nuevasBalas[i]->getRectaDestino().x);
		
		strBalas.replace(TAMANIO_POS_BALAS * balasActuales,RESPUESTA_POSX + 1,nuevoX);

		string nuevoY = pasarAStringDeTamanio(RESPUESTA_POSY,nuevasBalas[i]->getRectaDestino().y);
		strBalas.replace(TAMANIO_POS_BALAS * balasActuales + RESPUESTA_POSX + 1,RESPUESTA_POSY,nuevoY);

		balasActuales++;
	}
}


/*string ArmadorDeRespuesta::dameLaRespuesta(){
	string respuesta = to_string(nivel);

	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo1);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo2);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo3);

	respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,posPersonajeX);
	respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,posPersonajeY);

	respuesta += to_string(saltando);
	respuesta += to_string(disparando);
	respuesta += to_string(mirandoALaDerecha);
	respuesta += to_string(activo);
	respuesta += to_string(grisado);

	respuesta += to_string(estado);
	respuesta += to_string(dirDisparo);


	respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,posPersonajeX2);
	respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,posPersonajeY2);

	respuesta += to_string(saltando2);
	respuesta += to_string(disparando2);
	respuesta += to_string(mirandoALaDerecha2);
	respuesta += to_string(activo2);
	respuesta += to_string(grisado2);

	respuesta += to_string(estado2);
	respuesta += to_string(dirDisparo2);

	respuesta += pasarAStringDeTamanio(MENSAJE_CANT_BALAS,cantidadDeBalas);
	respuesta += strBalas;
	strBalas = resetBalas;



	return respuesta;
}*/

string ArmadorDeRespuesta::dameLaRespuestaPara(int jugadores, DatosPersonaje* datosBoby, DatosPersonaje* datosBoby2, DatosPersonaje* datosBoby3, DatosPersonaje* datosBoby4){

	//Estan cargado desde juego los parametros universales para los 4 jugadores
	string respuesta = to_string(nivel);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo1);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo2);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo3);

	//Dependiendo los jugadores se codifica de alguna manera

	respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby->getPosX());
	respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby->getPosY());
	respuesta += to_string(datosBoby->estaSaltando());
	respuesta += to_string(datosBoby->estaDisparando());
	respuesta += to_string(datosBoby->estaMirandoALaDerecha());
	respuesta += to_string(datosBoby->estaActivo());
	respuesta += to_string(datosBoby->estaGrisado());
	respuesta += to_string(datosBoby->getEstado());
	respuesta += to_string(datosBoby->getDireccionDisparo());

	if(jugadores >= 2){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby2->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby2->getPosY());
		respuesta += to_string(datosBoby2->estaSaltando());
		respuesta += to_string(datosBoby2->estaDisparando());
		respuesta += to_string(datosBoby2->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby2->estaActivo());
		respuesta += to_string(datosBoby2->estaGrisado());
		respuesta += to_string(datosBoby2->getEstado());
		respuesta += to_string(datosBoby2->getDireccionDisparo());

	}

	if(jugadores >= 3){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby3->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby3->getPosY());
		respuesta += to_string(datosBoby3->estaSaltando());
		respuesta += to_string(datosBoby3->estaDisparando());
		respuesta += to_string(datosBoby3->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby3->estaActivo());
		respuesta += to_string(datosBoby3->estaGrisado());
		respuesta += to_string(datosBoby3->getEstado());
		respuesta += to_string(datosBoby3->getDireccionDisparo());
	}

	if(jugadores == 4){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby4->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby4->getPosY());
		respuesta += to_string(datosBoby4->estaSaltando());
		respuesta += to_string(datosBoby4->estaDisparando());
		respuesta += to_string(datosBoby4->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby4->estaActivo());
		respuesta += to_string(datosBoby4->estaGrisado());
		respuesta += to_string(datosBoby4->getEstado());
		respuesta += to_string(datosBoby4->getDireccionDisparo());
	}

	respuesta += pasarAStringDeTamanio(MENSAJE_CANT_BALAS,cantidadDeBalas);
	respuesta += strBalas;
	strBalas = resetBalas;

	return respuesta;

}

//------------ DESTRUCTOR------------------
ArmadorDeRespuesta::~ArmadorDeRespuesta() {
}

