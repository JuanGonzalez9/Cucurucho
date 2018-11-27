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
	string serial = "";
	if(rectDestino.x > -300 && rectDestino.x < 10000 && rectDestino.y > -300 && rectDestino.y < 10000){
		serial = to_string(Constantes::ovni);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	}

	return serial;
}

Ovni::~Ovni() {
	// TODO Auto-generated destructor stub
}


///////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////MARCIANITO////////////////////////////////////////////
//se mueve de izquierda a derecha. dispara a la derecha

Marcianito::Marcianito(int x,int y, int coorX, int coordenada, int nivel):Enemigo(){

	rectOrigen = {0,0,40,50};
	rectDestino = {x,y,50,65};
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

	if(marcapasos>=40){
		moverDerecha(3);
	} else{
		moverIzquierda(3);
	}
	marcapasos++;
	if(marcapasos==80)
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
	string serial = "";
	if(rectDestino.x > -300 && rectDestino.x < 10000 && rectDestino.y > -300 && rectDestino.y < 10000){
		serial = to_string(Constantes::marcianito);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	}

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
	
	inmune=0;
	quieroDisparar=false;
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
}


void MonstruoFinal::dispararBalasFuego(int coordenadaX,int coordenadaY,int level){
	
	int vel_x;
	int vel_y;
	Bullet * nuevaBala;

	if (level == 2){
		vel_x=3;
		vel_y=-5;
	}
	else{
		vel_x=-1;
		vel_y=0;
	}

	for(int i = 0; i<6 ; i++){
		vel_x+=(-i);
		vel_y++;		
		nuevaBala = new Bullet(coordenadaX,coordenadaY,vel_x,vel_y,2,100);
		nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", j->dameElRender());
		nuevaBala->actualizarRectOrigenOriginal();
		j->agregarBalaEnemigo(nuevaBala);
	}

	
}

void MonstruoFinal::dispararBalasFuego2(int coordenadaX,int coordenadaY, int level){
	
	
	int vel_x;
	int vel_y;
	Bullet * nuevaBala;

	if (level == 2){
		vel_x=3;
		vel_y=0;
	}
	else{
		vel_x=-1;
		vel_y=3;
	}

	for(int i = 0; i<6 ; i++){
		//vel_x+=(-i);
		vel_y--;		
		nuevaBala = new Bullet(coordenadaX,coordenadaY,vel_x,vel_y,2,100);
		nuevaBala->obtenerTextura("//configuracion//items//bala_fuego_2//sprite", j->dameElRender());
		nuevaBala->actualizarRectOrigenOriginal();
		j->agregarBalaEnemigo(nuevaBala);
	}


	

}

void MonstruoFinal::dispararMisiles(int coordenadaX,int coordenadaY, int level){
	//OJO CON TOCAR VALORES... O NO SE PARSEARAN...
	int vel_x;
	int vel_y;
	Bullet * nuevaBala;

	if (level == 2){
		vel_x=3;
		vel_y=1;
	}
	else{
		vel_x=-5;
		vel_y=0;
	}

	for(int i = 0; i<6 ; i++){
		
		vel_y++;		
		nuevaBala = new Bullet(coordenadaX,coordenadaY+(i*4),vel_x,vel_y,2,100);
		if(level==2){
			vel_x--;
			nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
			nuevaBala->actualizarRectOrigenOriginal();
			j->agregarBalaEnemigo(nuevaBala);
			//Aca poner sprite rotado.
		}
		else{
			nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
			nuevaBala->actualizarRectOrigenOriginal();
			j->agregarBalaEnemigo(nuevaBala);
		}
	}
	/*
	
	Bullet * nuevaBala;
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	nuevaBala->actualizarRectOrigenOriginal();
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	nuevaBala->actualizarRectOrigenOriginal();
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	nuevaBala->actualizarRectOrigenOriginal();
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	nuevaBala->actualizarRectOrigenOriginal();
	j->agregarBalaEnemigo(nuevaBala);
	nuevaBala = new Bullet(coordenadaX,coordenadaY,-5,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
	nuevaBala->actualizarRectOrigenOriginal();
	j->agregarBalaEnemigo(nuevaBala);
	*/
	
}

void MonstruoFinal::tenerInmunidad(){
	inmune = 40; //1seg de inmunidad
}

