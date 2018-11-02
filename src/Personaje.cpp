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
	activo=false;
	grisado=false;
	godmode=false;

	posX = 50;
	posY = 280;

	coordenadaX = posX;
	//coordenadaY = posY; // Comienzan igual pero despues se actualizan diferentes

	arma = Pistola;
	estado = Quieto;
	direccionDisparo = Centro;
	saltando = false;
	disparando = false;
	mirandoALaDerecha = true;

	rectDestino = {posX,posY,34,72};

	spritesJugador = new Sprite();

	//vidas
	//vidas = maxVidas
	hitPoints = 3;
	invincibilityFrames = 0;

	loginfo("Se construyo personaje");
}

//--------GET----SET

Personaje::Estado Personaje::getEstado(){
	return estado;
}

Personaje::DireccionDisparo Personaje::getDireccionDisparo(){
	return direccionDisparo;
}

int Personaje::getPosX(){
	return posX;
}

int Personaje::getPosY(){
	return posY;
}

int Personaje::getCantidadDeBalas(){
	return bullets.size();
}

vector<Bullet*> Personaje::getBalas(){
	return bullets;
}

void Personaje::setBalas(vector<Bullet*> balas){
	for(unsigned i = 0;i < bullets.size();i++){
		delete bullets[i];	
	}
	bullets = balas;
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

void Personaje::setEstados(bool salto,bool disp,bool mirando,bool act,bool gris){
	saltando = salto;
	disparando = disp;
	mirandoALaDerecha = mirando;
	activo = act;
	if(grisado == false && gris == true){
		this->grisar();
	}
	else if(grisado == true && gris == false){
		this->desgrisar();
	}
}

void Personaje::setEstadosEnumerados(Constantes::Estado est,Constantes::DireccionDisparo dir){
	estado = (Personaje::Estado) est;
	direccionDisparo = (Personaje::DireccionDisparo) dir;
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

void Personaje::subirPosX(){
	posX+=3;
}

int Personaje::getInvincibilityFrames(){
	return invincibilityFrames;
}

void Personaje::agregarGris(const char* path,SDL_Renderer* renderer){
	tex2 = cfg.obtener_textura_grisada (path, renderer);
}





//-------Metodos----------

void Personaje::swapTextures(){
	SDL_Texture* backupTex;
	backupTex = tex2;
	tex2 = textura;
	textura = backupTex;
}


void Personaje::actualizar(){
	loginfo("Se comenzo actualizar personaje");

	posX += velocidadX;
	posY += velocidadY;
	coordenadaY += velocidadY;

	actualizarRectDestino();
	/*rectDestino.x = posX;
	rectDestino.y = posY;


	if(estado == CuerpoATierra && !saltando){
		rectDestino.y += 40;
		rectDestino.w = 72;
		rectDestino.h = 34;
	}
	else{
		rectDestino.w = 34;
		rectDestino.h = 72;
	}*/

	if(estado != HaciendoComoQueCamina && estado != CuerpoATierra && velocidadX == 0 && !saltando)
		estado = Quieto;
	if(estado == HaciendoComoQueCamina) estado = Caminando;
	
	if(!grisado&&saltando) velocidadY += gravedad;

	velocidadX = 0;
	loginfo("Se termino de actualizar personaje");
}

bool Personaje::estaMirandoALaDerecha(){
	return mirandoALaDerecha;
}

bool Personaje::estaDisparando(){
	return disparando;
}

void Personaje::activar(){
	activo=true;
}
void Personaje::desactivar(){
	activo=false;
}
bool Personaje::esActivo(){
	return activo;
}

void Personaje::grisar(){
	if (!grisado) this->swapTextures();
	velocidadY = 0;
	velocidadX = 0;
	grisado=true;
}
void Personaje::desgrisar(){
	if (grisado) this->swapTextures();
	grisado=false;
}
bool Personaje::esGrisado(){
	return grisado;
}

void Personaje::decrementarPosY(int y){
	if (!grisado){
		posY -=y;
	}
}

void Personaje::decrementarPosX(int x){
	if (!grisado){
		posX -=x;
	}
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

void Personaje::verSiBalasPegan(Enemigo* malo){
	for(unsigned i=0; i<bullets.size();i++){
		if(! malo->derrotado() && collision(bullets[i]->getRectaDestino(),malo->getRectaDestino())){
			malo->perderVida();
			bullets.erase(bullets.begin() + i);
			}
	}
}

void Personaje::dibujarBalas(SDL_Renderer* renderer){
	for(unsigned i = 0; i < bullets.size();i++){
		bullets[i]->dibujar(renderer);
	}
}

void Personaje::disparar(int dirX, int dirY,SDL_Texture *text){
	//ajusto posicion de la bala por estado
	int x=posX;
	int y=posY+24;
	//estos offset definen las balas extra para la escopeta.
	int offsetY1=0;
	int offsetX1=0;
	int offsetY2=0;
	int offsetX2=0;

	if(estado == CuerpoATierra){
		dirY = 0;
		if(dirX > 0) x += 40;
		y += 30;
	}

	if(estado == Personaje::Caminando){
		if(dirX < 0) x -= 10;
		else x += 10;
	}

	//ajusto por diagonal (divido por raiz de dos)
	if (dirY!=0){
		dirY = dirY*0.7;
		dirX = dirX*0.7;
	}

	//si esta apuntando para la derecha
	if(dirX > 0){
		//si esta apuntando para abajo
		if(dirY > 0){
			y -= 5;
			x += 10;

			offsetY1=+1;
			offsetX1=-1;
			offsetY2=-1;
			offsetX2=+2;
		}

		//si esta apuntando para arriba
		if(dirY < 0){
			y -= 12;
			x += 14;

			offsetY1=-2;
			offsetX1=-1;
			offsetY2=+1;
			offsetX2=+2;
		}

		//si apunta dereche
		if(dirY == 0){
			x += 15;

			offsetY1=2;
			offsetX1=-1;
			offsetY2=-2;
			offsetX2=-1;
		}
	}

	//si esta apuntando para la izquierda
	if(dirX < 0){
		//si esta apuntando para abajo
		if(dirY > 0){
			y += 10;

			offsetY1=+2;
			offsetX1=1;
			offsetY2=-1;
			offsetX2=-2;
		}

		//si esta apuntando para arriba
		if(dirY < 0){
			y -= 20;

			offsetY1=-2;
			offsetX1=1;
			offsetY2=+1;
			offsetX2=-2;
		}
		//si apunta dereche
		if(dirY == 0){
			x += 15;
			offsetY1=2;
			offsetX1=1;
			offsetY2=-2;
			offsetX2=1;
		}
	}

	//ajusto tamanio, el de la bazooka sera mayor
	int size=1;
	if (arma == Bazooka) size=4;

	//ajusto duracion, la escopeta tiene menor rango
	int duracion=60;
	if (arma == Escopeta) duracion =40;

	Bullet* nuevaBala;
	nuevaBala = new Bullet(x,y,dirX,dirY,size,duracion);
	nuevaBala->asignarTextura(text);
	bullets.push_back(nuevaBala);

	//para la escopeta, creo dos balas adicionales
	if (arma == Escopeta){
		Bullet* nuevaBala2;
		nuevaBala2 = new Bullet(x,y,dirX+offsetX1,dirY+offsetY1,size,duracion);
		nuevaBala2->asignarTextura(text);
		bullets.push_back(nuevaBala2);

		Bullet* nuevaBala3;
		nuevaBala3 = new Bullet(x,y,dirX+offsetX2,dirY+offsetY2,size,duracion);
		nuevaBala3->asignarTextura(text);
		bullets.push_back(nuevaBala3);
	}
	//define tiempo de disparo entre bala y bala
	//menor para ametralladora, mayor para bazooka
	shootTimer=15;
	if (arma == Ametralladora) shootTimer=5;
	if (arma == Bazooka) shootTimer=20;
}


void Personaje::pelarElChumbo(){
	disparando = true;
}

void Personaje::refreshBullets(){
	//Actualizo balas
	for(int i = 0; i < bullets.size(); i++){
		bullets[i]->move();
	}
	//borro las balas que exceden su rango para que no sigan hasta el infinito
	int i=0;
	while (i<bullets.size()){
		if (bullets[i]->getDuracion() ==0)
			bullets.erase(bullets.begin() + i);
		else i++;
	}
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

void Personaje::cambiarArma(int nroArma){
	if (nroArma ==1)
		arma = Pistola;
	if (nroArma ==2)
		arma = Ametralladora;
	if (nroArma ==3)
		arma = Escopeta;
	if (nroArma ==4)
		arma = Bazooka;
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
	if( !saltando && !grisado ){
		velocidadY = - maximaVelocidadY;
		saltando = true;
	}
}

void Personaje::caer(){
	if(!grisado) saltando = true;
}

void Personaje::aterrizar(){
	velocidadY = 0;
	if(!grisado) saltando = false;
	
}

void Personaje::bajar(){
	if(!grisado) velocidadY= 1;
}

void Personaje::resetFall(){
	velocidadY= 0;
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

void Personaje::actualizarRectDestino(){
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
}


void Personaje::dibujar(SDL_Renderer* renderer){
	loginfo("Se comenzo a dibujar personaje");

	actualizarRectDestino();
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

	if (!grisado) rectOrigen = rect_origen;
	
	if(mirandoALaDerecha)
		SDL_RenderCopy(renderer, textura, & rectOrigen , &rectDestino);
	else
		SDL_RenderCopyEx(renderer,textura,& rectOrigen,&rectDestino,180.0,NULL,SDL_FLIP_VERTICAL);

	loginfo("Se termino de dibujar personaje");
}

// Manejo de vidas
void Personaje::perderVida(){
	if (!godmode && (invincibilityFrames == 0)) {
		this->cambiarArma(1);
		hitPoints--;
		invincibilityFrames = 90;
	}
	//invincibilityFrames = IFramesMAX
}

bool Personaje::muerto(){
	return (hitPoints <= 0);
}

void Personaje::refreshIFrames(){
	if(invincibilityFrames > 0)
		invincibilityFrames--;
	if ((invincibilityFrames == 40) && godmode)
		invincibilityFrames = 44;
}

void Personaje::godmodeSwitch(){
	if (invincibilityFrames ==0){
		godmode = true;
		invincibilityFrames = 90;
	} else if (invincibilityFrames <= 44)
		godmode = false;
}


//--------Destructor-------

Personaje::~Personaje() {
	//se destruye en el padre
	for(unsigned i = 0;i < bullets.size();i++){
		bullets[i]->~Bullet();
	}

}

