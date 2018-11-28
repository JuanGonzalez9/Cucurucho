#ifndef ALIEN_HPP
#define ALIEN_HPP

#include <string>
#include "Enemigo.h"

class juego;

class alien: public Enemigo
{
public:
	alien(juego* j, int x,int y, int vidas, int coordenada, int nivel);
	virtual ~alien();

	typedef enum {caminando, reptando, saltando, cayendo} estado;
	
	void hazLoTuyo();
	void cambiar_estado(estado e);
	void actualizar();
	void actualizar_caminando();
	void actualizar_saltando();
	void actualizar_cayendo();
	void actualizar_reptando();
	std::string serializar();
protected:
	juego *j;
	int contador, marco, tmp, sentido;
	float velocidad, altura;
	estado e;
};

#endif
