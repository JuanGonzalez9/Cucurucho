#include "ArmadorDeRespuesta.h"

ArmadorDeRespuesta::ArmadorDeRespuesta() {
	nivel = 0;
	fondo1 = 0;
	fondo2 = 0;
	fondo3 = 0;
	hayEnemigo = false;

	int longitudStringBalas = MAX_BALAS * TAMANIO_POS_BALAS;
	string aux(longitudStringBalas,'0');
	strBalas = aux;
	resetBalas = aux;
	balasActuales = 0;

	int longitudEnemigos = MENSAJE_ENEMIGOS;
	string aux2(longitudEnemigos,'0');
	mensajeEnemigo = aux2;
	resetEnemigos = aux2;

	int longitudItems = MENSAJE_ITEMS;
	string aux3(longitudItems,'0');
	mensajeItems = aux3;
	resetItems = aux3;

	int longitudStringBalasEnemigas = MENSAJE_BALAS_ENEMIGAS;
	string aux4(longitudStringBalasEnemigas,'0');
	strBalasEnemigas = aux4;
	resetBalasEnemigas = aux4;

}

// --------------- setter getter
void ArmadorDeRespuesta::setNivel(int nivel){
	this->nivel = nivel;
}

void ArmadorDeRespuesta::setFondo1(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo1 = x;
	}
	else {
		fondo1 = y;
	}
}

void ArmadorDeRespuesta::setFondo2(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo2 = x;
	}
	else {
		fondo2 = y;
	}
}

void ArmadorDeRespuesta::setFondo3(int x, int y){
	if(nivel == 1 || nivel == 3){
		fondo3 = x;
	}
	else {
		fondo3 = y;
	}
}

void ArmadorDeRespuesta::setPosPersonaje(int x,int y){
	posPersonajeX = x;
	posPersonajeY = y;
}

void ArmadorDeRespuesta::setSaltando(bool saltando){
	this->saltando = saltando;
}

void ArmadorDeRespuesta::setDisparando(bool disp){
	this->disparando = disp;
}

void ArmadorDeRespuesta::setMirandoALaDerecha(bool mirando){
	this->mirandoALaDerecha = mirando;
}

void ArmadorDeRespuesta::setActivo(bool active){
	this->activo = active;
}

void ArmadorDeRespuesta::setGrisado(bool grisado){
	this->grisado = grisado;
}

void ArmadorDeRespuesta::setEstado(Constantes::Estado estado){
	this->estado = estado;
}

void ArmadorDeRespuesta::setDireccionDisparo(Constantes::DireccionDisparo dir){
	this->dirDisparo = dir;
}

void ArmadorDeRespuesta::setCantidadDeBalas(int balas){
	this->cantidadDeBalas = balas;
	balasActuales = 0;
}

void ArmadorDeRespuesta::setEnemigoFinal(bool hay){
	this->hayEnemigo = hay;
}

void ArmadorDeRespuesta::setMensajeEnemigos(string msjEnemigo){
	if( (msjEnemigo.size() / MENSAJE_ENEMIGOS) > MAX_ENEMIGOS_EN_ESCENA){
		cout<<"error, la cantidad maxima de enemigos en escena es "<<MAX_ENEMIGOS_EN_ESCENA<<endl;
	}
	this->mensajeEnemigo.replace(0,msjEnemigo.size(),msjEnemigo);
}

void ArmadorDeRespuesta::setMensajeItems(string msjItems){
	if( (msjItems.size() / MENSAJE_ITEMS) > MAX_ITEMS_EN_ESCENA){
		cout<<"error, la cantidad maxima de items es "<<MAX_ITEMS_EN_ESCENA<<endl;
	}
	this->mensajeItems.replace(0,msjItems.size(),msjItems);
}

void ArmadorDeRespuesta::setMensajeBalasEnemigas(string msjBalas){
	if( (msjBalas.size() / MENSAJE_BALAS_ENEMIGAS) > MAX_BALAS_ENEMIGAS){
		cout<<"error , se supero la cantidad maxima de balas enemigas"<<endl;	
	}
	this->strBalasEnemigas.replace(0,msjBalas.size(),msjBalas);
}


//------------ METODOS ------------
string ArmadorDeRespuesta::fondoToString(int f){
	string respuesta = "0000";
	string nuevo = to_string(f);

	//para que la respuesta ocupe exactamente 4 caracteres
	respuesta.replace(4 - nuevo.size(),nuevo.size(),nuevo);

	loginfo("Se devuelve la respuesta del fondo");
	return respuesta;
}

