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

bool traductorDelCliente::quiereAccion(Constantes::BitAccion accion){
	if(mensajeSinTraducir[accion] == 1){
		return true;
	}
	return false;
}

traductorDelCliente::~traductorDelCliente() {
	// TODO Auto-generated destructor stub
}

