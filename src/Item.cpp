#include "Item.hpp"

Item::Item(int x,int y, int coorX, int coorY, int arma, int nivel) {
	//como enemigo, su posicion, coordenadas y nivel se determina por el constructor

	Enemigo();

	rectOrigen = {0,0,40,40};
	rectDestino = {x,y,40,40};

	coordenadaX=coorX;
	coordenadaY=coorY;
	nivelActivo=nivel;
	if (nivel==2)
		coordenadaActiva=coorY;
	else
		coordenadaActiva=coorX;

	//recentro el item:
	posX-=(rectDestino.w/2);
	posY-=(rectDestino.h/2);

	rectDestino.x-= (rectDestino.w/2);
	rectDestino.y-= (rectDestino.h/2);

	coordenadaX-=(rectDestino.w/2);
	coordenadaY-=(rectDestino.h/2);

	//el activo arranca en true siempre.
	activo=true;

	//para item, marcapasos actua como los frames de invisibilidad del personaje
	//arranca en un valor fijo (360 -> 6 segundos) y va bajando
	//cuando se termina el marcapasos el item se borra
	marcapasos=360;

	//recibe contenido desde arma.
	//cuando muere un enemigo, este genera un numero aleatorio de 1 a 10
	//si este numero esta entre 1 y 4, el enemigo genera este item,
	//y pasa el numero aleatorio entre 1 y 4 para determinar el contenido del item
	//este se pasa directo al personaje y le cambia el arma o sube vida.
	//1 es una vida extra
	//2 es la ametralladora
	//3 es la escopeta
	//4 es la bazooka
	contenido=arma;
}

void Item::actualizarItem(){
	marcapasos--;
	if (marcapasos==0)
		vidas=0;
}

bool Item::puedoDibujar(){
	//indica si debo dibujar el item o no
	//true si el item es activo y no esta derrotado
	//el resto hace que pestaniee en los ultimos 2 segundos
	bool dibujo= true;
	if(vidas<=0)
		dibujo=false;
	if(!activo)
		dibujo=false;
	if((marcapasos<120)&&((marcapasos/2) %2 == 0)) 
		dibujo=false;
	return dibujo;
}

int Item::darContenido(){
	return contenido;
}

Item::~Item() {
	// TODO Auto-generated destructor stub
}