#include "SubclasesEnemigos.h"

///////////////////////////////////////OVNI//////////////////////////////////////////
//aparece desde la derecha por arriba de la pantalla y dispara para abajo

Ovni::Ovni(int x,int y, int coorX, int coordenada, int nivel) {
	Enemigo();

	// despues cambio el sprite
	rectOrigen = {0,0,40,80};
	rectDestino = {x,y,100,50};
	posX=x;
	posY=y;
	
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada;
		coordenadaActiva=coordenada-y+300;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
		coordenadaActiva=coordenada-x+400;
	}

	nivelActivo=nivel;
	activo=false;
	marcapasos=0;
}


void Ovni::hazLoTuyo(){
	//el ovni se mueve de derecha a izquierda, disparando para abajo

	moverIzquierda(2);
	if(shootTimer==0)
		quieroDisparar=true;			
}

void Ovni::disparo(){
	shootTimer=100;
	quieroDisparar=false;
}

int Ovni::disparoXVel(){
	return 0;
}

int Ovni::disparoYVel(){
	return 5;
}

Ovni::~Ovni() {
	// TODO Auto-generated destructor stub
}


///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////MARCIANITO////////////////////////////////////////////
//se mueve de izquierda a derecha. dispara a la derecha

Marcianito::Marcianito(int x,int y, int coorX, int coordenada, int nivel) {
	Enemigo();

	rectOrigen = {0,0,40,80};
	rectDestino = {x,y,50,100};
	posX=x;
	posY=y;
	
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada;
		coordenadaActiva=coordenada-y+300;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
		coordenadaActiva=coordenada-x+400;
	}

	nivelActivo=nivel;
	activo=false;
	marcapasos=0;
	shootTimer=0;
}

void Marcianito::hazLoTuyo(){
	//el marcianito se mueve de lado a lado.
	//por ahora dispara adelante pero despues lo cambio para que se de vuelta

	if(marcapasos>=60){
		moverDerecha(3);
	} else{
		moverIzquierda(3);
	}
	marcapasos++;
	if(marcapasos==120)
		marcapasos=0;

	if(shootTimer==0)
		quieroDisparar=true;	
	
}

void Marcianito::disparo(){
	shootTimer=60;
	quieroDisparar=false;
}

int Marcianito::disparoXVel(){
	return -4;
}

int Marcianito::disparoYVel(){
	return 0;
}

Marcianito::~Marcianito() {
	// TODO Auto-generated destructor stub
}