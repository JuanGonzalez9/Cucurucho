/*
 * Personaje.h
 *
 *  Created on: Sep 18, 2018
 *      Author: juan
 */

#ifndef PERSONAJE_H_
#define PERSONAJE_H_

#include "EntidadDibujable.h"
#include "Sprite.h"

class Personaje : public EntidadDibujable{

public:
	Personaje();
	virtual ~Personaje();

	//Si se necesita otro estado se puede agregar aca
	enum Estado{
		Quieto,
		Caminando,
		Retrocediendo,
		CaminandoYDisparando,
		Disparando,
		CuerpoATierra
	};

	//----GET--SET
	Personaje::Estado getEstado();
	int getPosX();
	int getPosY();

	//----METODOS
	void saltar();
	void avanzar();
	void retroceder();
	void agacharse();
	void actualizar();
	void dibujar(SDL_Renderer* renderer);

private:
	Sprite* spritesJugador;
	int maximaVelocidadX;
	int maximaVelocidadY;
	int velocidadY;
	int velocidadX;
	int gravedad;
	int posX;
	int posY;

	Personaje::Estado estado;
	//el estado de saltando o no se guarda en este booleano para no mezclar
	bool saltando;
};

#endif /* PERSONAJE_H_ */
