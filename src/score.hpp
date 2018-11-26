#ifndef SCORE_HPP
#define SCORE_HPP

#include <list>
#include <vector>
#include "etiqueta.hpp"
#include "panel.hpp"
#include "boton.hpp"
#include "dialogo.hpp"

extern "C"
{
}

class puntaje
{
public:
	std::string jugador;
	int n[3];
	void serializar (std::stringstream &ss);
	void desserializar (std::stringstream &ss);
};

class puntajes
{
public:
	puntajes(int jugadores);
	virtual ~puntajes ();
	void nombre (int orden, const std::string &jugador);
	void puntos (int orden, int nivel, int puntos);
	virtual void serializar (std::stringstream &ss);
	virtual void desserializar (std::stringstream &ss);
protected:
	int jugadores;
	puntaje pts[4];
};

class columna_score: public contenedor
{
public:
	columna_score ();
	void color (const SDL_Color &color);
	void centrar ();
	void mover (int x, int y);
	void texto (const char *t0, const char *t1, const char *t2, const char *t3, const char *t4);
	etiqueta e0, e1, e2, e3, e4;
};

class score: public dialogo, public puntajes
{
public:
	score (ventana &v, int orden, int jugadores);
	virtual ~score ();
	virtual void manejar_eventos ();
	virtual bool manejar_evento (SDL_Event e);
	virtual void actualizar ();
	virtual void dibujar ();
	virtual void al_aceptar ();
	virtual void desserializar (std::stringstream &ss);
	void agregar_boton_aceptar ();
	void resaltar (int jugador);
protected:
	panel pnl;
	boton aceptar;
	columna_score c[5];
};

#endif

