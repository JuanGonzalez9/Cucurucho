/*
 * Parser.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: juan
 */

#include "Parser.h"

Parser::Parser() {
	cantJugadores = 1;
	nivel = 1;
	posFondo1 = 0;
	posFondo2 = 0;
	posFondo3 = 0;
	cantBalas = 0;
	hayEnemigo = false;
}

void Parser::parsear(string msj,int jugadores){
	cantJugadores = jugadores;
	resetearBalas();

	nivel = msj[0] - '0';
	posFondo1 = stoi(msj.substr(1,4));
	posFondo2 = stoi(msj.substr(5,4));
	posFondo3 = stoi(msj.substr(9,4));
	
	parsearPersonaje(msj,13,1);

	if(cantJugadores >= 2){
		parsearPersonaje(msj,27,2);		
	}

	if(cantJugadores >= 3){
		parsearPersonaje(msj,41,3);		
	}

	if(cantJugadores == 4){
		parsearPersonaje(msj,55,4);		
	}

	cantBalas = stoi(msj.substr(13 + RESPUESTA_PERSONAJE * cantJugadores,2));
	parsearBalas(msj.substr(13 + RESPUESTA_PERSONAJE * cantJugadores + 2,TAMANIO_POS_BALAS * cantBalas));

	hayEnemigo = (msj[13 + RESPUESTA_PERSONAJE * cantJugadores + 2 + TAMANIO_POS_BALAS * MAX_BALAS] == '1');	
}

DatosPersonaje* Parser::dameAlBobyNumero(int numeroDePersonaje){
	DatosPersonaje* datosP;
	switch(numeroDePersonaje){
		case (1):
			datosP = &datosBoby;
			break;
		case (2):
			datosP = &datosBoby2;
			break;
		case (3):
			datosP = &datosBoby3;
			break;
		case (4):
			datosP = &datosBoby4;
			break;
		default:
			break;
	}

	return datosP;
}


void Parser::parsearPersonaje(string msj,int i,int numeroDePersonaje){

	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	
	datosP->setPosX(stoi(msj.substr(i,3)));
	datosP->setPosY(parsearPosY(msj.substr(i+3,4)));

	datosP->setSaltando(msj[i+7] == '1');
	datosP->setDisparando(msj[i+8] == '1');
	datosP->setMirandoALaDerecha(msj[i+9] == '1');
	datosP->setActivo(msj[i+10] == '1');
	datosP->setGrisado(msj[i+11] == '1');

	datosP->setEstado((Constantes::Estado) (msj[i+12] - '0'));
	datosP->setDireccionDisparo((Constantes::DireccionDisparo) (msj[i+13] - '0'));
}

int Parser::parsearPosY(string substr){
	int respuesta;
	unsigned pos = substr.find_first_of("-");
	if(pos > 3) respuesta = stoi(substr);
	else{
		respuesta = stoi(substr.substr(pos,4-pos));
	}

	return respuesta;
}

int Parser::dameElInt(string sub){
	int respuesta;
	unsigned pos = sub.find_first_of("-");
	if(pos > 3) respuesta = stoi(sub);
	else{
		respuesta = stoi(sub.substr(pos,4-pos));
	}
	return respuesta;	
}

void Parser::parsearBalas(string sub){
	int x,y;
	for(int i = 0; i < cantBalas; i++){
		x = dameElInt(sub.substr(TAMANIO_POS_BALAS * i,4));
		y = dameElInt(sub.substr(TAMANIO_POS_BALAS * i + 4,4));
		pair<int,int> nuevaBala;
		nuevaBala.first = x;
		nuevaBala.second = y;
		balas.push_back(nuevaBala);
	}
}

void Parser::resetearBalas(){
	balas.clear();
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

int Parser::getPosPersonajeX(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getPosX();
}

int Parser::getPosPersonajeY(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getPosY();
}

bool Parser::estaSaltando(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaSaltando();
}

bool Parser::estaDisparando(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaDisparando();
}

bool Parser::estaMirandoALaDerecha(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaMirandoALaDerecha();
}

bool Parser::estaActivo(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaActivo();
}

bool Parser::estaGrisado(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaGrisado();
}

Constantes::Estado Parser::getEstado(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getEstado();
}

Constantes::DireccionDisparo Parser::getDireccionDisparo(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getDireccionDisparo();
}

bool Parser::estaElEnemigo(){
	return hayEnemigo;
}

vector< pair<int,int> > Parser::getBalas(){
	return balas;
}

Parser::~Parser() {
}

