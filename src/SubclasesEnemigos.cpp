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
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
	}
	coordenadaActiva = coordenada;

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

/////////////////////////////////////MARCIANITO////////////////////////////////////////////
//se mueve de izquierda a derecha. dispara a la derecha

Marcianito::Marcianito(int x,int y, int coorX, int coordenada, int nivel):Enemigo(){

	rectOrigen = {0,0,40,50};
	rectDestino = {x,y,64,80};
	posX=x;
	posY=y;
	
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
	}
	coordenadaActiva = coordenada;

	nivelActivo=nivel;
	activo=false;
	marcapasos=0;
	shootTimer=30;
}

void Marcianito::hazLoTuyo(){
	//el marcianito se mueve de lado a lado.
	//por ahora dispara adelante pero despues lo cambio para que se de vuelta

	//movimiento
	if(marcapasos>=40){
		moverDerecha(3);
	} else{
		moverIzquierda(3);
	}
	marcapasos++;
	if(marcapasos==80)
		marcapasos=0;

	//animacion
	if(marcapasos%10==0){
		if(rectOrigen.x==120)
			rectOrigen.x=0;
		else rectOrigen.x+=40;
	}
	if(marcapasos==40)
		rectOrigen.y=50;
	if(marcapasos==0)
		rectOrigen.y=0;

	//disparo
	if(shootTimer==0)
		quieroDisparar=true;	
	
}

void Marcianito::disparo(){
	shootTimer=40;
	quieroDisparar=false;
}

int Marcianito::disparoXVel(){
	if ((marcapasos<35) && (marcapasos>25))
		return -4;
	else return 4;
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

//////////////////////////////////////TURRET////////////////////////////////////////
//estacionario, apunta y dispara en doce direcciones

Turret::Turret(int x,int y, int coorX, int coordenada, int nivel){
	Enemigo();

	rectOrigen = {0,0,34,34};
	rectDestino = {x,y,80,80};
	posX=x;
	posY=y;
	
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
	}
	coordenadaActiva = coordenada;

	nivelActivo=nivel;
	activo=false;
	marcapasos=0;
	apunteX=0;
	apunteY=-5;
}

void Turret::hazLoTuyo(){

	//el turret rota 
	//esta animado y dispara hacia donde mira

	
	//ciclo de apunte: marcapasos indica las fases (0 apunta arriba, 3 apunta derecha, etc)
	if (shootTimer==0){
		//en medio de cada ciclo de disparo, roto el turret
		switch(marcapasos){
		case 0:
			rectOrigen.x=0;
			apunteX=0;
			apunteY=-5;
			break;
		case 1:
			rectOrigen.x+=34;
			apunteX=2;
			apunteY=-4;
			break;
		case 2:
			rectOrigen.x+=34;
			apunteX=4;
			apunteY=-2;
			break;
		case 3:
			rectOrigen.x+=34;
			apunteX=5;
			apunteY=0;
			break;
		case 4:
			rectOrigen.x+=34;
			apunteX=4;
			apunteY=2;
			break;
		case 5:
			rectOrigen.x+=34;
			apunteX=2;
			apunteY=4;
			break;
		case 6:
			rectOrigen.x+=34;
			apunteX=0;
			apunteY=5;
			break;
		case 7:
			rectOrigen.x+=34;
			apunteX=-2;
			apunteY=4;
			break;
		case 8:
			rectOrigen.x+=34;
			apunteX=-4;
			apunteY=2;
			break;
		case 9:
			rectOrigen.x+=34;
			apunteX=-5;
			apunteY=0;
			break;
		case 10:
			rectOrigen.x+=34;
			apunteX=-4;
			apunteY=-2;
			break;
		case 11:
			rectOrigen.x+=34;
			apunteX=-2;
			apunteY=-4;
			break;
		default:
			break;	
		}
	}
	
	if(shootTimer==0){
		quieroDisparar=true;
		//cada vez que dispara cambia de fase
		//la imagen y direccion de bala no cambia hasta la mitad de la rotacion
		marcapasos++;
		if(marcapasos==12)
			marcapasos=0;
	}	

}

void Turret::disparo(){
	shootTimer=20;
	quieroDisparar=false;
}

int Turret::disparoXVel(){
	return apunteX;
}

int Turret::disparoYVel(){
	return apunteY;
}

string Turret::serializar(){
	string serial = "";
	if(rectDestino.x > -300 && rectDestino.x < 10000 && rectDestino.y > -300 && rectDestino.y < 10000){
		serial = to_string(Constantes::turret);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	}

	return serial;
}

