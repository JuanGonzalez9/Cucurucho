#include <SDL2/SDL.h>
#include <iostream>
#include "EntidadDibujable.h"
#include "Personaje.h"
#include "Utils.h"
#include "Constantes.h"

#ifndef BULLET_H
#define BULLET_H

class Bullet : public EntidadDibujable{

private:
	int xvel, yvel, duracion;
	Constantes::TipoArma tipo;

public:

	Bullet(int x,int y,int xVel, int yVel,int tamanio,int last);
	Bullet(int x,int y);
	virtual ~Bullet();
	int posY();
	int posX();
	void move();
	int getDuracion();
	void dibujar(SDL_Renderer* renderer);
	void empujarAtras(int i, int nivel);
	string serializar();
	void setTipoBala(Constantes::TipoArma t);
	

};

#endif
