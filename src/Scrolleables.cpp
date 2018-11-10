#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include "Scrolleables.hpp"
#include "cfg.hpp"

using namespace std;


Scrolleables::Scrolleables(){

}

void Scrolleables::agregarEnemigo(const char* path,SDL_Renderer* renderer, int x, int y, int life, int coor, int lv){
	//crea y agrega un enemigo al vector de enemigos

	Enemigo* nuevoEnemigo;
	nuevoEnemigo = new Enemigo(x,y,life,coor,lv);
	nuevoEnemigo->obtenerTextura(path, renderer);
	vectorEnemigos.push_back(nuevoEnemigo);
}


void Scrolleables::empujarTodos(int distancia, int lv){
	for(int i = 0; i < vectorEnemigos.size(); i++){
		vectorEnemigos[i]->empujarAtras(distancia, lv);
	}
}


void Scrolleables::personajeTocaEoB(Personaje jugador){
	//verifica si alguno de los enemigos (luego pongo balas) toca un personaje
	//si tocan, el personaje pierde vida si no es invencible.
	//cuando ponga las balas, se van a borrar.

	for(int i = 0; i < vectorEnemigos.size(); i++){
		if(!vectorEnemigos[i]->derrotado() && collision(jugador.getRectaDestino(),vectorEnemigos[i]->getRectaDestino())){
			if(jugador.getInvincibilityFrames() == 0)
				jugador.perderVida();
		}
	}

}

void Scrolleables::BalasDeJugadorPegan(Personaje jugador){
	//ve para cada enemigo si alguna de las balas del jugador le pegan
	//lo borra si pierde su ultima vida

	int i=0;
	while (i<vectorEnemigos.size()){
		if(vectorEnemigos[i]->esActivo()){	
			jugador.verSiBalasPegan(vectorEnemigos[i]);
			if (vectorEnemigos[i]->derrotado())
				vectorEnemigos.erase(vectorEnemigos.begin() + i);
			else i++;
		} else i++;
	}
}

void Scrolleables::actualizarEnemigos(int nivel, int coor){
	//actualiza todos los enemigos: su posicion, comportamiento, estado, etc
	//los activa si estan en el nivel y coordenada correcta
	//llama la funcion que borra los expirados

	for(int i = 0; i < vectorEnemigos.size(); i++){
		vectorEnemigos[i]->actualizar(nivel, coor);
	}
	this->sacarExpirados(nivel);
}

void Scrolleables::dibujarEnemigos(SDL_Renderer* renderer){
	for(int i = 0; i < vectorEnemigos.size(); i++){
		vectorEnemigos[i]->dibujar(renderer);
	}
}


void Scrolleables::sacarExpirados(int lv){
	//borra los enemigos y balas que superan el borde o estan derrotados

	int i=0;
	while (i<vectorEnemigos.size()){
		if ((vectorEnemigos[i]->pasaBorde(lv))||(vectorEnemigos[i]->derrotado()))
			vectorEnemigos.erase(vectorEnemigos.begin() + i);
		else i++;
	}
}


Scrolleables::~Scrolleables(){
	//libera el vector de enemigos

	int size= vectorEnemigos.size();
	for(unsigned i = 0;i < size;i++){
		vectorEnemigos[i]->~Enemigo();
	}

}

bool Scrolleables::collision(SDL_Rect rect1,SDL_Rect rect2){
	if(rect1.y >= rect2.y + rect2.h)
		return false;
	if(rect1.x >= rect2.x + rect2.w)
		return false;
	if(rect1.y + rect1.h <= rect2.y)
		return false;
	if(rect1.x + rect1.w <= rect2.x)
		return false;
	return true;
}
