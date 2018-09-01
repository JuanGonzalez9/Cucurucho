/*
 * Juego.cpp
 *
 *  Created on: Sep 1, 2018
 *      Author: juan
 */

#include "Juego.h"

using namespace std;

Juego::Juego() {
	ventana = NULL;
	renderer = NULL;
	estaJugando = false;
}

void Juego::inicializar(const char* titulo,int posX,int posY,int ancho,int alto){
	estaJugando = true;

	int inicializado = SDL_Init(SDL_INIT_EVERYTHING);

	if(inicializado == sdlInicializado){
		cout<<"SDL inicializado"<<endl;
		//TODO pasar esto al LOG
	}
	else{
		cout<<"Error, no se pudo inicializar SDL";
		//TODO error del LOG
		estaJugando = false;
		return;
	}

	//Para pantalla completa poner SDL_WINDOW_FULLSCREAN en vez del 0
	ventana = SDL_CreateWindow(titulo,posX,posY,ancho,alto,0);
	if(ventana == NULL){
		cout<<"Error, no se pudo crear la ventana"<<endl;
		//TODO LOG
		estaJugando = false;
		return;
	}
	else{
		cout<<"Se creo la ventana"<<endl;
		//TODO log
	}

	renderer = SDL_CreateRenderer(ventana,-1,0);
	if(renderer == NULL){
		cout<<"Error, no se pudo crear el renderer"<<endl;
		//TODO ponerlo con el log
		estaJugando = false;
		return;
	}
	else{
		cout<<"Renderer creado"<<endl;
		// elegir el color con RGBA
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
	}

}

void Juego::actualizar(){

}

void Juego::renderizar(){
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

bool Juego::jugando(){
	return estaJugando;
}

void Juego::manejarEventos(){
	SDL_Event evento;
	SDL_PollEvent(&evento);

	switch(evento.type){

		case (SDL_QUIT):
			estaJugando = false;
			break;
		default:
			break;
	}

}

void Juego::limpiar(){
	SDL_DestroyWindow(ventana);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout<<"Ventana y renderer destruidas,ggwp";
	//TODO poner en el log
}



Juego::~Juego() {
	// TODO Auto-generated destructor stub
}
