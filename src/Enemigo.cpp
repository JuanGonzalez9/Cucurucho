#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,int life, int coordenada, int nivel) {
	//x e y definen en que posicion de la pantalla se crea el enemigo
	//life define con cuantas vidas arranca
	//nivel define en que nivel aparece
	//coordenada indica en que coordenada del nivel dado se activa
	//es decir, a que coordenada de la pantalla se lo activa.


	EntidadDibujable();
	vidas = life;
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
	posX=x;
	posY=y;
	if(nivel==2){
		coordenadaX=x;
		coordenadaY=coordenada-rectDestino.h+posY;
	}else{
		coordenadaY=y;
		coordenadaX=coordenada+posX-800;
	}
	coordenadaActiva=coordenada;
	nivelActivo=nivel;
	activo=false;
}

Enemigo::Enemigo(int x,int y,int life) {

	EntidadDibujable();
	vidas = life;
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
	posX=x;
	posY=y;
}

void Enemigo::perderVida(){
	vidas--;
	if (this->derrotado()){
		activo=false;
	}
}

bool Enemigo::derrotado(){
	return (vidas <= 0);
}

int Enemigo::obtenerCoordenadaActiva(){
	return coordenadaActiva;
}

bool Enemigo::pasaBorde(int nivel){
	bool pasa=false;
	if (nivel !=2){
		pasa= ((posX+rectOrigen.w)<=0);
	}else{
		pasa= (posY>600);
	}
	return pasa;
}

void Enemigo::empujarAtras(int i, int nivel){
	if(activo){
		if (nivel ==2){
			posX= posY-i;
			rectDestino.y=rectDestino.y-i;
		}else{
			posX= posX-i;
			rectDestino.x=rectDestino.x-i;
		}
	}
}

bool Enemigo::esActivo(){
	return activo;
}

void Enemigo::activar(int nivel, int coordenada){
	if(nivel==nivelActivo){
		if(coordenada>=coordenadaActiva)
			activo=true;
	}
}

void Enemigo::activarV2(int nivel, int coordenada){
	if((vidas>0)&&(nivel==nivelActivo)&&(!this->pasaBorde(nivel))){
		if((nivel==2)&&(coordenada<=coordenadaActiva)){
			activo=true;
		}else{
			if((nivel!=2)&&(coordenada>=coordenadaActiva))
				activo=true;
		}
	}
}

void Enemigo::actualizarV2(int nivel, int coordenada){
	if(activo&&(posY+rectDestino.h>0)&&(posX>0)){
			this->hazLoTuyo();
	}

}

void Enemigo::actualizar(int nivel, int coordenada){
	if(nivel==2){
		if(nivelActivo==nivel)
			activo=true;
	}else{
		if(!activo)
			this->activar(nivel, coordenada);
	}

	if(activo&&(posY+rectDestino.h>0)){
			this->hazLoTuyo();
	}

}

void Enemigo::hazLoTuyo(){
			//aca va todo el comportamiento del enemigo cuando esta activo
			//moverse, disparar, actualizar algun frame, actualizar gravedad, etc.
			//se sobreescribe para cada subclase
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}

