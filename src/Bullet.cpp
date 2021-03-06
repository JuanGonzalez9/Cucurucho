#include "Bullet.h"

Bullet::Bullet(int x,int y,int xVel,int yVel,int tamanio,int last):EntidadDibujable()
{
	
	rectOrigen.x=0;
	rectOrigen.y=0;
	rectOrigen.w=32;
	rectOrigen.h=32;
	rectDestino.x=x-4*(tamanio-1);
	rectDestino.y=y-4*(tamanio-1);
	rectDestino.w=8*tamanio;
	rectDestino.h=8*tamanio;
	xvel=xVel;
	yvel=yVel;
	duracion=last;
	tipo = Constantes::normal;
	//duracion=bulletRange;
}

Bullet::Bullet(int x,int y):Bullet(x,y,0,0,1,60){
	
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

void Bullet::empujarAtras(int i, int nivel){
	if (nivel ==2){
		rectDestino.y=rectDestino.y-i;
	}else{
		rectDestino.x=rectDestino.x-i;
	}
}

void Bullet::setTipoBala(Constantes::TipoArma t){
	this->tipo = t;
}

string Bullet::serializar(){
	// 1 es bala normal
	string serial = "";
	if(rectDestino.x > -300 && rectDestino.x < 10000 && rectDestino.y > -300 && rectDestino.y < 10000){
		serial = to_string(tipo);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	}
	return serial;
}

void Bullet::dibujar(SDL_Renderer* renderer){

	SDL_RenderCopy(renderer, textura, & rectOrigen , &rectDestino);
}



Bullet::~Bullet(){

}
