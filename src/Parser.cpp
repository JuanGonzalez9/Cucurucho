/*
 * Parser.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: juan
 */

#include "Parser.h"

Parser::Parser() {
	nivel = 1;
	posFondo1 = 0;
	posFondo2 = 0;
	posFondo3 = 0;
	posPersonajeX = 0;
	posPersonajeY = 0;
	saltando = false;
	disparando = false;
	mirandoALaDerecha = false;
	activo = true;
	grisado = false;
	estado = Constantes::Quieto;
	direccionDisparo = Constantes::Centro;
}

void Parser::parsear(string msj){
	nivel = msj[0] - '0';
	posFondo1 = stoi(msj.substr(1,4));
	posFondo2 = stoi(msj.substr(5,4));
	posFondo3 = stoi(msj.substr(9,4));
	posPersonajeX = stoi(msj.substr(13,3));
	parsearPosY(msj.substr(16,4));

	if(msj[20] == '1') saltando = true;
	else saltando = false;

	if(msj[21] == '1') disparando = true;
	else disparando = false;

	if(msj[22] == '1') mirandoALaDerecha = true;
	else mirandoALaDerecha = false;

	if(msj[23] == '1') activo = true;
	else activo = false;

	if(msj[24] == '1') grisado = true;
	else grisado = false;

	estado = (Constantes::Estado) (msj[25] - '0');
	direccionDisparo = (Constantes::DireccionDisparo) (msj[26] - '0');
}

void Parser::parsearPosY(string substr){
	unsigned pos = substr.find_first_of("-");
	if(pos > 3) posPersonajeY = stoi(substr);
	else{
		posPersonajeY = stoi(substr.substr(pos,4-pos));
	}
}

int Parser::getNivel(){
	return nivel;
}

int Parser::getPosFondo1(){
	return posFondo1;
}

int Parser::getPosFondo2(){
	return posFondo2;
}

int Parser::getPosFondo3(){
	return posFondo3;
}

int Parser::getPosPersonajeX(){
	return posPersonajeX;
}

int Parser::getPosPersonajeY(){
	return posPersonajeY;
}

bool Parser::estaSaltando(){
	return saltando;
}

bool Parser::estaDisparando(){
	return disparando;
}

bool Parser::estaMirandoALaDerecha(){
	return mirandoALaDerecha;
}

bool Parser::estaActivo(){
	return activo;
}

bool Parser::estaGrisado(){
	return grisado;
}

Constantes::Estado Parser::getEstado(){
	return estado;
}

Constantes::DireccionDisparo Parser::getDireccionDisparo(){
	return direccionDisparo;
}

Parser::~Parser() {
}
