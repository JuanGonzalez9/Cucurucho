#include "alien.hpp"
#include "juego.hpp"
#include "Utils.h"
#include "Constantes.h"
#include <cmath>

static const int ancho = 148;
static const float v0 = -14.0;
static const float aceleracion = 1.2;

alien::alien(juego* j, int x,int y, int vidas, int coordenada, int nivel):
	Enemigo(x, y, vidas, coordenada, nivel),
	j(j),
	contador(1),
	marco(0),
	sentido(1),
	velocidad(v0),
	e(caminando)
{
	obtenerTextura("//configuracion//personajes//alien//sprite", j->dameElRender());
	rectOrigen = {1, 0, ancho, 80};
	rectDestino = {x, y, rectOrigen.w, rectOrigen.h};
	activo = true;
	quieroDisparar = false;
}

alien::~alien()
{
}

void alien::cambiar_estado(estado e)
{
	this->e = e;
	contador = 1;
	marco = 0;
	switch (e) {
		case reptando:
			rectOrigen.x = 1;
			rectOrigen.y = 0;
			rectOrigen.h = 80;
			rectDestino.h = rectOrigen.h;
			break;
		case caminando:
			rectOrigen.x = 1;
			rectOrigen.y = 0;
			rectOrigen.h = 80;
			rectDestino.h = rectOrigen.h;
			break;
		case saltando:
			velocidad = v0;
			tmp = rectDestino.y;
			marco = 4;
			rectOrigen.x = 1360;
			rectOrigen.y = marco*140;
			rectOrigen.h = 140;
			rectDestino.h = rectOrigen.h;
			rectDestino.y += 20;
			altura = rectDestino.y;
			break;
		case cayendo:
			rectOrigen.x = 1;
			rectOrigen.y = 0;
			rectOrigen.h = 80;
			rectDestino.h = rectOrigen.h;
			break;
	};
}

void alien::actualizar()
{
	switch (e) {
		case caminando:
			actualizar_caminando();
			break;
		case reptando:
			actualizar_reptando();
			break;
		case saltando:
			actualizar_saltando();
			break;
		case cayendo:
			actualizar_cayendo();
			break;
	};
}

void alien::actualizar_caminando()
{
	if (contador % 7 == 0) {
		marco++;
		rectOrigen.x = 1 + marco*(ancho + 1);
		if (marco > 7) {
			rectOrigen.x = 1;
			marco = 0;
		}
		if (marco == 0) {
			rectDestino.x += 27*sentido;
		} else if (marco == 1) {
			rectDestino.x += 3*sentido;
		} else if (marco == 2) {
			rectDestino.x += 6*sentido;
		} else if (marco == 3) {
			rectDestino.x += 18*sentido;
		} else if (marco == 4) {
			rectDestino.x += 27*sentido;
		} else if (marco == 5) {
			rectDestino.x += 6*sentido;
		} else if (marco == 6) {
			rectDestino.x += 7*sentido;
		} else if (marco == 7) {
			rectDestino.x += 18*sentido;
		}
	}
	contador++;
	if (sentido > 0 && rectDestino.x >= 400) {
		cambiar_estado(saltando);
	} else if (sentido < 0 && rectDestino.x <= 200) {
		cambiar_estado(saltando);
	}
}

void alien::actualizar_reptando()
{
	if (contador % 7 == 0) {
		marco++;
		rectOrigen.x = 1 + marco*(ancho + 1);
		if (marco > 7) {
			rectOrigen.x = 1;
			marco = 0;
		}
		if (marco == 0) {
			rectDestino.x += 27*sentido;
		} else if (marco == 1) {
			rectDestino.x += 3*sentido;
		} else if (marco == 2) {
			rectDestino.x += 6*sentido;
		} else if (marco == 3) {
			rectDestino.x += 18*sentido;
		} else if (marco == 4) {
			rectDestino.x += 27*sentido;
		} else if (marco == 5) {
			rectDestino.x += 6*sentido;
		} else if (marco == 6) {
			rectDestino.x += 7*sentido;
		} else if (marco == 7) {
			rectDestino.x += 18*sentido;
		}
	}
	contador++;
	if (sentido > 0 && rectDestino.x >= 600) {
		cambiar_estado(saltando);
	} else if (sentido < 0 && rectDestino.x <= 200) {
		cambiar_estado(saltando);
	}
}

void alien::actualizar_saltando()
{
	if ((marco == 1 && contador % 5 == 0) || (marco != 1 && contador % 3 == 0) ) {
		if (marco > 0) {
			marco--;
			rectOrigen.y = 140*marco;
			if (marco == 3) {
				rectDestino.y -= 30;
				rectDestino.x += 6*sentido;
				contador = 0;
			} else if (marco == 2) {
				rectDestino.y -= 26;
				rectDestino.x += 8*sentido;
				contador = 0;
			} else if (marco == 1) {
				rectDestino.y -= 25;
				rectDestino.x -= 9*sentido;
				contador = 0;
			} else if (marco == 0) {
				altura = rectDestino.y;
				contador = 0;
			}
		}
	}
	contador++;
	if (marco == 0) {
		altura += velocidad;
		rectDestino.y = roundf (altura);
		velocidad += aceleracion;
		if (velocidad >= 0.0) {
			cambiar_estado(cayendo);
		}
	}
}

void alien::actualizar_cayendo()
{
	altura += velocidad;
	rectDestino.y = roundf (altura);
	velocidad += aceleracion;
	if (rectDestino.y >= tmp) {
		rectDestino.y = tmp;
		invertir_x = !invertir_x;
		sentido = invertir_x ? -1 : 1;
		cambiar_estado(invertir_x ? reptando : caminando);
	}
}

void alien::hazLoTuyo()
{
	actualizar();
}

std::string alien::serializar(){
	std::string serial = "";
	serial = to_string(Constantes::marcianito);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.x);
	serial += Utils::pasarAStringDeTamanio(RESPUESTA_POSY,rectDestino.y);
	return serial;
}
