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
	int getPosPersonajeX2();
	int getPosPersonajeY2();
	bool estaSaltando2();
	bool estaDisparando2();
	bool estaMirandoALaDerecha2();
	bool estaActivo2();
	bool estaGrisado2();
	Constantes::Estado getEstado();
	Constantes::DireccionDisparo getDireccionDisparo();
	Constantes::Estado getEstado2();
	Constantes::DireccionDisparo getDireccionDisparo2();
	vector< pair<int,int> > getBalas();

	//..........METODO........
	void parsear(string msj);
	void parsearPersonaje1(string msj,int i);
	void parsearPersonaje2(string msj,int i);


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

	int posPersonajeX2;
	int posPersonajeY2;
	bool saltando2;
	bool disparando2;
	bool mirandoALaDerecha2;
	bool activo2;
	bool grisado2;
	Constantes::Estado estado2;
	Constantes::DireccionDisparo direccionDisparo2;


	int cantBalas;
	vector< pair<int,int> > balas;

	void parsearPosY(string substr);
	void parsearPosY2(string substr);
	void parsearBalas(string sub);
	int dameElInt(string sub);
	void resetearBalas();

};


#endif /* PARSER_H_ */
