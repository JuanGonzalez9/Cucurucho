#include <SDL2/SDL.h>
#include <iostream>
#include "EntidadDibujable.h"
#include "Personaje.h"

#ifndef BULLET_H
#define BULLET_H

class Bullet : public EntidadDibujable{

private:
	int xvel, yvel, duracion;

public:

	Bullet(int x,int y,int xVel, int yVel);
	Bullet(int x,int y);
	virtual ~Bullet();
	int posY();
	int posX();
	void move();
	int getDuracion();
	void dibujar(SDL_Renderer* renderer);

};

#endif
