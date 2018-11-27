#include "Parser.h"

Parser::Parser() {
	cantJugadores = 1;
	nivel = 1;
	posFondo1 = 0;
	posFondo2 = 0;
	posFondo3 = 0;
	cantBalas.resize(4);
	balasTotales = 0;
	hayEnemigo = false;
}

void Parser::parsear(string msj,int jugadores){
	cantJugadores = jugadores;
	resetearBalas();

	nivel = msj[0] - '0';
	posFondo1 = stoi(msj.substr(1,4));
	posFondo2 = stoi(msj.substr(5,4));
	posFondo3 = stoi(msj.substr(9,4));
	
	parsearPersonaje(msj,13,1);

	if(cantJugadores >= 2){
		parsearPersonaje(msj,27,2);		
	}

	if(cantJugadores >= 3){
		parsearPersonaje(msj,41,3);		
	}

	if(cantJugadores == 4){
		parsearPersonaje(msj,55,4);		
	}

	//falta cambiar de 1 cantidad a 4
	for(unsigned i = 0; i < cantBalas.size(); i++){
		cantBalas[i] = stoi(msj.substr(13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * i,MENSAJE_CANT_BALAS));
		balasTotales += cantBalas[i];
	}
	//cantBalas = stoi(msj.substr(13 + RESPUESTA_PERSONAJE * cantJugadores,2));

	parsearBalas(msj.substr(13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4,TAMANIO_POS_BALAS * balasTotales));

	hayEnemigo = (msj[13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4 + TAMANIO_POS_BALAS * MAX_BALAS] == '1');
	parsearEnemigos(msj);
	parsearItems(msj);
	parsearBalasEnemigas(msj);
	parsearVidas(msj);
}

void Parser::parsearEnemigos(string msj){
	
	int i = 13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4 + TAMANIO_POS_BALAS * MAX_BALAS + RESPUESTA_ENEMIGO;
	int tamanioMsjEnemigo = 1 + RESPUESTA_POSY * 2;
	unsigned contador = 0;
	vEnemigos.clear();
	
	while(contador < MAX_ENEMIGOS_EN_ESCENA && msj[i] != '0'){
		DatosEnemigo* datosE = new DatosEnemigo();
		datosE->setPosX(dameElInt(msj.substr(i + contador * tamanioMsjEnemigo + 1,RESPUESTA_POSY)));
		datosE->setPosY(dameElInt(msj.substr(i + contador * tamanioMsjEnemigo + 5,RESPUESTA_POSY)));

		datosE->setTipoEnemigo((Constantes::TipoEnemigo)(msj[i] - '0'));
		vEnemigos.push_back(datosE);	
		i += tamanioMsjEnemigo;
	}
}

void Parser::parsearItems(string msj){
	
	int i = 13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4 + TAMANIO_POS_BALAS * MAX_BALAS + RESPUESTA_ENEMIGO + MENSAJE_ENEMIGOS;
	int tamanioMsjItem = 1 + RESPUESTA_POSY * 2;
	unsigned contador = 0;
	vItems.clear();

	while(contador < MAX_ITEMS_EN_ESCENA && msj[i] != '0'){
		DatosItem* datosI = new DatosItem();
		datosI->setPosX(dameElInt(msj.substr(i + contador * tamanioMsjItem + 1,RESPUESTA_POSY)));
		datosI->setPosY(dameElInt(msj.substr(i + contador * tamanioMsjItem + 5,RESPUESTA_POSY)));

		switch(msj[i]){
			case ('1'):
				datosI->setTipoItem(Constantes::vidaExtra);
				break;
			case ('2'):
				datosI->setTipoItem(Constantes::ametralladora);
				break;
			case ('3'):
				datosI->setTipoItem(Constantes::escopeta);
				break;
			case ('4'):
				datosI->setTipoItem(Constantes::bazooka);
				break;
			default:
				cout<<"No conozco ese tipo de Item"<<endl;
				break;
		}		
		
		vItems.push_back(datosI);	
		i += tamanioMsjItem;
	}
}

void Parser::parsearBalasEnemigas(string msj){
	
	int i = 13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4 + TAMANIO_POS_BALAS * MAX_BALAS + RESPUESTA_ENEMIGO + MENSAJE_ENEMIGOS + MENSAJE_ITEMS;

	int tamanioMsj = 1 + RESPUESTA_POSY * 2;
	unsigned contador = 0;
	vBalasEnemigas.clear();

	while(contador < MAX_BALAS_ENEMIGAS && msj[i] != '0'){
		DatosBalaEnemiga* datos = new DatosBalaEnemiga();

		//cout<<msj.substr(i + contador * tamanioMsj + 5,RESPUESTA_POSY)<<endl;
		//cout<<msj.substr(i + contador * tamanioMsj + 1,RESPUESTA_POSY)<<endl;
		datos->setPosX(dameElInt(msj.substr(i + contador * tamanioMsj + 1,RESPUESTA_POSY)));
		datos->setPosY(dameElInt(msj.substr(i + contador * tamanioMsj + 5,RESPUESTA_POSY)));
		datos->setTipoArma((Constantes::TipoArma) (msj[i] - '0'));
		cout<<msj[i]<<endl;

		/*switch(msj[i]){
			case ('1'):
				datos->setTipoArma(Constantes::normal);
				break;
			default:
				cout<<"No conozco ese tipo de arma"<<endl;
				break;
		}*/		
		
		vBalasEnemigas.push_back(datos);	
		i += tamanioMsj;
	}
}

