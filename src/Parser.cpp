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
	/*posPersonajeX = 0;
	posPersonajeY = 0;
	saltando = false;
	disparando = false;
	mirandoALaDerecha = false;
	activo = true;
	grisado = false;
	estado = Constantes::Quieto;
	direccionDisparo = Constantes::Centro;*/
	cantBalas = 0;
}

void Parser::parsear(string msj){
	resetearBalas();

	nivel = msj[0] - '0';
	posFondo1 = stoi(msj.substr(1,4));
	posFondo2 = stoi(msj.substr(5,4));
	posFondo3 = stoi(msj.substr(9,4));
	
	parsearPersonaje(msj,13,1);
	parsearPersonaje(msj,27,2);

	cantBalas = stoi(msj.substr(41,2));
	parsearBalas(msj.substr(43,TAMANIO_POS_BALAS * cantBalas));


	
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

int Parser::getPosPersonajeX(){
	return datosBoby.getPosX();
}

int Parser::getPosPersonajeY(){
	return datosBoby.getPosY();
}

bool Parser::estaSaltando(){
	return datosBoby.estaSaltando();
}

bool Parser::estaDisparando(){
	return datosBoby.estaDisparando();
}

bool Parser::estaMirandoALaDerecha(){
	return datosBoby.estaMirandoALaDerecha();
}

bool Parser::estaActivo(){
	return datosBoby.estaActivo();
}

bool Parser::estaGrisado(){
	return datosBoby.estaGrisado();
}

Constantes::Estado Parser::getEstado(){
	return datosBoby.getEstado();
}

Constantes::DireccionDisparo Parser::getDireccionDisparo(){
	return datosBoby.getDireccionDisparo();
}

int Parser::getPosPersonajeX2(){
	return datosBoby2.getPosX();
}

int Parser::getPosPersonajeY2(){
	return datosBoby2.getPosY();
}

bool Parser::estaSaltando2(){
	return datosBoby2.estaSaltando();
}

bool Parser::estaDisparando2(){
	return datosBoby2.estaDisparando();
}

bool Parser::estaMirandoALaDerecha2(){
	return datosBoby2.estaMirandoALaDerecha();
}

bool Parser::estaActivo2(){
	return datosBoby2.estaActivo();
}

bool Parser::estaGrisado2(){
	return datosBoby2.estaGrisado();
}

Constantes::Estado Parser::getEstado2(){
	return datosBoby2.getEstado();
}

Constantes::DireccionDisparo Parser::getDireccionDisparo2(){
	return datosBoby2.getDireccionDisparo();
}

vector< pair<int,int> > Parser::getBalas(){
	return balas;
}

Parser::~Parser() {
}

