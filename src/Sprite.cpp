/*
 * Sprite.cpp
 *
 *  Created on: Sep 19, 2018
 *      Author: juan
 */

#include "Sprite.h"

Sprite::Sprite() {
	cantFramesCaminando = 6;
	cantFramesSaltando = 4;
	contador = 0;
	frameActual = 0;
	ultimoFrame = Quieto;
	cargarFramesCaminando();
	cargarFramesSaltando();




}

SDL_Rect Sprite::getFrameCaminando(){
	if(ultimoFrame != Caminando) frameActual = 0;
	SDL_Rect frame = framesCaminando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesCaminando;
		contador = 0;
	}
	ultimoFrame = Caminando;
	return frame;
}

SDL_Rect Sprite::getFrameQuieto(){
	SDL_Rect frameQuieto = { 0, 7, 24 , 34};
	ultimoFrame = Quieto;
	return frameQuieto;
}

SDL_Rect Sprite::getFrameSaltando(){
	if(ultimoFrame != Saltando) frameActual = 0;
	SDL_Rect frame = framesSaltando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesSaltando;
		contador = 0;
	}
	ultimoFrame = Saltando;
	return frame;
}

void Sprite::cargarFramesCaminando(){
	SDL_Rect caminando = {0, 44 , 18 , 34 };
	framesCaminando.push_back(caminando);

	caminando = {18, 44, 19, 34};
	framesCaminando.push_back(caminando);

	caminando = {37, 44, 22, 34};
	framesCaminando.push_back(caminando);

	caminando = {58, 44, 18, 34};
	framesCaminando.push_back(caminando);

	caminando = {75, 44, 20, 34};
	framesCaminando.push_back(caminando);

	caminando = {95, 44, 20, 34};
	framesCaminando.push_back(caminando);
}

void Sprite::cargarFramesSaltando(){
	SDL_Rect saltando = {116, 44 , 18 , 34 };
	framesSaltando.push_back(saltando);

	saltando = {136, 44 , 21, 34 };
	framesSaltando.push_back(saltando);

	saltando = {156, 44 , 18, 34 };
	framesSaltando.push_back(saltando);

	saltando = {156, 44 , 18, 34 };
	framesSaltando.push_back(saltando);
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

