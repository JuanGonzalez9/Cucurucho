/*
 * Juego.cpp
 *
 *  Created on: Sep 1, 2018
 *      Author: juan
 */

#include "Juego.h"

using namespace std;

SDL_Texture* texturaJugador;
SDL_Texture* texturaMapa;

SDL_Rect rectaDestino;
SDL_Rect sprites[5];

SDL_Rect rectMapaDestino;
SDL_Rect rectMapaOrigen;

int posx = 0;
int cont = 0;

Juego::Juego() {
	ventana = NULL;
	renderer = NULL;
	estaJugando = false;
	sdlInicializado = 0;
	altoVentana = 0;
	anchoVentana = 0;

	sprites[0].x = 145;
	sprites[0].y = 132;
	sprites[0].h = 38;
	sprites[0].w = 23;

	sprites[1].x = 168;
	sprites[1].y = 132;
	sprites[1].h = 38;
	sprites[1].w = 23;

	sprites[2].x = 191;
	sprites[2].y = 132;
	sprites[2].h = 38;
	sprites[2].w = 23;

	sprites[3].x = 214;
	sprites[3].y = 132;
	sprites[3].h = 38;
	sprites[3].w = 23;

	sprites[4].x = 237;
	sprites[4].y = 132;
	sprites[4].h = 38;
	sprites[4].w = 23;

	rectaDestino.x = 0;
	rectaDestino.y = 280;
	rectaDestino.h = 64;
	rectaDestino.w = 32;

	//rectangulo para el mapa
	//Por ser imagen de fondo no necesita recta destino, el valor para el render_copy es NULL

	rectMapaOrigen.x = 0;
	rectMapaOrigen.y = 0;
	rectMapaOrigen.w = 327;
	rectMapaOrigen.h = 220;

}

void Juego::inicializar(const char* titulo,int posX,int posY,int ancho,int alto){
	estaJugando = true;
	anchoVentana = ancho;
	altoVentana = alto;

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


	SDL_Surface* mapaTemp = IMG_Load("imagenes/nivel.png");
	texturaMapa = SDL_CreateTextureFromSurface(renderer,mapaTemp);
	SDL_FreeSurface(mapaTemp);

	SDL_Surface* temp = IMG_Load("imagenes/ContraPersonaje.gif");
	texturaJugador = SDL_CreateTextureFromSurface(renderer,temp);
	SDL_FreeSurface(temp);


}

void Juego::actualizar(){
	cont++;
	cont %= 4;

}

void Juego::jugadorAvanzar(){

	posx += 4;
	rectaDestino.x = posx;
}

void Juego::jugadorRetroceder(){

	posx -= 4;
	rectaDestino.x = posx;
}

void Juego::mapaScroll(){
	rectMapaOrigen.x += 2;
}

void Juego::renderizar(){
	SDL_RenderClear(renderer);
	//el rectangulo destino lo dejo en NULL para que ocupe toda la pantalla
	SDL_RenderCopy(renderer,texturaMapa,&rectMapaOrigen,NULL);
	SDL_RenderCopy(renderer,texturaJugador,&sprites[cont],&rectaDestino);
	SDL_RenderPresent(renderer);
}

bool Juego::jugando(){
	return estaJugando;
}

void Juego::manejarEventos(){

	SDL_Event evento;
	while(SDL_PollEvent(&evento)){
		switch(evento.type){

			case (SDL_QUIT):
				estaJugando = false;
				break;

			case (SDL_KEYDOWN):
					if(evento.key.keysym.sym == SDLK_RIGHT){
						if(rectaDestino.x <= (anchoVentana / 2)){
							jugadorAvanzar();
						}
						else mapaScroll();
					}
					if(evento.key.keysym.sym == SDLK_LEFT){
						jugadorRetroceder();
					}
					break;
			default:
			break;
		}
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
