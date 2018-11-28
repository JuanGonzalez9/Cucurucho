#include "alien.hpp"
#include "juego.hpp"
#include "Utils.h"
#include "Constantes.h"
#include <cmath>

static const int ancho = 148;
static const float v0 = -20.0;
static const float aceleracion = 0.9;

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
			break;
		case caminando:
			break;
		case saltando:
			velocidad = v0;
			tmp = rectDestino.y;
			altura = rectDestino.y;
			break;
		case cayendo:
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
	if (sentido > 0 && rectDestino.x >= 600) {
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
	altura += velocidad;
	rectDestino.y = roundf (altura);
	velocidad += aceleracion;
	if (velocidad >= 0.0) {
		cambiar_estado(cayendo);
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
