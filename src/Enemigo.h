#ifndef ENEMIGO_H_
#define ENEMIGO_H_
#include "EntidadDibujable.h"

class Enemigo : public EntidadDibujable{
public:
	//constructor para juego
	Enemigo(int x,int y,int life, int coordenada, int nivel);
	//constructor viejo para jefes
	Enemigo(int x,int y,int vidas);
	//default para los que heredan
	Enemigo();
	virtual ~Enemigo();

	//GETTER SETTER
	void perderVida();
	bool derrotado();
	bool esActivo();
	int obtenerCoordenadaActiva();
	int obtenerNivelActivo();

	int coordenadaXParaItem();
	int coordenadaYParaItem();
	int posicionXParaItem();
	int posicionYParaItem();

	int invisible;

	//Metodos

	//activacion
	bool pasaBorde(int nivel);
	void empujarAtras(int i, int nivel);
	virtual void activar(int nivel, int coordenada, int pos);
	void actualizar(int nivel, int coordenada);
	virtual void hazLoTuyo();

	//acciones
	void moverIzquierda(int x);
	void moverDerecha(int x);
	void moverArriba(int y);
	void moverAbajo(int y);

	bool quiereDisparar();
	/*
	virtual int disparoXVel() = 0;
	virtual int disparoYVel() = 0;
	virtual void disparo() = 0;
	*/
	virtual int disparoXVel();
	virtual int disparoYVel();
	virtual void disparo();
	
	void bajarShootTimer();

	virtual string serializar();
	virtual void setMarcaPasos(int valor);
	bool estoyEnEscena(int x,int y,int w,int h);
	virtual int obtenerPuntaje();
	bool visible;

protected:
	int coordenadaX;
	int coordenadaY;
	int posX;
	int posY;

	bool activo;
	int vidas;
	int nivelActivo;
	int coordenadaActiva;

	int marcapasos;
	int shootTimer;
	bool quieroDisparar;

	int inmune;

};

#endif /* ENEMIGO_H_ */
