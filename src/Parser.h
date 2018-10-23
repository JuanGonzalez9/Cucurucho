#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Constantes.h"

using namespace std;

class Parser {
public:
	Parser();

	//..........GET SET.......
	int getNivel();
	int getPosFondo1();
	int getPosFondo2();
	int getPosFondo3();
	int getPosPersonajeX();
	int getPosPersonajeY();
	bool estaSaltando();
	bool estaDisparando();
	bool estaMirandoALaDerecha();
	bool estaActivo();
	bool estaGrisado();
	Constantes::Estado getEstado();
	Constantes::DireccionDisparo getDireccionDisparo();

	//..........METODO........
	void parsear(string msj);

	virtual ~Parser();
private:
	int nivel;
	int posFondo1;
	int posFondo2;
	int posFondo3;
	int posPersonajeX;
	int posPersonajeY;
	bool saltando;
	bool disparando;
	bool mirandoALaDerecha;
	bool activo;
	bool grisado;
	Constantes::Estado estado;
	Constantes::DireccionDisparo direccionDisparo;

	void parsearPosY(string substr);

};


#endif /* PARSER_H_ */
