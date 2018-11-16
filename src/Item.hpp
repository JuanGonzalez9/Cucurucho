#ifndef ITEM_H_
#define ITEM_H_
#include "Enemigo.h"

class Item : Enemigo{

public:
	Item(int x,int y, int coorX, int coorY, int arma, int nivel);
	virtual ~Item();

	//GETTER SETTER
	int darContenido();

	//Metodos
	void actualizarItem();
	bool puedoDibujar();

	//acciones

protected:
	int contenido;
};

#endif /* ITEM_H_ */