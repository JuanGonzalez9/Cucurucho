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
using namespace std;


class Sprite {
public:
	Sprite();
	virtual ~Sprite();

	enum Estado{
		Quieto,
		Caminando,
		Saltando
	};

	SDL_Rect getFrameCaminando();
	SDL_Rect getFrameQuieto();
	SDL_Rect getFrameSaltando();
private:
	vector<SDL_Rect> framesCaminando;
	vector<SDL_Rect> framesSaltando;

	int cantFramesCaminando;
	int cantFramesSaltando;
	int contador;
	int frameActual;
	Sprite::Estado ultimoFrame;

	void cargarFramesCaminando();
	void cargarFramesSaltando();

};

#endif /* SPRITE_H_ */
