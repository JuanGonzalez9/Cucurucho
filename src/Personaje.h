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
		CuerpoATierra
	};

	enum DireccionDisparo{
		Arriba,
		Centro,
		Abajo
	};

	//----GET--SET
	Personaje::Estado getEstado();
	int getPosX();
	int getPosY();
	bool estaMirandoALaDerecha();

	//----METODOS
	void saltar();
	void avanzar();
	void retroceder();
	void agacharse();
	void disparar();
	void pelarElChumbo();
	void actualizar();
	void dejarDeDisparar();
	void apuntarArriba();
	void apuntarAbajo();
	//para que la posicion del arma vuelva al centro
	void dejarDeApuntar();
	bool puedeDisparar();
	void refreshBullets();
	void dibujar(SDL_Renderer* renderer);
	//para tirotear
	int getInvincibilityFrames();
	void perderVida();
	bool muerto();
	void refreshIFrames();

private:
	Sprite* spritesJugador;
	int maximaVelocidadX;
	int maximaVelocidadY;
	int velocidadY;
	int velocidadX;
	int gravedad;
	int posX;
	int posY;
	int shootTimer;

	// manejo de vida
	int hitPoints;
	int invincibilityFrames;

	Personaje::Estado estado;
	Personaje::DireccionDisparo direccionDisparo;
	//el estado de saltando o no se guarda en este booleano para no mezclar
	bool saltando;
	bool disparando;
	bool mirandoALaDerecha;
};

#endif /* PERSONAJE_H_ */
