/*
 * JuegoCliente.cpp
 *
 *  Created on: Oct 23, 2018
 *      Author: juan
 */

#include "JuegoCliente.h"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_keycode.h>
}

JuegoCliente::JuegoCliente(ventana &v, int cantidadJugadores,int numeroDeJugador):
	juego (v, cantidadJugadores)
{
	this->numeroDeJugador = numeroDeJugador;
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

Personaje* JuegoCliente::dameAlBobyNumero(int numeroDeBoby){
	Personaje* unBoby;
	switch(numeroDeBoby){
		case (1):
			unBoby = &boby;
			break;
		case (2):
			unBoby = &boby2;
			break;
		case (3):
			unBoby = &boby3;
			break;
		case (4):
			unBoby = &boby4;
			break;
		default:
			break;
	}

	return unBoby;
}

void JuegoCliente::actualizarPosicionDeMiPersonaje(int numeroPersonaje){
	Personaje* unBoby = dameAlBobyNumero(numeroPersonaje);

	unBoby->setPosX(p.getPosPersonajeX(numeroPersonaje));
	unBoby->setPosY(p.getPosPersonajeY(numeroPersonaje));
}

void JuegoCliente::actualizarEstadoDeMiPersonaje(int numeroP){
	Personaje* unBoby = dameAlBobyNumero(numeroP);

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

void JuegoCliente::manejarEventos (){
	SDL_Event e;
	while (SDL_PollEvent (&e) != 0) { switch (e.type){
		case SDL_QUIT:
			termino = true;
			break;
		default:
			break;
	} }
}

void JuegoCliente::dibujarEnemigo(){
	switch(nivel){
		case (1):
			enemigoNivel1->dibujar(renderer);
			break;
		case (2):
			enemigoNivel2->dibujar(renderer);
			break;
		case (3):
			enemigoNivel3->dibujar(renderer);
			break;
		default:
			break;
	}
}

void JuegoCliente::dibujarJugadores(){
	Personaje* unBoby;
	for(int i = 0; i < num_jugadores; i++){
		unBoby = dameAlBobyNumero(i + 1);
		if(i != numeroDeJugador && unBoby->esActivo()){
			actualizarPosicionDeMiPersonaje(i+1);
			actualizarEstadoDeMiPersonaje(i+1);
			unBoby->dibujar(renderer);
		}
	}
	
	unBoby = dameAlBobyNumero(numeroDeJugador + 1);
	actualizarPosicionDeMiPersonaje(numeroDeJugador + 1);
	actualizarEstadoDeMiPersonaje(numeroDeJugador + 1);
	unBoby->dibujar(renderer);
	
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

	dibujarJugadores();

	vector< pair<int,int> > balas = p.getBalas();
	dibujarBalas(balas);

	if(p.estaElEnemigo()) dibujarEnemigo();
}

JuegoCliente::~JuegoCliente() {
}


