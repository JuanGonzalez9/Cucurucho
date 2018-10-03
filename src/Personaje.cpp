/*
 * Personaje.cpp
 *
 *  Created on: Sep 18, 2018
 *      Author: juan
 */

#include "Personaje.h"

Personaje::Personaje() {
	EntidadDibujable();
	shootTimer = 0;
	velocidadY = 0;
	velocidadX = 0;
	maximaVelocidadX = 5;
	maximaVelocidadY = 22;
	gravedad = 1;


	posX = 50;
	posY = 280;

	coordenadaX = posX;
	//coordenadaY = posY; // Comienzan igual pero despues se actualizan diferentes

	estado = Quieto;
	direccionDisparo = Centro;
	saltando = false;
	disparando = false;
	mirandoALaDerecha = true;

	rectDestino = {posX,posY,34,72};

	spritesJugador = new Sprite();

	//vidas
	//vidas = maxVidas
	hitPoints = 5;
	invincibilityFrames = 0;

	registro.registrar (LogEventos::info, "Se construyo personaje");
}

//--------GET----SET

Personaje::Estado Personaje::getEstado(){
	return estado;
}

int Personaje::getPosX(){
	return posX;
}

int Personaje::getPosY(){
	return posY;
}

void Personaje::setPosX(int x){
	posX=x;
}

void Personaje::setPosY(int y){
	posY=y;
}

void Personaje::setCoordenadaX(int x){
	coordenadaX= x;
}

void Personaje::setCoordenadaY(int y){
	coordenadaY= y;
}

int Personaje::obtenerCoordenadaX(){
	return coordenadaX;
}

int Personaje::obtenerCoordenadaY(){
	return coordenadaY;
}

int Personaje::obtenerVelocidadY(){
	return velocidadY;
}

int Personaje::obtenerVelocidadX(){
	return velocidadX;
}

//<<<<<<< HEAD
void Personaje::subirPosX(){
	posX+=3;
}
//=======
int Personaje::getInvincibilityFrames(){
	return invincibilityFrames;
//>>>>>>> 6100df544e34c0d07915ef9e808d20fe90d8f406
}

//-------Metodos----------

void Personaje::actualizar(){
	registro.registrar (LogEventos::info, "Se comenzo actualizar personaje");

	posX += velocidadX;
	posY += velocidadY;
	coordenadaY += velocidadY;
	rectDestino.x = posX;
	rectDestino.y = posY;

	if(estado == CuerpoATierra && !saltando){
		rectDestino.y += 40;
		rectDestino.w = 72;
		rectDestino.h = 34;
	}
	else{
		rectDestino.w = 34;
		rectDestino.h = 72;
	}

	if(estado != HaciendoComoQueCamina && estado != CuerpoATierra && velocidadX == 0 && !saltando)
		estado = Quieto;
	if(estado == HaciendoComoQueCamina) estado = Caminando;
	if(saltando) velocidadY += gravedad;
	velocidadX = 0;
	registro.registrar (LogEventos::info, "Se termino de actualizar personaje");
}

bool Personaje::estaMirandoALaDerecha(){
	return mirandoALaDerecha;
}

void Personaje::decrementarPosY(int y){
	posY -=y;
	coordenadaY -=y;
}

bool Personaje::estaSaltando(){
	return saltando;
}

void Personaje::avanzar(){
	velocidadX = maximaVelocidadX;
	estado = Caminando;
	mirandoALaDerecha = true;
}

void Personaje::hacerComoQueCamina(){
	estado = HaciendoComoQueCamina;
	mirandoALaDerecha = true;
}

void Personaje::retroceder(){
	velocidadX = - maximaVelocidadX;
	estado = Caminando;
	mirandoALaDerecha = false;
}

void Personaje::agacharse(){
	estado = CuerpoATierra;
}

void Personaje::pararse(){
	if(estado == CuerpoATierra)
		estado = Quieto;
}

void Personaje::disparar(){
	//shoot timer = rateOfFire
	shootTimer = 10;
}

void Personaje::pelarElChumbo(){
	disparando = true;
}

void Personaje::refreshBullets(){
	if(shootTimer > 0)
		shootTimer--;
}

bool Personaje::puedeDisparar(){

	if(shootTimer <= 0)
		return true;

	return false;
}

