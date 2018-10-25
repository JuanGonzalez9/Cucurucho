#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Constantes.h"
#include <vector>
#include "Bullet.h"
#include "DatosPersonaje.h"

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

	virtual ~Parser();
private:
	int nivel;
	int posFondo1;
	int posFondo2;
	int posFondo3;

	DatosPersonaje datosBoby,datosBoby2;
	
	int cantBalas;
	vector< pair<int,int> > balas;

	int parsearPosY(string substr);
	void parsearBalas(string sub);
	int dameElInt(string sub);
	void resetearBalas();
	void parsearPersonaje(string msj,int i,int numeroDePersonaje);
	DatosPersonaje* dameAlBobyNumero(int numeroDePersonaje);

};


#endif /* PARSER_H_ */
