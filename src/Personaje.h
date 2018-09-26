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
#include "plataformas.hpp"

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
		CuerpoATierra,
		HaciendoComoQueCamina
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
	void setPosX(int x);
	void setPosY(int y);
	void setCoordenadaX(int x);
	void setCoordenadaY(int y);
	bool estaMirandoALaDerecha();
	bool estaSaltando();

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

	void caer();
	void aterrizar();
	void subirCoordenadaXEn(int cantidad);
	void subirCoordenadaYEn(int cantidad);
	int obtenerCoordenadaX();
	int obtenerCoordenadaY();
	int obtenerVelocidadY();
	int obtenerVelocidadX();
	void actualizarPos(int,int  nivel);
	void decrementarPosY(int y);
	void bajar();
	void flotar();
	void hacerComoQueCamina();
	
//<<<<<<< HEAD
	
	void subirPosX();
//=======
	//para tirotear
	int getInvincibilityFrames();
	void perderVida();
	bool muerto();
	void refreshIFrames();
//>>>>>>> 6100df544e34c0d07915ef9e808d20fe90d8f406

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

	int coordenadaX;
	int coordenadaY;

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
