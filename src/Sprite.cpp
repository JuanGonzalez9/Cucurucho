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
	cantFramesDisparando = 2;
	cantFramesDisparandoCaminando = 3;
	cantFramesDisparandoArribaCaminando = 3;
	cantFramesDisparandoAbajoCaminando = 3;
	contador = 0;
	frameActual = 0;
	ultimoFrame = Quieto;
	cargarFramesCaminando();
	cargarFramesSaltando();
	cargarFramesDisparando();
	cargarFramesDisparandoCaminando();
	cargarFramesDisparandoArribaCaminando();
	cargarFramesDisparandoAbajoCaminando();

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

SDL_Rect Sprite::getFrameDisparando(){
	if(ultimoFrame != Disparando) frameActual = 0;
	SDL_Rect frame = framesDisparando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesDisparando;
		contador = 0;
	}
	ultimoFrame = Disparando;
	return frame;
}

SDL_Rect Sprite::getFrameDisparandoCaminando(){
	if(ultimoFrame != DisparandoCaminando) frameActual = 0;
	SDL_Rect frame = framesDisparandoCaminando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesDisparandoCaminando;
		contador = 0;
	}
	ultimoFrame = DisparandoCaminando;
	return frame;
}

SDL_Rect Sprite::getFrameDisparandoArribaCaminando(){
	if(ultimoFrame != DisparandoArribaCaminando) frameActual = 0;
	SDL_Rect frame = framesDisparandoArribaCaminando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesDisparandoArribaCaminando;
		contador = 0;
	}
	ultimoFrame = DisparandoArribaCaminando;
	return frame;
}

SDL_Rect Sprite::getFrameDisparandoAbajoCaminando(){
	if(ultimoFrame != DisparandoAbajoCaminando) frameActual = 0;
	SDL_Rect frame = framesDisparandoAbajoCaminando[frameActual];
	contador++;
	if(contador == 10){
		frameActual++;
		frameActual %= cantFramesDisparandoAbajoCaminando;
		contador = 0;
	}
	ultimoFrame = DisparandoAbajoCaminando;
	return frame;
}

SDL_Rect Sprite::getFrameDisparandoArriba(){
	SDL_Rect frameDA = framesDisparandoArribaCaminando[0];
	ultimoFrame = DisparandoArriba;
	return frameDA;
}

SDL_Rect Sprite::getFrameDisparandoAbajo(){
	SDL_Rect frameDAbajo = framesDisparandoAbajoCaminando[0];
	ultimoFrame = DisparandoAbajo;
	return frameDAbajo;
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

void Sprite::cargarFramesDisparando(){
	SDL_Rect disparando = { 0, 7, 24 , 34};
	framesDisparando.push_back(disparando);

	disparando = { 24 , 7 , 25 , 34};
	framesDisparando.push_back(disparando);
}

void Sprite::cargarFramesDisparandoCaminando(){
	SDL_Rect disparandoCaminando = { 0, 78, 24 , 36};
	framesDisparandoCaminando.push_back(disparandoCaminando);

	disparandoCaminando = { 24, 78, 26 , 36};
	framesDisparandoCaminando.push_back(disparandoCaminando);

	disparandoCaminando = { 50, 78, 26 , 36};
	framesDisparandoCaminando.push_back(disparandoCaminando);
}

void Sprite::cargarFramesDisparandoArribaCaminando(){
	SDL_Rect disparandoArribaCaminando = { 0, 149, 17 , 36};
	framesDisparandoArribaCaminando.push_back(disparandoArribaCaminando);

	disparandoArribaCaminando = { 17, 149, 20 , 36};
	framesDisparandoArribaCaminando.push_back(disparandoArribaCaminando);

	disparandoArribaCaminando = { 37, 149 , 21 , 36};
	framesDisparandoArribaCaminando.push_back(disparandoArribaCaminando);
}

void Sprite::cargarFramesDisparandoAbajoCaminando(){
	SDL_Rect dAbajo = { 0, 221, 22 , 36};
	framesDisparandoAbajoCaminando.push_back(dAbajo);

	dAbajo = { 22, 221, 21 , 36};
	framesDisparandoAbajoCaminando.push_back(dAbajo);

	dAbajo = { 43, 221, 22 , 36};
	framesDisparandoAbajoCaminando.push_back(dAbajo);
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