void MonstruoFinal::tenerInvisibilidad(){
	invisible = 40; // en juego evitar que se dibuje
	visible = false;
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

void MonstruoFinalNivel1::posicionar(){
	actualizarRectOrigenOriginal();

	rectDestino = {800-(rectOrigen.w+50),posY,rectOrigen.w+50,rectOrigen.h+50};
	posX=rectDestino.x;

	coordenadaY=posY;
	coordenadaX=8000-rectOrigen.w;
	coordenadaActiva=coordenadaX-800;
	
}

void MonstruoFinalNivel1::hazLoTuyo(){
	if(estoyEnEscena(j->rect_origen_fondo3.x,j->rect_origen_fondo3.y,j->rect_origen_fondo3.w,j->rect_origen_fondo3.h)){

		const int xx = 7860 - j->rect_origen_fondo3.x; 	
		const int yy = 160; 
		if (xx < 0 || xx >= 800) {
		 	return; 	
		}


		contador_marcos++;
		if(estado==comenzo){
			estado=disparando_fuego;
			dispararBalasFuego(xx,160,1);
			
			printf("Cambiando de estado a disparar balas fuego\n");
		}
		if(estado==disparando_fuego && contador_marcos>200){
			printf("Cambiando de estado a disparar balas fuego2\n");
			estado=disparando_fuego2;
			dispararMisiles(xx,160,1);
			dispararBalasFuego2(xx,160,1);
			
		}
		if(estado==disparando_fuego2 && contador_marcos>300){
			estado=inmunidad;
			tenerInmunidad();
			tenerInvisibilidad();
			printf("Cambiando de estado a inmune\n");
		}
		if(estado==inmunidad && inmune <= 0 ){
			contador_marcos=0;
			estado=comenzo;
			printf("Cambiando de estado a nuevo ciclo\n");
		}
	}


} 

void MonstruoFinalNivel2::hazLoTuyo(){
	
	if(estoyEnEscena(j->rect_origen_fondo3.x,j->rect_origen_fondo3.y,j->rect_origen_fondo3.w,j->rect_origen_fondo3.h)){
		contador_marcos++;
		if(estado==comenzo){
			estado=disparando_fuego;
			dispararBalasFuego(350,200,2);
			
		}
		if(estado==disparando_fuego && contador_marcos>200){
			estado=disparando_fuego2;
			dispararBalasFuego2(350,150,2);
			
		}
		if(estado==disparando_fuego2 && contador_marcos>300){
			estado=disparando_misiles;
			dispararMisiles(350,200,2);
			
		}
		if(estado==disparando_misiles && contador_marcos>400){
			contador_marcos=0;
			estado=comenzo;
			
		}
	}
}

void MonstruoFinalNivel3::hazLoTuyo(){
	
	if(estoyEnEscena(j->rect_origen_fondo3.x,j->rect_origen_fondo3.y,j->rect_origen_fondo3.w,j->rect_origen_fondo3.h)){
		
		const int xx = 7800 - j->rect_origen_fondo3.x; 	
		const int yy = 300; 
		if (xx < 0 || xx >= 800) {
			return; 	
		}
		contador_marcos++;
		if(estado==comenzo){
			estado=inmunidad;
			dispararBalasFuego(xx,yy,3);
			dispararBalasFuego2(xx,yy,3);
		}
		if(estado==inmunidad && inmune <= 0 ){
			estado=disparando_misiles;
			dispararMisiles(xx,yy,3);
		}
		if(estado==disparando_misiles && contador_marcos>300){
			estado=disparando_fuego;
			dispararBalasFuego(xx,yy,3);
			
		}
		if(estado==disparando_fuego && contador_marcos>400){
			contador_marcos=0;
			estado=comenzo;
		}
	}
}


MonstruoFinalNivel1::MonstruoFinalNivel1(juego* j,int x, int y, int life):
MonstruoFinal( j,x,  y, life, 7300, 1 ){}

MonstruoFinalNivel2::MonstruoFinalNivel2(juego* j,int x, int y, int life):
MonstruoFinal(j, x, y, life, 115, 2){}

MonstruoFinalNivel3::MonstruoFinalNivel3(juego* j,int x, int y, int life):
MonstruoFinal( j, x,  y,  life , 7400 , 3){}

MonstruoFinalNivel1::~MonstruoFinalNivel1(){}
MonstruoFinalNivel2::~MonstruoFinalNivel2(){}
MonstruoFinalNivel3::~MonstruoFinalNivel3(){}

string MonstruoFinalNivel1::serializar(){


	string serial = to_string(Constantes::marcianito);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	//rectDestino.x < 1 ? 1 : rectDestino.x
	return serial;
}

string MonstruoFinalNivel2::serializar(){
	string serial = to_string(Constantes::marcianito);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}

string MonstruoFinalNivel3::serializar(){
	string serial = to_string(Constantes::marcianito);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}