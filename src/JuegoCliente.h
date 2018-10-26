#ifndef JUEGOCLIENTE_H_
#define JUEGOCLIENTE_H_

#include "Parser.h"
#include "juego.hpp"
#include "Sprite.h"
#include <string>
#include <iostream>

using namespace std;

class JuegoCliente : public juego{
public:
	JuegoCliente(string comportamiento, int cantidadJugadores);
	void setMensajeDelServidor(string msj);
	void dibujar();
	virtual ~JuegoCliente();

private:
	Parser p;
	string msjServer;
	Sprite* miPersonajeSprites;
	int nivel;
	int cantJugadores;

	SDL_Rect bala_rectOrigen;
	SDL_Rect bala_rectDestino;

	void actualizarFondos();
	void actualizarPosicionDeMiPersonaje(int numPersonaje);
	void actualizarEstadoDeMiPersonaje(int numPersonaje);
	void dibujarBalas(vector< pair<int,int> > balas);
};

#endif /* JUEGOCLIENTE_H_ */
