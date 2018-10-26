/*
 * JuegoCliente.cpp
 *
 *  Created on: Oct 23, 2018
 *      Author: juan
 */

#include "JuegoCliente.h"

JuegoCliente::JuegoCliente(string comportamiento, int cantidadJugadores) : juego(comportamiento,cantidadJugadores){
	//juego(comportamiento);
	miPersonajeSprites = new Sprite();
	nivel = 1;
	cantJugadores = cantidadJugadores;

	bala_rectOrigen = {0,0,32,32};
	bala_rectDestino = {0,0,8,8};
}

void JuegoCliente::setMensajeDelServidor(string msj){
	p.parsear(msj,cantJugadores);
	if(nivel == 1 && p.getNivel() == 2){
		actualizarNivel1();
		nivel = 2;
	}
	else if(nivel == 2 && p.getNivel() == 3){
		actualizarNivel2();
		nivel = 3;	
	}
}

void JuegoCliente::actualizarFondos(){
	int nivel = p.getNivel();

	if(nivel == 1 || nivel == 3){
		fondo1.setRectOrigenX(p.getPosFondo1());
		fondo2.setRectOrigenX(p.getPosFondo2());
		rect_origen_fondo3.x = p.getPosFondo3();
	}
	else{
		fondo1.setRectOrigenY(p.getPosFondo1());
		fondo2.setRectOrigenY(p.getPosFondo2());
		rect_origen_fondo3.y = p.getPosFondo3();	
	}
}

void JuegoCliente::actualizarPosicionDeMiPersonaje(int numeroPersonaje){
	Personaje* unBoby;
	switch(numeroPersonaje){
		case (1):
			unBoby = &boby;
			break;
		case (2):
			unBoby = &boby2;
			break;
		default:
			break;
	}

	unBoby->setPosX(p.getPosPersonajeX(numeroPersonaje));
	unBoby->setPosY(p.getPosPersonajeY(numeroPersonaje));
}

void JuegoCliente::actualizarEstadoDeMiPersonaje(int numeroP){
	Personaje* unBoby;
	switch(numeroP){
		case (1):
			unBoby = &boby;
			break;
		case (2):
			unBoby = &boby2;
			break;
		default:
			break;
	}
	unBoby->setEstados(p.estaSaltando(numeroP),p.estaDisparando(numeroP),p.estaMirandoALaDerecha(numeroP),p.estaActivo(numeroP),p.estaGrisado(numeroP));
	unBoby->setEstadosEnumerados(p.getEstado(numeroP),p.getDireccionDisparo(numeroP));
}

void JuegoCliente::dibujarBalas(vector< pair<int,int> > balas){
	for(unsigned i = 0; i < balas.size();i++){
		bala_rectDestino.x = balas[i].first;
		bala_rectDestino.y = balas[i].second;
		SDL_RenderCopy(renderer, textura_bala, & bala_rectOrigen , & bala_rectDestino);
	}
}

void JuegoCliente::dibujar(){
	SDL_SetTextureBlendMode (textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget (renderer, textura_objetivo);

	actualizarFondos();

	fondo1.dibujarFondo(renderer);
	fondo2.dibujarFondo(renderer);
	SDL_RenderCopy (renderer, textura_fondo3, &rect_origen_fondo3, nullptr);

	SDL_SetRenderTarget (renderer, nullptr);
	SDL_RenderCopy (renderer, textura_objetivo, nullptr, nullptr);

	actualizarPosicionDeMiPersonaje(1);
	actualizarEstadoDeMiPersonaje(1);
	boby.dibujar(renderer);

	if (num_jugadores >= 2 && boby2.esActivo()){
		actualizarPosicionDeMiPersonaje(2);
		actualizarEstadoDeMiPersonaje(2);
		boby2.dibujar(renderer);
	}

	vector< pair<int,int> > balas = p.getBalas();
	dibujarBalas(balas);
}

JuegoCliente::~JuegoCliente() {
	miPersonajeSprites->~Sprite();
}


