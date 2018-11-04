#ifndef JUEGOCLIENTE_H_
#define JUEGOCLIENTE_H_

#include "Parser.h"
#include "juego.hpp"
#include "Sprite.h"
#include "editor.hpp"
#include <string>
#include <iostream>

using namespace std;

class JuegoCliente : public juego{
public:
	JuegoCliente(const std::string &titulo, string comportamiento, int cantidadJugadores,int numeroDeJugador);
	void setMensajeDelServidor(string msj);
	void dibujar();
	void manejarEventos();
	virtual ~JuegoCliente();
	typedef enum {Registrandose, Jugando, MostrandoScore} Estado;
	Estado getEstado () const;
	void setEstado (Estado estado);

private:
	Parser p;
	string msjServer;
	int nivel;
	int cantJugadores;
	int numeroDeJugador;
	Estado estado;
	editor usuario;

	SDL_Rect bala_rectOrigen;
	SDL_Rect bala_rectDestino;

	void actualizarFondos();
	void actualizarPosicionDeMiPersonaje(int numPersonaje);
	void actualizarEstadoDeMiPersonaje(int numPersonaje);
	void dibujarBalas(vector< pair<int,int> > balas);
	Personaje* dameAlBobyNumero(int numeroDeBoby);
	void dibujarEnemigo();
	void dibujarJugadores();
	void dibujarLogin();
	void dibujarJuego();
	void dibujarScore();
	void manejarEventosLogin();
	void manejarEventosJuego();
	void manejarEventosScore();
};

#endif /* JUEGOCLIENTE_H_ */
