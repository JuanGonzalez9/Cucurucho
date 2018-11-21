#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "Constantes.h"
#include <vector>
#include "Bullet.h"
#include "DatosPersonaje.h"
#include "DatosEnemigo.h"
#include "DatosItem.h"
#include "DatosBalaEnemiga.h"

using namespace std;

class Parser {
public:
	Parser();

	//..........GET SET.......
	int getNivel();
	int getPosFondo1();
	int getPosFondo2();
	int getPosFondo3();
	int getPosPersonajeX(int numeroDePersonaje);
	int getPosPersonajeY(int numeroDePersonaje);
	bool estaSaltando(int numeroDePersonaje);
	bool estaDisparando(int numeroDePersonajeint);
	bool estaMirandoALaDerecha(int numeroDePersonaje);
	bool estaActivo(int numeroDePersonaje);
	bool estaGrisado(int numeroDePersonaje);
	Constantes::Estado getEstado(int numeroDePersonaje);
	Constantes::DireccionDisparo getDireccionDisparo(int numeroDePersonaje);
	bool estaElEnemigo();
	
	vector<DatosEnemigo*> getEnemigos();
	vector<DatosItem*> getItems();
	vector<DatosBalaEnemiga*> getBalasEnemigas();

	vector< pair<int,int> > getBalas();

	//..........METODO........
	void parsear(string msj,int jugadores);

	virtual ~Parser();
private:
	int cantJugadores;
	int nivel;
	int posFondo1;
	int posFondo2;
	int posFondo3;

	DatosPersonaje datosBoby,datosBoby2,datosBoby3,datosBoby4;
	
	int cantBalas;
	vector< pair<int,int> > balas;
	bool hayEnemigo;

	int parsearPosY(string substr);
	void parsearBalas(string sub);
	int dameElInt(string sub);
	void resetearBalas();
	void parsearPersonaje(string msj,int i,int numeroDePersonaje);
	DatosPersonaje* dameAlBobyNumero(int numeroDePersonaje);

	vector<DatosEnemigo*> vEnemigos;
	void parsearEnemigos(string msj);

	vector<DatosItem*> vItems;
	void parsearItems(string msj);

	vector<DatosBalaEnemiga*> vBalasEnemigas;
	void parsearBalasEnemigas(string msj);

};


#endif /* PARSER_H_ */
