#ifndef ENEMIGO_H_
#define ENEMIGO_H_
#include "EntidadDibujable.h"

class Enemigo : public EntidadDibujable{
public:
	Enemigo(int x,int y,int life, int coordenada, int nivel);
	Enemigo(int x,int y,int vidas);
	virtual ~Enemigo();

	//GETTER SETTER
	void perderVida();
	bool derrotado();
	bool esActivo();
	//Metodos
	bool pasaBorde(int nivel);
	void empujarAtras(int i, int nivel);
	void activar(int nivel, int coordenada);
	void actualizar(int nivel, int coordenada);

protected:
	bool activo;
	int vidas;
	int coordenadaX;
	int coordenadaY;
	int posX;
	int posY;
	int nivelActivo;
	int coordenadaActiva;
};

#endif /* ENEMIGO_H_ */
