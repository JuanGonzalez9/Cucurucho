#ifndef ESCUCHADORDEACCIONES_H_
#define ESCUCHADORDEACCIONES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>

using namespace std;

class EscuchadorDeAcciones {
public:
	EscuchadorDeAcciones();
	string obtenerAcciones();
	bool enAccion();
	virtual ~EscuchadorDeAcciones();

	enum BitAccion{
		derecha,
		izquierda,
		arriba,
		abajo,
		disparo,
		salto,
		agacharse,
		nivel2,
		nivel3
	};

private:
	bitset<9> acciones;
	bool jugando;

	bool apretandoDerecha(const Uint8* state);
	bool apretandoIzquierda(const Uint8* state);
	bool apretandoArriba(const Uint8* state);
	bool apretandoAbajo(const Uint8* state);
	bool apretandoDisparo(const Uint8* state);
	bool apretandoSalto(const Uint8* state);
	bool apretandoAgacharse(const Uint8* state);
	bool apretandoNivel2(const Uint8* state);
	bool apretandoNivel3(const Uint8* state);
};

#endif /* ESCUCHADORDEACCIONES_H_ */
