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

void ArmadorDeRespuesta::setSaltando(bool saltando){
	this->saltando = saltando;
}

void ArmadorDeRespuesta::setDisparando(bool disp){
	this->disparando = disp;
}

void ArmadorDeRespuesta::setMirandoALaDerecha(bool mirando){
	this->mirandoALaDerecha = mirando;
}

void ArmadorDeRespuesta::setActivo(bool active){
	this->activo = active;
}

void ArmadorDeRespuesta::setGrisado(bool grisado){
	this->grisado = grisado;
}

void ArmadorDeRespuesta::setEstado(Constantes::Estado estado){
	this->estado = estado;
}

void ArmadorDeRespuesta::setDireccionDisparo(Constantes::DireccionDisparo dir){
	this->dirDisparo = dir;
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

string ArmadorDeRespuesta::dameLaRespuesta(){
	string respuesta = to_string(nivel);

	respuesta += fondoToString(fondo1);
	respuesta += fondoToString(fondo2);
	respuesta += fondoToString(fondo3);

	respuesta += posicionToString(posPersonajeX);
	respuesta += fondoToString(posPersonajeY);

	respuesta += to_string(saltando);
	respuesta += to_string(disparando);
	respuesta += to_string(mirandoALaDerecha);
	respuesta += to_string(activo);
	respuesta += to_string(grisado);

	respuesta += to_string(estado);
	respuesta += to_string(dirDisparo);

	return respuesta;
}
//------------ DESTRUCTOR
ArmadorDeRespuesta::~ArmadorDeRespuesta() {
}