string ArmadorDeRespuesta::posicionToString(int p){

	string respuesta = "000";
	string nuevo = to_string(p);

	//para que la respuesta ocupe exactamente 4 caracteres
	respuesta.replace(3 - nuevo.size(),nuevo.size(),nuevo);

	loginfo("Se devuelve la respuesta de la posicion");
	return respuesta;
}

string ArmadorDeRespuesta::pasarAStringDeTamanio(int tamanio,int valor){

	string respuesta(tamanio,'0');

	string nuevo = to_string(valor);
	respuesta.replace(tamanio - nuevo.size(),nuevo.size(),nuevo);

	loginfo("Se pasa la respuesta a string");
	return respuesta;
}

void ArmadorDeRespuesta::sumarBalas(vector<Bullet*> nuevasBalas){
	for(unsigned i = 0; i < nuevasBalas.size();i++){
		string nuevoX = pasarAStringDeTamanio(RESPUESTA_POSX + 1,nuevasBalas[i]->getRectaDestino().x);
		
		strBalas.replace(TAMANIO_POS_BALAS * balasActuales,RESPUESTA_POSX + 1,nuevoX);

		string nuevoY = pasarAStringDeTamanio(RESPUESTA_POSY,nuevasBalas[i]->getRectaDestino().y);
		strBalas.replace(TAMANIO_POS_BALAS * balasActuales + RESPUESTA_POSX + 1,RESPUESTA_POSY,nuevoY);

		balasActuales++;
	}

	loginfo("Se sumaron las balas");
}

string ArmadorDeRespuesta::dameLaRespuestaPara(int jugadores, DatosPersonaje* datosBoby, DatosPersonaje* datosBoby2, DatosPersonaje* datosBoby3, DatosPersonaje* datosBoby4){

	loginfo("Se arma la respuesta para un respectivo jugador");
	//Estan cargado desde juego los parametros universales para los 4 jugadores
	string respuesta = to_string(nivel);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo1);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo2);
	respuesta += pasarAStringDeTamanio(RESPUESTA_FONDO,fondo3);

	//Dependiendo los jugadores se codifica de alguna manera

	respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby->getPosX());
	respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby->getPosY());
	respuesta += to_string(datosBoby->estaSaltando());
	respuesta += to_string(datosBoby->estaDisparando());
	respuesta += to_string(datosBoby->estaMirandoALaDerecha());
	respuesta += to_string(datosBoby->estaActivo());
	respuesta += to_string(datosBoby->estaGrisado());
	respuesta += to_string(datosBoby->getEstado());
	respuesta += to_string(datosBoby->getDireccionDisparo());

	if(jugadores >= 2){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby2->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby2->getPosY());
		respuesta += to_string(datosBoby2->estaSaltando());
		respuesta += to_string(datosBoby2->estaDisparando());
		respuesta += to_string(datosBoby2->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby2->estaActivo());
		respuesta += to_string(datosBoby2->estaGrisado());
		respuesta += to_string(datosBoby2->getEstado());
		respuesta += to_string(datosBoby2->getDireccionDisparo());

	}

	if(jugadores >= 3){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby3->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby3->getPosY());
		respuesta += to_string(datosBoby3->estaSaltando());
		respuesta += to_string(datosBoby3->estaDisparando());
		respuesta += to_string(datosBoby3->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby3->estaActivo());
		respuesta += to_string(datosBoby3->estaGrisado());
		respuesta += to_string(datosBoby3->getEstado());
		respuesta += to_string(datosBoby3->getDireccionDisparo());
	}

	if(jugadores == 4){
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSX,datosBoby4->getPosX());
		respuesta += pasarAStringDeTamanio(RESPUESTA_POSY,datosBoby4->getPosY());
		respuesta += to_string(datosBoby4->estaSaltando());
		respuesta += to_string(datosBoby4->estaDisparando());
		respuesta += to_string(datosBoby4->estaMirandoALaDerecha());
		respuesta += to_string(datosBoby4->estaActivo());
		respuesta += to_string(datosBoby4->estaGrisado());
		respuesta += to_string(datosBoby4->getEstado());
		respuesta += to_string(datosBoby4->getDireccionDisparo());
	}

	respuesta += pasarAStringDeTamanio(MENSAJE_CANT_BALAS,cantidadDeBalas);
	respuesta += strBalas;

	respuesta += to_string(hayEnemigo);
	respuesta += mensajeEnemigo;
	respuesta += mensajeItems;
	respuesta += strBalasEnemigas;

	strBalas = resetBalas;
	strBalasEnemigas = resetBalasEnemigas;
	mensajeEnemigo = resetEnemigos;
	mensajeItems = resetItems;	

	return respuesta;

}

//------------ DESTRUCTOR------------------
ArmadorDeRespuesta::~ArmadorDeRespuesta() {
}

