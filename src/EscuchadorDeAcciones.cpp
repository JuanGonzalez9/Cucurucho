/*
 * EscuchadorDeAcciones.cpp
 *
 *  Created on: Oct 15, 2018
 *      Author: juan
 */

#include "EscuchadorDeAcciones.h"

EscuchadorDeAcciones::EscuchadorDeAcciones() {
	jugando = true;
	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		cout<<"No se pudo inicializar SDL"<<endl;
		jugando = false;
	}
}

string EscuchadorDeAcciones::obtenerAcciones(){
	SDL_PumpEvents();
	acciones.reset();

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	if(apretandoDerecha(state)){
		acciones.set(Constantes::derecha); //pone el bit 0 en 1 indicando que se esta apretando la tecla derecha
	}

	if(apretandoIzquierda(state)){
		acciones.set(Constantes::izquierda);
	}

	if(apretandoArriba(state)){
		acciones.set(Constantes::arriba);
	}

	if(apretandoAbajo(state)){
		acciones.set(Constantes::abajo);
	}

	if(apretandoDisparo(state)){
		acciones.set(Constantes::disparo);
	}

	if(apretandoSalto(state)){
		acciones.set(Constantes::salto);
	}

	if(apretandoAgacharse(state)){
		acciones.set(Constantes::agacharse);
	}

	if(apretandoNivel2(state)){
		acciones.set(Constantes::nivel2);
	}

	if(apretandoNivel3(state)){
		acciones.set(Constantes::nivel3);
	}

	return acciones.to_string();
}

bool EscuchadorDeAcciones::enAccion(){
	return jugando;
}

bool EscuchadorDeAcciones::apretandoDerecha(const Uint8* state){
	//controla tambien que no este la tecla izquierda
	return (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]);
}

bool EscuchadorDeAcciones::apretandoIzquierda(const Uint8* state){
	//controla tambien que no este la tecla derecha
	return (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]);
}

bool EscuchadorDeAcciones::apretandoArriba(const Uint8* state){
	return state[SDL_SCANCODE_UP];
}

bool EscuchadorDeAcciones::apretandoDisparo(const Uint8* state){
	return state[SDL_SCANCODE_Z];
}

bool EscuchadorDeAcciones::apretandoSalto(const Uint8* state){
	return state[SDL_SCANCODE_X];
}

bool EscuchadorDeAcciones::apretandoAbajo(const Uint8* state){
	return state[SDL_SCANCODE_DOWN];
}

bool EscuchadorDeAcciones::apretandoAgacharse(const Uint8* state){
	return state[SDL_SCANCODE_C];
}

//Cambia de nivel
bool EscuchadorDeAcciones::apretandoNivel2(const Uint8* state){
	return state[SDL_SCANCODE_Q];
}

//sube al jugador
bool EscuchadorDeAcciones::apretandoNivel3(const Uint8* state){
	return state[SDL_SCANCODE_W];
}

EscuchadorDeAcciones::~EscuchadorDeAcciones() {
	SDL_Quit();
}