void Personaje::dejarDeDisparar(){
	disparando = false;
}

void Personaje::apuntarAbajo(){
	direccionDisparo = Abajo;
}

void Personaje::apuntarArriba(){
	direccionDisparo = Arriba;
}

void Personaje::dejarDeApuntar(){
	direccionDisparo = Centro;
}

void Personaje::saltar(){
	//esto podria cambiar si tenemos estados como SaltandoYAvanzando o Saltando y otra cosa
	//deberia decir if(esta en una plataforma)
	if( !saltando ){
		velocidadY = - maximaVelocidadY;
		saltando = true;
	}
}

void Personaje::caer(){
	
	saltando = true;
}

void Personaje::aterrizar(){
	velocidadY = 0;
	saltando = false;
	
}

void Personaje::bajar(){
	velocidadY= 1;
}

void Personaje::flotar(){
	velocidadY= -3;
}

void Personaje::subirCoordenadaXEn(int cantidad){
	coordenadaX += cantidad;
}

void Personaje::subirCoordenadaYEn(int cantidad){
	coordenadaY += cantidad;
}

void Personaje::actualizarPos(int nuevaPos, int nivel){
	if (nivel == 1 || nivel == 3){
		posY = nuevaPos;
	}
	else if (nivel == 2){


		posY += nuevaPos- coordenadaY ;

		coordenadaY = nuevaPos;
	}
	
	
}

bool Personaje::llegoAlFinalDelNivel1(){
	return (coordenadaX >= 7600);
}

bool Personaje::llegoAlFinalDelNivel2(){
	
	return (coordenadaY <= 30);
}

bool Personaje::llegoAlFinalDelNivel3(){
	return (coordenadaX >= 7600);
}




bool Personaje::estaCercaDelFinalDelNivel1(){
	return (coordenadaX >= 7400);
}

bool Personaje::estaCercaDelFinalDelNivel2(){
	return (coordenadaY <= 300);
}

bool Personaje::estaCercaDelFinalDelNivel3(){
	return (coordenadaX >= 7400);
}


void Personaje::dibujar(SDL_Renderer* renderer){
	registro.registrar (LogEventos::info, "Se comenzo a dibujar personaje");
	SDL_Rect rect_origen;
	if(saltando){
		rect_origen = spritesJugador->getFrameSaltando();
	}
	else{
		switch(estado){
			case Caminando:
				if(disparando){
					if(direccionDisparo == Centro)
						rect_origen = spritesJugador->getFrameDisparandoCaminando();
					if(direccionDisparo == Arriba)
						rect_origen = spritesJugador->getFrameDisparandoArribaCaminando();
					if(direccionDisparo == Abajo)
						rect_origen = spritesJugador->getFrameDisparandoAbajoCaminando();
				}
				else
					rect_origen = spritesJugador->getFrameCaminando();
				break;
			case CuerpoATierra:
					rect_origen = spritesJugador->getFrameAgachado();
					break;
			default:
				if(disparando){
					if(direccionDisparo == Centro)
						rect_origen = spritesJugador->getFrameDisparando();
					if(direccionDisparo == Arriba)
						rect_origen = spritesJugador->getFrameDisparandoArriba();
					if(direccionDisparo == Abajo)
						rect_origen = spritesJugador->getFrameDisparandoAbajo();
				}
				else
					rect_origen = spritesJugador->getFrameQuieto();
				break;
		}
	}

	if(mirandoALaDerecha)
		SDL_RenderCopy(renderer, textura, & rect_origen , &rectDestino);
	else
		SDL_RenderCopyEx(renderer,textura,& rect_origen,&rectDestino,180.0,NULL,SDL_FLIP_VERTICAL);

	registro.registrar (LogEventos::info, "Se termino de dibujar personaje");
}

// Manejo de vidas -- guerreando
void Personaje::perderVida(){
	//cout<<"perdi vida ahora me queda "<<hitPoints<<endl;
	hitPoints--;
	invincibilityFrames = 90;
	//invincibilityFrames = IFramesMAX
}

bool Personaje::muerto(){
	return (hitPoints <= 0);
}

void Personaje::refreshIFrames(){
	if(invincibilityFrames > 0)
		invincibilityFrames--;
}

//--------Destructor-------

Personaje::~Personaje() {
	//se destruye en el padre
}

