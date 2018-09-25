/*
 * Sprite.h
 *
 *  Created on: Sep 19, 2018
 *      Author: juan
 */

#ifndef SPRITE_H_
#define SPRITE_H_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <vector>
#include <iostream>
using namespace std;


class Sprite {
public:
	Sprite();
	virtual ~Sprite();

	enum Estado{
		Quieto,
		Caminando,
		Saltando,
		Agachado,
		Disparando,
		DisparandoCaminando,
		DisparandoArriba,
		DisparandoAbajo,
		DisparandoArribaCaminando,
		DisparandoAbajoCaminando
	};

	SDL_Rect getFrameCaminando();
	SDL_Rect getFrameQuieto();
	SDL_Rect getFrameSaltando();
	SDL_Rect getFrameDisparando();
	SDL_Rect getFrameDisparandoArriba();
	SDL_Rect getFrameDisparandoAbajo();
	SDL_Rect getFrameDisparandoCaminando();
	SDL_Rect getFrameDisparandoArribaCaminando();
	SDL_Rect getFrameDisparandoAbajoCaminando();
	SDL_Rect getFrameAgachado();
private:
	vector<SDL_Rect> framesCaminando;
	vector<SDL_Rect> framesSaltando;
	vector<SDL_Rect> framesDisparando;
	vector<SDL_Rect> framesDisparandoCaminando;
	vector<SDL_Rect> framesDisparandoArribaCaminando;
	vector<SDL_Rect> framesDisparandoAbajoCaminando;
	vector<SDL_Rect> framesAgachado;

	int cantFramesCaminando;
	int cantFramesSaltando;
	int cantFramesDisparando;
	int cantFramesDisparandoCaminando;
	int cantFramesDisparandoArribaCaminando;
	int cantFramesDisparandoAbajoCaminando;
	int cantFramesAgachado;
	int contador;
	int frameActual;
	Sprite::Estado ultimoFrame;

	void cargarFramesCaminando();
	void cargarFramesSaltando();
	void cargarFramesDisparando();
	void cargarFramesDisparandoCaminando();
	void cargarFramesDisparandoArribaCaminando();
	void cargarFramesDisparandoAbajoCaminando();
	void cargarFramesAgachado();

};

#endif /* SPRITE_H_ */
