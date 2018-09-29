#include "Bullet.h"

Bullet::Bullet(int x,int y,int xVel,int yVel,Personaje::Estado estadoPersonaje)
{
	EntidadDibujable();
	rectOrigen.x=0;
	rectOrigen.y=0;
	rectOrigen.w=32;
	rectOrigen.h=32;
	rectDestino.x=x;
	rectDestino.y=y+24;
	rectDestino.w=8;
	rectDestino.h=8;
	xvel=xVel;
	yvel=yVel;
    duracion=60;
     //duracion=bulletRange;

    ajustarPorEstado(estadoPersonaje);
}


int Bullet::posY()
{
	return rectDestino.y;
}

int Bullet::getDuracion()
{
    return duracion;
}

int Bullet::posX()
{
	return rectDestino.x;
}


void Bullet::move()
{
	rectDestino.x+=xvel;
	rectDestino.y+=yvel;
	duracion--;
}

void Bullet::dibujar(SDL_Renderer* renderer){
	SDL_RenderCopy(renderer, textura, & rectOrigen , &rectDestino);
}

void Bullet::ajustarPorEstado(Personaje::Estado estado){

	if(estado == Personaje::Estado::CuerpoATierra){
		if(xvel > 0){
			rectDestino.x += 40;
		}

		rectDestino.y += 30;
	}

	if(estado == Personaje::Estado::Caminando){
		if(xvel < 0){
			rectDestino.x -= 10;
		}
		else{
		rectDestino.x += 10;
		}
	}

	//si esta apuntando para la derecha
	if(xvel > 0){
		//si esta apuntando para abajo
		if(yvel > 0){
			rectDestino.y -= 5;
			rectDestino.x += 10;
		}

		//si esta apuntando para arriba
		if(yvel < 0){
			rectDestino.y -= 12;
			rectDestino.x += 14;
		}

		//si apunta dereche
		if(yvel == 0)
			rectDestino.x += 15;
	}

	//si esta apuntando para la izquierda
	if(xvel < 0){
		//si esta apuntando para abajo
		if(yvel > 0){
			rectDestino.y += 10;
		}

		//si esta apuntando para arriba
		if(yvel < 0){
			rectDestino.y -= 20;
		}

	}

}

Bullet::~Bullet(){

}
