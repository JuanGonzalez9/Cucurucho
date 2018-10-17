/*
 * traductorDelCliente.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: juan
 */

#include "traductorDelCliente.h"

traductorDelCliente::traductorDelCliente() {
}

void traductorDelCliente::setMensajeATraducir(char* mensaje){
	string strAux(mensaje);
	bitset<9> bitSetAux(strAux);
	mensajeSinTraducir = bitSetAux;
}

bool traductorDelCliente::quiereMoverDerecha(){
	if(mensajeSinTraducir[0] == 1){
		return true;
	}
	return false;
}

bool traductorDelCliente::quiereSaltar(){
	if(mensajeSinTraducir[5] == 1){
		return true;
	}
	return false;
}

bool traductorDelCliente::quiereMoverIzquierda(){
if(mensajeSinTraducir[1] == 1){
		return true;
	}
	return false;
}

traductorDelCliente::~traductorDelCliente() {
	// TODO Auto-generated destructor stub
}

