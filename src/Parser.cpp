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
	cantBalas = 0;
}

void Parser::parsear(string msj){
	resetearBalas();

	printf("hola1%s\n",msj);
	nivel = msj[0] - '0';
	posFondo1 = stoi(msj.substr(1,4));
	posFondo2 = stoi(msj.substr(5,4));
	posFondo3 = stoi(msj.substr(9,4));

	printf("hola2\n");
	
	parsearPersonaje(msj,13);

	printf("hola3\n");
	parsearPersonaje(msj,27);

	printf("hola4\n");
	cantBalas = stoi(msj.substr(31,2));
	printf("hola5 %d\n", cantBalas);
	//parsearBalas(msj.substr(33,TAMANIO_POS_BALAS * cantBalas));

	printf("hola6\n");
	
}


void Parser::parsearPersonaje(string msj,int i){
	
	posPersonajeX = stoi(msj.substr(i,3));
	parsearPosY(msj.substr(i+3,4));

	if(msj[i+7] == '1') saltando = true;
	else saltando = false;

	if(msj[i+8] == '1') disparando = true;
	else disparando = false;

	if(msj[i+9] == '1') mirandoALaDerecha = true;
	else mirandoALaDerecha = false;

	if(msj[i+10] == '1') activo = true;
	else activo = false;

	if(msj[i+11] == '1') grisado = true;
	else grisado = false;

	estado = (Constantes::Estado) (msj[i+12] - '0');
	direccionDisparo = (Constantes::DireccionDisparo) (msj[i+13] - '0');



}

void Parser::parsearPosY(string substr){
	unsigned pos = substr.find_first_of("-");
	if(pos > 3) posPersonajeY = stoi(substr);
	else{
		posPersonajeY = stoi(substr.substr(pos,4-pos));
	}
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

int Parser::getPosPersonajeX2(){
	return posPersonajeX2;
}

int Parser::getPosPersonajeY2(){
	return posPersonajeY2;
}

bool Parser::estaSaltando2(){
	return saltando2;
}

bool Parser::estaDisparando2(){
	return disparando2;
}

bool Parser::estaMirandoALaDerecha2(){
	return mirandoALaDerecha2;
}

bool Parser::estaActivo2(){
	return activo2;
}

bool Parser::estaGrisado2(){
	return grisado2;
}

Constantes::Estado Parser::getEstado2(){
	return estado2;
}

Constantes::DireccionDisparo Parser::getDireccionDisparo2(){
	return direccionDisparo2;
}

vector< pair<int,int> > Parser::getBalas(){
	return balas;
}

Parser::~Parser() {
}

