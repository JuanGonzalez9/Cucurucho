#ifndef SCROLLEABLES_HPP
#define SCROLLEABLES_HPP

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <vector>
#include "cfg.hpp"
#include "iocontra.hpp"
#include "EntidadDibujable.h"
#include "Personaje.h"
#include "Bullet.h"
#include "Enemigo.h"

using namespace std;

class Scrolleables {
public:
	
	Scrolleables();
	~Scrolleables();	

	//METODOS
	void agregarEnemigo(const char* path,SDL_Renderer* renderer, int x, int y, int life, int coor, int lv);
	void sacarExpirados(int lv);
	void empujarTodos(int distancia, int lv);
	void personajeTocaEoB(Personaje jugador);
	void BalasDeJugadorPegan(Personaje jugador);
	void actualizarEnemigos(int nivel, int coor);
	void dibujarEnemigos(SDL_Renderer* renderer);
	bool collision(SDL_Rect rect1,SDL_Rect rect2);


protected:
	//la clase cuenta con un vector que contiene los enemigos, sus balas, y los items

	vector<Bullet*> balasEnemigas;
	vector<Enemigo*> vectorEnemigos;
	//falta vector de items
};

#endif
