#include "Enemigo.h"

Enemigo::Enemigo(int x,int y,int life, int coordenada, int nivel){
	//x e y definen en que posicion de la pantalla se crea el enemigo
	//life define con cuantas vidas arranca
	//nivel define en que nivel aparece
	//coordenada indica (para nivel 2) la coordenada Y en la que quiero que aparezca
	//y para nivel 1 y 3 la coordenada X en la que quiero que aparezca

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
	shootTimer=0;
	visible = true;
	inmune = 0;
}

Enemigo::Enemigo() {

	EntidadDibujable();
	rectOrigen = {0,0,250,158};
	rectDestino = {0,0,252,240};
	coordenadaX=0;
	coordenadaY=0;
	posX=0;
	posY=0;
	activo=false;
	vidas=1;
	nivelActivo=1;
	coordenadaActiva=0;
	marcapasos=0;
	shootTimer=0;
	visible = true;
	inmune = 0;
}

Enemigo::Enemigo(int x,int y,int life) {

	EntidadDibujable();
	vidas = life;
	rectOrigen = {0,0,250,158};
	rectDestino = {x,y,252,240};
	posX=x;
	posY=y;
	visible = true;
	inmune = 0;
}

bool Enemigo::perderVida(){

	if(inmune > 0){
		return false;
	}

	vidas--;
	if (this->derrotado()){
		activo=false;
		return true;
	}
	return false;
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

int Enemigo::coordenadaXParaItem(){
	return coordenadaX+(rectDestino.w/2);
}

int Enemigo::coordenadaYParaItem(){
	return coordenadaX+(rectDestino.h/2);
}

int Enemigo::posicionXParaItem(){
	return rectDestino.x+(rectDestino.w/2);
}

int Enemigo::posicionYParaItem(){
	return rectDestino.y+(rectDestino.h/2);
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

void Enemigo::disparo(){
	shootTimer=120;
	quieroDisparar=false;
}

void Enemigo::bajarShootTimer(){
	if(shootTimer > 0){
		shootTimer--;
	}
}

int Enemigo::disparoXVel(){
	return -3;
}

int Enemigo::disparoYVel(){
	return 0;
}

bool Enemigo::quiereDisparar(){
	return(quieroDisparar);
}


void Enemigo::activar(int nivel, int coordenada, int pos){
	int desfasaje=0;
	
	if((vidas>0)&&(nivel==nivelActivo)&&(!this->pasaBorde(nivel))){

		if(nivel!=2){
			//para ver si tengo que activar, hago coordenada-pos (el resultado es la coordenada al borde izquierdo de la pantalla)
			//y le sumo la posX donde quiero que aparezca el enemigo. Esto lo llamo desfasaje.
			//Si este valor supera la coordenada activa, el enemigo se activa.
			desfasaje= coordenada-pos+rectDestino.x;

			//la condicion seria if (desfasaje>=coordenadaActiva), activo.
			//en otras palabras, if (desfasaje-coordenadaActiva>=0), activo.

			desfasaje= desfasaje-coordenadaActiva;

			if(desfasaje>=0){
				activo=true;
				//Pero para que hice esta resta innecesaria?
				//Bueno, supongamos que tengo un enemigo con X=200, coordenadaActiva=2000.
				//si boby esta en una coordenada y posicion tal que coordenada-pos+rectDestino.x da 1997, entonces no debo activar.
				//PERO. Que pasa si en el proximo frame boby avanza 5 px, de tal manera que coordenada-pos+rectDestino.x=2002?
				//El enemigo se activa, eso está bien...
				//Pero si le digo que aparezca en la posicionX =200... va a aparecer corrido
				//Le tengo que restar esa diferencia a la posicion.
				rectDestino.x -=desfasaje;
				posX -=desfasaje;
			}
		}else{
			//idem pero con las coordenadasY
			desfasaje= coordenada-pos+rectDestino.y -coordenadaActiva;

			if(desfasaje<=0){
				activo=true;
				rectDestino.y+=desfasaje;
				posY+=desfasaje;
			}

		}	
	}
}

void Enemigo::actualizar(int nivel, int coordenada){
	if(activo){

		if(inmune > 0){
		inmune--;
		}

		if(invisible > 0){
		invisible--;
		}
		else{
			visible = true;
		}


		if(shootTimer>0)
			shootTimer--;

		
		this->hazLoTuyo();

	}


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

void Enemigo::moverArriba(int y){
	coordenadaY-=y;
	posY-=y;
	rectDestino.y-=y;
}

void Enemigo::moverAbajo(int y){
	coordenadaY+=y;
	posY+=y;
	rectDestino.y +=y;
}

void Enemigo::hazLoTuyo(){
	//aca va todo el comportamiento del enemigo cuando esta activo
	//moverse, disparar, actualizar algun frame, actualizar gravedad, etc.
	//se sobreescribe para cada subclase

	//un ejemplo(mueve lado a lado)
	/*
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
	//ejemplo (dispara)
	if((tipoEnemigo==3)&&(shootTimer==0)){
		quieroDisparar=true;
	}*/	
	moverIzquierda(3);			
}

string Enemigo::serializar(){
	return "soy tu padre";
}

void Enemigo::setMarcaPasos(int valor){
	//SE REDEFINE EN LAS CLASES HIJAS
}

int Enemigo::obtenerPuntaje(){
	int score=0;
	if(vidas==1)
		score=50;
	return score;
}

bool Enemigo::estoyEnEscena(int x,int y,int w,int h){
	int unCacho = 300;
	if(coordenadaX + rectDestino.w <= x) return false;
	if(coordenadaX >= x + w + unCacho) return false;
	if(coordenadaY + rectDestino.h <= y) return false;
	if(coordenadaY >= y + h) return false;
	return true;
}

Enemigo::~Enemigo() {
	// TODO Auto-generated destructor stub
}
