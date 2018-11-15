#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,int life, int coordenada, int nivel, int tipo){
	//x e y definen en que posicion de la pantalla se crea el enemigo
	//life define con cuantas vidas arranca
	//nivel define en que nivel aparece
	//coordenada indica (para nivel 2) la coordenada Y en la que quiero que aparezca
	//y para nivel 1 y 3 la coordenada X en la que quiero que aparezca
	
	//tipo indica el comportamiento del enemigo, luego se va a reemplazar por polimorfismo como dios manda
	//tipo 1 = no hace nada
	//tipo 2 = se mueve de lado a lado

	//Coordenada activa es la minima coordenada por la que tiene que pasar el jugador para que el enemigo aparezca
	//sirve para corregir errores como la reconexión de un jugador por adelante de la mitad de la pantalla

	//text es la textura que se le asigna

	EntidadDibujable();
	vidas = life;
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

	//marcapasos se usa para hacer ciclos a la hora de moverse
	marcapasos=0;

	tipoEnemigo=tipo;
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

int Enemigo::obtenerNivelActivo(){
	return nivelActivo;
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
			posY= posY+i;
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

void Enemigo::activar(int nivel, int coordenada, int pos){
	int desfasaje=0;
	if((vidas>0)&&(nivel==nivelActivo)&&(!this->pasaBorde(nivel))){
		if((nivel==2)&&(coordenada<=coordenadaActiva)){
			activo=true;
			//arreglo el desfasaje, que ocurre si uno de los jugadores
			//se pasa de mambo y reconecta pasando la mitad de la pantalla
			//para nivel 2 hay que correr la recta de destino y posicion para arriba segun:
			//(coordenadaActiva - coordenadaMaxima)-(300-posicionMaxima)
			desfasaje= coordenadaActiva - coordenada - 300 + pos;
			rectDestino.y+=desfasaje;
			posY+=desfasaje;
		}else{
			if((nivel!=2)&&(coordenada>=coordenadaActiva)){
				//desfasaje para nivel 1 o 3 es:
				//(coordact-coordmax)+(posmax-400)
				activo=true;
				//desfasaje= coordenadaActiva - coordenada +pos - 400;
				desfasaje= coordenadaActiva - coordenada +pos;
				rectDestino.x+=desfasaje;
				posX+=desfasaje;
			}
		}
	}
}

void Enemigo::actualizar(int nivel, int coordenada){
	if(activo)
		this->hazLoTuyo();
	//if(activo&&(posY+rectDestino.h>0)&&(posX<800)){

}

void Enemigo::moverIzquierda(int x){
	coordenadaX-=x;
	posX-=x;
	rectDestino.x -=x;
}

void Enemigo::moverDerecha(int x){
	coordenadaX+=x;
	posX+=x;
	rectDestino.x +=x;
}


void Enemigo::hazLoTuyo(){
	//aca va todo el comportamiento del enemigo cuando esta activo
	//moverse, disparar, actualizar algun frame, actualizar gravedad, etc.
	//se sobreescribe para cada subclase

	//un ejemplo(mueve lado a lado)
	if(tipoEnemigo==2){
		if(marcapasos>=60){
			moverDerecha(3);
		} else{
			moverIzquierda(3);
		}
		marcapasos++;
		if(marcapasos==120)
			marcapasos=0;
	}
			
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}