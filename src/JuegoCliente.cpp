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

	bala_rectOrigen = {0,0,32,32};
	bala_rectDestino = {0,0,8,8};
}

void JuegoCliente::setMensajeDelServidor(string msj){
	p.parsear(msj);
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

void JuegoCliente::actualizarPosicionDeMiPersonaje(){
	boby.setPosX(p.getPosPersonajeX());
	boby.setPosY(p.getPosPersonajeY());
}

void JuegoCliente::actualizarEstadoDeMiPersonaje(){
	boby.setEstados(p.estaSaltando(),p.estaDisparando(),p.estaMirandoALaDerecha(),p.estaActivo(),p.estaGrisado());
	boby.setEstadosEnumerados(p.getEstado(),p.getDireccionDisparo());
}

void JuegoCliente::actualizarPosicionDeMiPersonaje2(){
	boby2.setPosX(p.getPosPersonajeX2());
	boby2.setPosY(p.getPosPersonajeY2());
}

void JuegoCliente::actualizarEstadoDeMiPersonaje2(){
	boby2.setEstados(p.estaSaltando2(),p.estaDisparando2(),p.estaMirandoALaDerecha2(),p.estaActivo2(),p.estaGrisado2());
	boby2.setEstadosEnumerados(p.getEstado2(),p.getDireccionDisparo2());
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

	actualizarPosicionDeMiPersonaje();
	actualizarEstadoDeMiPersonaje();
	boby.dibujar(renderer);

	if (boby2.esActivo()){
		actualizarPosicionDeMiPersonaje2();
		actualizarEstadoDeMiPersonaje2();
		boby2.dibujar(renderer);
	}

	vector< pair<int,int> > balas = p.getBalas();
	dibujarBalas(balas);
}

JuegoCliente::~JuegoCliente() {
	miPersonajeSprites->~Sprite();
}


