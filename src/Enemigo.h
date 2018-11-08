#ifndef ENEMIGO_H_
#define ENEMIGO_H_
#include "EntidadDibujable.h"

class Enemigo : public EntidadDibujable{
public:
	Enemigo(int x,int y,int vidas);
	virtual ~Enemigo();

	//GETTER SETTER
	void perderVida();
	bool derrotado();
	//Metodos
	bool pasaBorde(int nivel);
	void empujarAtras(int i, int nivel);

private:
	int vidas;
	int coordenadaX;
	int coordenadaY;
	int posX;
	int posY;
};

#endif /* ENEMIGO_H_ */