void Parser::parsearVidas(string msj){

	int i = 13 + RESPUESTA_PERSONAJE * cantJugadores + MENSAJE_CANT_BALAS * 4 + TAMANIO_POS_BALAS * MAX_BALAS + RESPUESTA_ENEMIGO + MENSAJE_ENEMIGOS + MENSAJE_ITEMS + MENSAJE_BALAS_ENEMIGAS;

	vidas.clear();
	vidas.push_back(msj[i] - '0');
	vidas.push_back(msj[i+1] - '0');
	vidas.push_back(msj[i+2] - '0');
	vidas.push_back(msj[i+3] - '0');
	
}

DatosPersonaje* Parser::dameAlBobyNumero(int numeroDePersonaje){
	DatosPersonaje* datosP;
	switch(numeroDePersonaje){
		case (1):
			datosP = &datosBoby;
			break;
		case (2):
			datosP = &datosBoby2;
			break;
		case (3):
			datosP = &datosBoby3;
			break;
		case (4):
			datosP = &datosBoby4;
			break;
		default:
			break;
	}

	return datosP;
}


void Parser::parsearPersonaje(string msj,int i,int numeroDePersonaje){

	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	
	datosP->setPosX(stoi(msj.substr(i,3)));
	datosP->setPosY(parsearPosY(msj.substr(i+3,4)));

	datosP->setSaltando(msj[i+7] == '1');
	datosP->setDisparando(msj[i+8] == '1');
	datosP->setMirandoALaDerecha(msj[i+9] == '1');
	datosP->setActivo(msj[i+10] == '1');
	datosP->setGrisado(msj[i+11] == '1');

	datosP->setEstado((Constantes::Estado) (msj[i+12] - '0'));
	datosP->setDireccionDisparo((Constantes::DireccionDisparo) (msj[i+13] - '0'));
}

int Parser::parsearPosY(string substr){
	int respuesta;
	unsigned pos = substr.find_first_of("-");
	if(pos > 3) respuesta = stoi(substr);
	else{
		respuesta = stoi(substr.substr(pos,4-pos));
	}

	return respuesta;
}

int Parser::dameElInt(string sub){
	int respuesta;
	unsigned pos = sub.find_first_of("-");
	if(pos > 3) respuesta = stoi(sub);
	else{
		respuesta = stoi(sub.substr(pos,4-pos));
	}
	return respuesta;	
}

void Parser::parsearBalas(string sub){
	int x,y;
	for(int i = 0; i < balasTotales; i++){
		int tipo = stoi(sub.substr(TAMANIO_POS_BALAS * i,TIPO_BALA));
		x = dameElInt(sub.substr(TAMANIO_POS_BALAS * i + TIPO_BALA,4));
		y = dameElInt(sub.substr(TAMANIO_POS_BALAS * i + TIPO_BALA + 4,4));
		pair< int,pair<int,int> > nuevaBala;
		nuevaBala.first = tipo;
		nuevaBala.second.first = x;
		nuevaBala.second.second = y;
		balas.push_back(nuevaBala);
	}
}

void Parser::resetearBalas(){
	balas.clear();
	balasTotales = 0;
}

int Parser::getNivel(){
	return nivel;
}

int Parser::getPosFondo1(){
	return posFondo1;
}

int Parser::getPosFondo2(){
	return posFondo2;
}

int Parser::getPosFondo3(){
	return posFondo3;
}

int Parser::getPosPersonajeX(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getPosX();
}

int Parser::getPosPersonajeY(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getPosY();
}

bool Parser::estaSaltando(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaSaltando();
}

bool Parser::estaDisparando(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaDisparando();
}

bool Parser::estaMirandoALaDerecha(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaMirandoALaDerecha();
}

bool Parser::estaActivo(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaActivo();
}

bool Parser::estaGrisado(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->estaGrisado();
}

Constantes::Estado Parser::getEstado(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getEstado();
}

Constantes::DireccionDisparo Parser::getDireccionDisparo(int numeroDePersonaje){
	DatosPersonaje* datosP = dameAlBobyNumero(numeroDePersonaje);
	return datosP->getDireccionDisparo();
}

bool Parser::estaElEnemigo(){
	return hayEnemigo;
}

vector<DatosEnemigo*> Parser::getEnemigos(){
	return vEnemigos;
}

vector<DatosItem*> Parser::getItems(){
	return vItems;
}

vector<DatosBalaEnemiga*> Parser::getBalasEnemigas(){
	return vBalasEnemigas;
}

vector<int> Parser::getCantBalas(){
	return cantBalas;
}

vector< pair< int,pair<int,int> > > Parser::getBalas(){
	return balas;
}

int Parser::getVidaPersonaje(int pj){
	return vidas[pj-1];
}

Parser::~Parser() {
}

