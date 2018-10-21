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

	//.......metodos...........................
	string dameLaRespuesta();

	virtual ~ArmadorDeRespuesta();

private:
	string fondoToString(int f);
	string posicionToString(int p);
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

	Constantes::Estado estado;
	Constantes::DireccionDisparo dirDisparo;
};

#endif /* ARMADORDERESPUESTA_H_ */
