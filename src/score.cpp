#include <iostream>
#include <sstream>
#include "score.hpp"

static const int fs = 18;  // font size
static const int pl = 10;  // primera linea
static const int sl = 33;  // separacion lineas
static const int ac = 130; // ancho columna
//#define av (40 + (jugadores+(jugadores < 4 ? 2 : 1))*ac)
#define av ((jugadores < 4 ? (jugadores == 1 ? 100 : 80) : 40) + (jugadores+1)*ac)
#define px ((av-(jugadores+1)*ac)/2)
static SDL_Color color_titulo = {140, 140, 140, alpha};
static SDL_Color color_principal = {240, 240, 140, alpha};
static SDL_Color color_normal = {170, 170, 170, alpha};

columna_score::columna_score ():
	contenedor(0, 0),
	e0(x, y, fs),
	e1(x, y + sl*1, fs),
	e2(x, y + sl*2, fs),
	e3(x, y + sl*3, fs),
	e4(x, y + sl*4, fs)
{
	w = ac;
	h = sl*5;
	agregar (&e0, false);
	agregar (&e1, false);
	agregar (&e2, false);
	agregar (&e3, false);
	agregar (&e4, false);
	e0.color (color_titulo);
}

void columna_score::color (const SDL_Color &color)
{
	e1.color (color);
	e2.color (color);
	e3.color (color);
	e4.color (color);
}

void columna_score::centrar ()
{
	e0.anclado_x = control::medio;
	e1.anclado_x = control::medio;
	e2.anclado_x = control::medio;
	e3.anclado_x = control::medio;
	e4.anclado_x = control::medio;
}

void columna_score::mover (int x, int y)
{
	this->x = x;
	this->y = y;
}

void columna_score::texto (const char *t0, const char *t1, const char *t2, const char *t3, const char *t4)
{
	e0.texto (t0);
	e1.texto (t1);
	e2.texto (t2);
	e3.texto (t3);
	e4.texto (t4);
}

void puntaje::serializar (std::stringstream &ss)
{
	ss << jugador << " " << n[0] << " " << n[1] << " " << n[2] << ";";
}

void puntaje::desserializar (std::stringstream &ss)
{
	std::getline(ss, jugador, ' ');
	ss >> n[0];
	ss >> n[1];
	ss >> n[2];
	char c;
	ss >> c;
	// std::cout << jugador << " " << n[0] << " " << n[1] << " " << n[2] << "\n";
}

puntajes::puntajes(int jugadores):
	jugadores(jugadores)
{
	for (int i = 0; i < jugadores; i++) {
		for (int j = 0; j < 3; j++) {
			pts[i].n[j] = -1;
		}
	}
}

puntajes::~puntajes ()
{
}

void puntajes::serializar (std::stringstream &ss)
{
	for (int i = 0; i < jugadores; i++) {
		pts[i].serializar(ss);
	}
}

void puntajes::desserializar (std::stringstream &ss)
{
	for (int i = 0; i < jugadores; i++) {
		pts[i].desserializar(ss);
	}
}

void puntajes::nombre (int orden, const std::string &jugador)
{
	pts[orden].jugador = jugador;
	pts[orden].jugador[0] = toupper (pts[orden].jugador[0]);
}

void puntajes::puntos (int orden, int nivel, int puntos)
{
	pts[orden].n[nivel] = puntos;
}

score::score (ventana &v, int orden, int jugadores):
	dialogo (v),
	puntajes (jugadores),
	pnl (180, 200, av, pl+sl*5+20),
	aceptar (10, 10, "Cerrar")
{
	pnl.anclado_x = control::medio;
	pnl.anclado_y = control::medio;
	agregar (&pnl, false);

	aceptar.anclado_x = control::opuesto;
	aceptar.anclado_y = control::opuesto;
	aceptar.al_presionar_callback = [this]() {al_aceptar();};

	c[0].mover (px, pl);
	c[0].texto ("", "Nivel 1", "Nivel 2",  "Nivel 3", "Total");
	c[0].color (color_titulo);
	pnl.agregar (&c[0], false);

	for (int i = 0; i < jugadores; i++) {
		c[i+1].centrar ();
		c[i+1].mover (px + (i+1)*ac, pl);
		pnl.agregar (&c[i+1], false);
	}
	
	resaltar (orden);
}

score::~score()
{
}

void score::resaltar (int jugador)
{
	int n = 2;
	for (int i = 1; i < 5; i++) {
		if (i-1 == jugador) {
			c[i].color (color_principal);
			c[i].mover (px + ac, pl);
		} else {
			c[i].color (color_normal);
			c[i].mover (px + n*ac, pl);
			n++;
		}
	}
}

void score::desserializar (std::stringstream &ss)
{
	puntajes::desserializar (ss);
	for (int i = 0; i < jugadores; i++) {
		int total = 0;
		for (int j = 0; j < 3; j++) {
			if (pts[i].n[j] > 0) total += pts[i].n[j];
		}
		c[i+1].texto (
			pts[i].jugador.c_str(),
			pts[i].n[0] >= 0 ? std::to_string(pts[i].n[0]).c_str() : "-",
			pts[i].n[1] >= 0 ? std::to_string(pts[i].n[1]).c_str() : "-",
			pts[i].n[2] >= 0 ? std::to_string(pts[i].n[2]).c_str() : "-",
			std::to_string(total).c_str());
	}
}

void score::agregar_boton_aceptar ()
{
	pnl.agregar (&aceptar, false);
	pnl.redimensionar	(av, pl+sl*6+20);
	popular_enfocables ();
}

void score::manejar_eventos()
{
	dialogo::manejar_eventos();
}

bool score::manejar_evento (SDL_Event e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_RETURN:
					al_aceptar ();
					return true;
			};
	}
	return dialogo::manejar_evento (e);
}

void score::al_aceptar ()
{
	corriendo = false;
}

void score::actualizar ()
{
	dialogo::actualizar ();
}

void score::dibujar ()
{
	dialogo::dibujar ();
}

