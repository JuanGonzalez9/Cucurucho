#include "Bullet.h"

Bullet::Bullet(int x,int y,int xVel,int yVel)
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

Bullet::~Bullet(){

}
