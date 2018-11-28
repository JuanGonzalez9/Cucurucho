#ifndef ITEM_H_
#define ITEM_H_
#include "Enemigo.h"
#include "Utils.h"
#include "Constantes.h"

class Item : public Enemigo{

public:
	Item(int x,int y, int coorX, int coorY, int arma, int nivel);
	virtual ~Item();

	//GETTER SETTER
	int darContenido();

	//Metodos
	void actualizarItem();
	bool puedoDibujar();
	string serializar();
	void setMarcaPasos(int valor);

	//acciones

protected:
	int contenido;
};

#endif /* ITEM_H_ */
