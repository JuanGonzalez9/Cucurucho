#ifndef SUBCLASESENEMIGOS_H_
#define SUBCLASESENEMIGOS_H_
#include "Enemigo.h"

//TUTORIAL PARA HACER ENEMIGOS NUEVOS
//La posicion desde donde se aparece y coordenada se decide en el constructor en juego.cpp
//Se sobreescriben los siguientes metodos:
//en el constructor se define el tamaño
//hazLoTuyo() define movimiento y disparo
//disparo() define el tiempo entre disparo y disparo (cambiar el int)
//disparoXVel() y disparoYVel() definen hacia donde disparan las balas


class Ovni : public Enemigo{
public:
	Ovni(int x,int y, int coorX, int coordenada, int nivel);
	~Ovni();

	void hazLoTuyo();
	void disparo();
	int disparoYVel();
	int disparoXVel();

protected:

};


class Marcianito : public Enemigo{

public: 
	Marcianito(int x,int y, int coorX, int coordenada, int nivel);
	~Marcianito();

	void hazLoTuyo();
	void disparo();
	int disparoYVel();
	int disparoXVel();

protected:

};

#endif /* ENEMIGO_H_ */