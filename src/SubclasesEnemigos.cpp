#include "SubclasesEnemigos.h"
#include "juego.hpp"

///////////////////////////////////////OVNI//////////////////////////////////////////
//aparece desde la derecha por arriba de la pantalla y dispara para abajo

Ovni::Ovni(int x,int y, int coorX, int coordenada, int nivel):Enemigo() {
	

	rectOrigen = {0,0,60,40};
	rectDestino = {x,y,100,80};
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

string Ovni::serializar(){
	string serial = to_string(Constantes::ovni);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}

Ovni::~Ovni() {
	// TODO Auto-generated destructor stub
}


///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////MARCIANITO////////////////////////////////////////////
//se mueve de izquierda a derecha. dispara a la derecha

Marcianito::Marcianito(int x,int y, int coorX, int coordenada, int nivel):Enemigo(){

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

string Marcianito::serializar(){
	string serial = to_string(Constantes::marcianito);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}

Marcianito::~Marcianito() {
	// TODO Auto-generated destructor stub
}




////// Monstruos Finales Nivel 1,2y3
MonstruoFinal::MonstruoFinal(juego* j,int x, int y, int life, int coordenada,int nivel):
	Enemigo(x,y,life, coordenada, nivel),
	j(j),
	contador_marcos(0),
	estado(comenzo){
	
	quieroDisparar=false;
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
}


void MonstruoFinal::dispararBalasFuego(int coordenadaX,int coordenadaY){
	Bullet * nuevaBala;
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,5,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-3,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,-3,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,0,-3,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	
	

}

void MonstruoFinal::dispararBalasFuego2(int coordenadaX,int coordenadaY){
	Bullet * nuevaBala;
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego2//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego2//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego2//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego2//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego2//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);

}

void MonstruoFinal::dispararMisiles(int coordenadaX,int coordenadaY){

	Bullet * nuevaBala;
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	j->agregarBalaEnemigo(nuevaBala);

}

void MonstruoFinal::tenerInmunidad(){
	inmune = 60; //1seg de inmunidad
}

void MonstruoFinal::tenerInvisibilidad(){
	invisible = 90; // en juego evitar que se dibuje
}

void MonstruoFinal::explotar(){

	/*SDL_RenderCopy(j->dameElRender(), obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());, &rectOrigen , &rectDestino);
	SDL_RenderCopy(j->dameElRender(), obtenerTextura("//configuracion//items//bala_//sprite", j->dameElRender());, &rectOrigen , &rectDestino);
	SDL_RenderCopy(j->dameElRender(), obtenerTextura("//configuracion//items//bala2//sprite", j->dameElRender());, &rectOrigen , &rectDestino);
	SDL_RenderCopy(j->dameElRender(), obtenerTextura("//configuracion//items//bala2//sprite", j->dameElRender());, &rectOrigen , &rectDestino);

*/}

//void MonstruoFinal::hazLoTuyo(){
//	
//}

MonstruoFinal::~MonstruoFinal(){}

void MonstruoFinalNivel1::hazLoTuyo(){

	contador_marcos++;
	if(estado==comenzo){
		estado=disparando_fuego;
		dispararBalasFuego(1000,0);
	}
	if(estado==disparando_fuego && contador_marcos>500){
		estado=disparando_fuego2;
		dispararBalasFuego2(1000,0);
	}
	if(estado==disparando_fuego2 && contador_marcos>1500){
		estado=inmunidad;
		tenerInmunidad();
	}
	if(estado==inmunidad && inmune <= 0 ){
		contador_marcos=0;
		estado=comenzo;
	}


} 

void MonstruoFinalNivel2::hazLoTuyo(){
	
}

void MonstruoFinalNivel3::hazLoTuyo(){
	
}


MonstruoFinalNivel1::MonstruoFinalNivel1(juego* j,int x, int y, int life):
MonstruoFinal( j,x,  y, life, 1000, 1 ){ }

MonstruoFinalNivel2::MonstruoFinalNivel2(juego* j,int x, int y, int life):
MonstruoFinal(j, x, y, life, 3000, 2){}

MonstruoFinalNivel3::MonstruoFinalNivel3(juego* j,int x, int y, int life):
MonstruoFinal( j, x,  y,  life , 1000 , 3){}

MonstruoFinalNivel1::~MonstruoFinalNivel1(){}
MonstruoFinalNivel2::~MonstruoFinalNivel2(){}
MonstruoFinalNivel3::~MonstruoFinalNivel3(){}

