#ifndef ENEMIGO_H_
#define ENEMIGO_H_
#include "EntidadDibujable.h"

class Enemigo : public EntidadDibujable{
public:
	Enemigo(int x,int y,int life, int coordenada, int nivel, int tipo);
	Enemigo(int x,int y,int vidas);
	virtual ~Enemigo();

	//GETTER SETTER
	void perderVida();
	bool derrotado();
	bool esActivo();
	int obtenerCoordenadaActiva();
	int obtenerNivelActivo();
	//Metodos

	//activacion
	bool pasaBorde(int nivel);
	void empujarAtras(int i, int nivel);
	void activar(int nivel, int coordenada, int pos);
	void actualizar(int nivel, int coordenada);
	void hazLoTuyo();

	//acciones
	void moverIzquierda(int x);
	void moverDerecha(int x);

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
	int tipoEnemigo;
};

#endif /* ENEMIGO_H_ */
