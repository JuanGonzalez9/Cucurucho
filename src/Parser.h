#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Constantes.h"
#include <vector>
#include "Bullet.h"

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
	vector< pair<int,int> > getBalas();

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
	int cantBalas;
	vector< pair<int,int> > balas;

	void parsearPosY(string substr);
	void parsearBalas(string sub);
	int dameElInt(string sub);
	void resetearBalas();

};


#endif /* PARSER_H_ */
