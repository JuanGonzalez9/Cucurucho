/*
 * Enemigo.h
 *
 *  Created on: Sep 22, 2018
 *      Author: juan
 */

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
private:
	int vidas;
};

#endif /* ENEMIGO_H_ */
