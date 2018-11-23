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

	enum Arma{
		Pistola,
		Ametralladora,
		Escopeta,
		Bazooka
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
	int obtenerCoordenadaX();
	int obtenerCoordenadaY();
	int obtenerVelocidadY();
	int obtenerVelocidadX();
	int obtenerVidas();
	bool esActivo();
	bool esGrisado();
	bool enJuego();
	int obtenerPuntaje();
	void subirPuntaje(int score);
	void resetearPuntaje();
	void agregarGris(const char* path,SDL_Renderer* renderer);

	//----METODOS

	//ESTADOS
	void activar();
	void desactivar();
	void grisar();
	void desgrisar(Plataformas plataformas, int nivel);

	//ANIMACIONES
	void saltar();
	void avanzar();
	void retroceder();
	void agacharse();
	void pararse();
	void apuntarArriba();
	void apuntarAbajo();
	void dejarDeApuntar();
	void pelarElChumbo();
	void dejarDeDisparar();
	void caer();
	void aterrizar();
	void hacerComoQueCamina();
	
	//DISPAROS
	void disparar(int dirX, int dirY,SDL_Texture *text);
	void cambiarArma(int nroArma);
	void verSiBalasPegan(Enemigo* malo);
	void dibujarBalas(SDL_Renderer* renderer);
	bool puedeDisparar();
	void refreshBullets();

	//MANEJO DE VIDAS
	int getInvincibilityFrames();
	void perderVida();
	bool muerto();
	void refreshIFrames();
	void godmodeSwitch();
	void setVidas(int vidas);

	//OTROS
	void subirCoordenadaXEn(int cantidad);
	void subirCoordenadaYEn(int cantidad);
	void actualizarPos(int,int  nivel);
	void decrementarPosY(int y);
	void decrementarPosX(int x);
	void bajar();
	void actualizar();
	void resetearPosicion(Plataformas plataformas, int nivel);
	void dibujar(SDL_Renderer* renderer);
	void subirPosX();


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

	int puntaje;

	Personaje::Arma arma;
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
