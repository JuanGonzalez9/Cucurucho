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
	JuegoCliente(string comportamiento);
	void setMensajeDelServidor(string msj);
	void dibujar();
	virtual ~JuegoCliente();

private:
	Parser p;
	string msjServer;
	Sprite* miPersonajeSprites;
	int nivel;

	SDL_Rect pj_rectOrigen;
	SDL_Rect pj_rectDestino;

	void actualizarFondos();
	void actualizarPosicionDeMiPersonaje();
	void actualizarEstadoDeMiPersonaje();
};

#endif /* JUEGOCLIENTE_H_ */
