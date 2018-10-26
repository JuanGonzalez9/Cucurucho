/*
 * ArmadorDeRespuesta.h
 *
 *  Created on: Oct 20, 2018
 *      Author: juan
 */

#ifndef ARMADORDERESPUESTA_H_
#define ARMADORDERESPUESTA_H_
#include <unistd.h>
#include <iostream>
#include "Constantes.h"
#include <vector>
#include "Bullet.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "DatosPersonaje.h"

using namespace std;

class ArmadorDeRespuesta {
public:
	ArmadorDeRespuesta();

	//........seters...........................
	void setNivel(int nivel);
	void setFondo1(int x,int y);
	void setFondo2(int x,int y);
	void setFondo3(int x,int y);

	void setPosPersonaje(int x,int y);
	void setSaltando(bool saltando);
	void setDisparando(bool disp);
	void setMirandoALaDerecha(bool mirando);
	void setActivo(bool active);
	void setGrisado(bool grisado);
	void setEstado(Constantes::Estado estado);
	void setDireccionDisparo(Constantes::DireccionDisparo dir);
	

	/*void setPosPersonaje2(int x,int y);
	void setSaltando2(bool saltando);
	void setDisparando2(bool disp);
	void setMirandoALaDerecha2(bool mirando);
	void setActivo2(bool active);
	void setGrisado2(bool grisado);
	void setEstado2(Constantes::Estado estado);
	void setDireccionDisparo2(Constantes::DireccionDisparo dir);*/

	
	void setCantidadDeBalas(int cant);
	void sumarBalas(vector<Bullet*> nuevasBalas);
	//.......metodos...........................
	/*string dameLaRespuesta();*/
	string dameLaRespuestaPara(int jugadores, DatosPersonaje* pj1, DatosPersonaje* pj2, DatosPersonaje* pj3, DatosPersonaje* pj4);


	virtual ~ArmadorDeRespuesta();

private:
	string strBalas;


	string resetBalas;



	string fondoToString(int f);
	string posicionToString(int p);
	string pasarAStringDeTamanio(int tamanio,int valor);
	int nivel;
	int fondo1;
	int fondo2;
	int fondo3;
	//posiciones relativas a la ventana
	int posPersonajeX;
	int posPersonajeY;
	bool saltando;
	bool disparando;
	bool mirandoALaDerecha;
	bool activo;
	bool grisado;
	int cantidadDeBalas;
	int balasActuales;
	//posicion del otro jugador
	/*int posPersonajeX2;
	int posPersonajeY2;
	bool saltando2;
	bool disparando2;
	bool mirandoALaDerecha2;
	bool activo2;
	bool grisado2;
	int cantidadDeBalas2;
	int balasActuales2;*/


	Constantes::Estado estado;
	Constantes::DireccionDisparo dirDisparo;

	/*Constantes::Estado estado2;
	Constantes::DireccionDisparo dirDisparo2;*/
};

#endif /* ARMADORDERESPUESTA_H_ */