Turret::~Turret() {
	// TODO Auto-generated destructor stub
}

/////////////////////////////////////PARACAIDAS//////////////////////////////////////
//aparece desde la derecha por arriba de la pantalla y dispara para abajo

Paracaidas::Paracaidas(int x,int y, int coorX, int coordenada, int nivel) {
	Enemigo();

	rectOrigen = {0,0,56,86};
	rectDestino = {x,y,56,86};
	posX=x;
	posY=y;
	
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada;
	}
	coordenadaActiva = coordenada;

	nivelActivo=nivel;
	activo=false;
	marcapasos=0;
}


void Paracaidas::hazLoTuyo(){
	//cae lentamente disparando a la izquierda en un angulo hacia abajo

	moverAbajo(2);
	if(shootTimer==0)
		quieroDisparar=true;			
}

void Paracaidas::disparo(){
	shootTimer=60;
	quieroDisparar=false;
}

int Paracaidas::disparoXVel(){
	return -6;
}

int Paracaidas::disparoYVel(){
	return 2;
}

string Paracaidas::serializar(){
	string serial = "";
	if(rectDestino.x > -300 && rectDestino.x < 10000 && rectDestino.y > -300 && rectDestino.y < 10000){
		serial = to_string(Constantes::paracaidas);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
		serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	}

	return serial;
}

Paracaidas::~Paracaidas() {
	// TODO Auto-generated destructor stub
}




///////////////////////////////////////////JEFES////////////////////////////////////////////


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


void MonstruoFinal::activar(int nivel, int coordenada, int pos){
	//uso la implementacion vieja para que no se pise con lo de barbara
	int desfasaje=0;
	if((vidas>0)&&(nivel==nivelActivo)&&(!this->pasaBorde(nivel))){
		if((nivel==2)&&(coordenada<=coordenadaActiva)){
			activo=true;
			desfasaje= coordenadaActiva - coordenada - 300 + pos;
			rectDestino.y+=desfasaje;
			posY+=desfasaje;
		}else{
			if((nivel!=2)&&(coordenada>=coordenadaActiva)){
				activo=true;
				rectDestino.x+=desfasaje;
				posX+=desfasaje;
			}
		}
	}
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
		nuevaBala->setTipoBala(Constantes::balaFuego);
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
		nuevaBala->setTipoBala(Constantes::balaFuego2);
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
			nuevaBala->setTipoBala(Constantes::balaMisil);
			j->agregarBalaEnemigo(nuevaBala);
			//Aca poner sprite rotado.
		}
		else{
			nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", j->dameElRender());
			nuevaBala->setTipoBala(Constantes::balaMisil);
			nuevaBala->actualizarRectOrigenOriginal();
			j->agregarBalaEnemigo(nuevaBala);
		}
	}
	
	
}

void MonstruoFinal::tenerInmunidad(){
	inmune = 10; //1seg de inmunidad
}

void MonstruoFinal::tenerInvisibilidad(){
	invisible = 10; // en juego evitar que se dibuje
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

	if (contador_marcos % 15 == 0) {
        marco++;
        rectOrigen.x = marco*(160);
        if (marco > 2) {
        	rectOrigen.x = 0;
            marco = 0;
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
MonstruoFinal( j,x,  y, life, 7300, 1 ){
	rectOrigen = {0,0,160,180};
	rectDestino = {x,y,252,240};
}

MonstruoFinalNivel2::MonstruoFinalNivel2(juego* j,int x, int y, int life):
MonstruoFinal(j, x, y, life, 115, 2){}

MonstruoFinalNivel3::MonstruoFinalNivel3(juego* j,int x, int y, int life):
MonstruoFinal( j, x,  y,  life , 7400 , 3){}

MonstruoFinalNivel1::~MonstruoFinalNivel1(){}
MonstruoFinalNivel2::~MonstruoFinalNivel2(){}
MonstruoFinalNivel3::~MonstruoFinalNivel3(){}

string MonstruoFinalNivel1::serializar(){


	string serial = to_string(Constantes::MonstruoNivel1);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	//rectDestino.x < 1 ? 1 : rectDestino.x
	return serial;
}

string MonstruoFinalNivel2::serializar(){
	string serial = to_string(Constantes::MonstruoNivel2);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}

string MonstruoFinalNivel3::serializar(){
	string serial = to_string(Constantes::MonstruoNivel3);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);

	return serial;
}
