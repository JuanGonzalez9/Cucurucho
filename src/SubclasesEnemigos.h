#ifndef SUBCLASESENEMIGOS_H_
#define SUBCLASESENEMIGOS_H_
#include "Enemigo.h"
#include "Utils.h"
#include "Constantes.h"
class juego;

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
	string serializar();
	void setMarcaPasos(int valor);

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
	string serializar();
	void setMarcaPasos(int valor);

protected:

};

class Turret : public Enemigo{

public: 
	Turret(int x,int y, int coorX, int coordenada, int nivel);
	~Turret();

	void hazLoTuyo();
	void disparo();
	int disparoYVel();
	int disparoXVel();
	string serializar();
	void setMarcaPasos(int valor);

protected:
	int apunteX;
	int apunteY;

};

class Paracaidas : public Enemigo{

public: 
	Paracaidas(int x,int y, int coorX, int coordenada, int nivel);
	~Paracaidas();

	void hazLoTuyo();
	void disparo();
	int disparoYVel();
	int disparoXVel();
	string serializar();
	void setMarcaPasos(int valor);

protected:

};

//////////////JEFES/////////////////////////


class MonstruoFinal : public Enemigo{
public:
	MonstruoFinal(juego* j, int x,int y, int coorX, int coordenada, int nivel);
	virtual ~MonstruoFinal();

	void activar(int nivel, int coordenada, int pos);
	void explotar();
	void dispararBalasFuego(int coordenadaX,int coordenadaY, int level);
	void dispararBalasFuego2(int coordenadaX,int coordenadaY, int level);
	void dispararMisiles(int coordenadaX,int coordenadaY, int level);
	void tenerInmunidad();
	void tenerInvisibilidad();
	void setMarcaPasos(int valor);
	
	int obtenerPuntaje();

protected:
	juego* j;
	typedef enum {comenzo,disparando_fuego,disparando_fuego2,disparando_misiles,inmunidad,invisibilidad,explosion}Estado;
	int contador_marcos;
	Estado estado;

};

class MonstruoFinalNivel1 : public MonstruoFinal{
public:
	MonstruoFinalNivel1(juego* j,int x, int y, int life);
	string serializar();
	virtual ~MonstruoFinalNivel1();
	void posicionar();
	void hazLoTuyo();
	void setMarcaPasos(int valor);	

protected:

};

class MonstruoFinalNivel2 : public MonstruoFinal{
public:
	MonstruoFinalNivel2(juego* j,int x, int y, int life);
	string serializar();
	virtual ~MonstruoFinalNivel2();

	void hazLoTuyo();
	void setMarcaPasos(int valor);	

protected:

};

class MonstruoFinalNivel3 : public MonstruoFinal{
public:
	MonstruoFinalNivel3(juego* j,int x, int y, int life);
	string serializar();
	virtual ~MonstruoFinalNivel3();

	void hazLoTuyo();
	void setMarcaPasos(int valor);	

protected:

};





#endif /* ENEMIGO_H_ */
