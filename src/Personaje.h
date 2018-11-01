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
#include "Bullet.h"
#include "Enemigo.h"
#include "iocontra.hpp"
#include "Constantes.h"

class Personaje : public EntidadDibujable{

public:
	Personaje();
	virtual ~Personaje();

	//Si se necesita otro estado se puede agregar aca
	enum Estado{
		Quieto,
		Caminando,
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
	Personaje::DireccionDisparo getDireccionDisparo();
	int getPosX();
	int getPosY();
	int getCantidadDeBalas();
	vector<Bullet*> getBalas();
	void setBalas(vector<Bullet*> balas);
	void setPosX(int x);
	void setPosY(int y);
	void setCoordenadaX(int x);
	void setCoordenadaY(int y);
	void setEstados(bool salto,bool disp,bool mirando,bool act,bool gris);
	void setEstadosEnumerados(Constantes::Estado est,Constantes::DireccionDisparo dir);
	bool estaMirandoALaDerecha();
	bool estaSaltando();
	bool estaDisparando();
	bool estaCercaDelFinalDelNivel1();
	bool estaCercaDelFinalDelNivel2();
	bool estaCercaDelFinalDelNivel3();
	bool llegoAlFinalDelNivel1();
	bool llegoAlFinalDelNivel2();
	bool llegoAlFinalDelNivel3();
	bool esActivo();
	bool esGrisado();
	void agregarGris(const char* path,SDL_Renderer* renderer);

	//----METODOS
	void activar();
	void desactivar();
	void grisar();
	void desgrisar();
	void saltar();
	void avanzar();
	void retroceder();
	void agacharse();
	void pararse();
	void disparar(int dirX, int dirY,SDL_Texture *text);
	void verSiBalasPegan(Enemigo* malo);
	void dibujarBalas(SDL_Renderer* renderer);
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
	void decrementarPosX(int x);
	void bajar();
	void resetFall();
	void hacerComoQueCamina();
	
	void subirPosX();

	//para tirotear
	int getInvincibilityFrames();
	void perderVida();
	bool muerto();
	void refreshIFrames();
	void godmodeSwitch();

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
	vector<Bullet*> bullets;
	int coordenadaX;
	int coordenadaY;

	void actualizarRectDestino();

	// manejo de vida
	int hitPoints;
	int invincibilityFrames;

	Personaje::Estado estado;
	Personaje::DireccionDisparo direccionDisparo;
	//el estado de saltando o no se guarda en este booleano para no mezclar
	bool saltando;
	bool disparando;
	bool activo;
	bool grisado;
	bool godmode;
	bool mirandoALaDerecha;
	SDL_Texture* tex2;
	void swapTextures();
};

#endif /* PERSONAJE_H_ */
