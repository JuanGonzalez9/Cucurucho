#ifndef JUEGOCLIENTE_H_
#define JUEGOCLIENTE_H_

#include "Parser.h"
#include "juego.hpp"
#include "contenedor.hpp"
#include "etiqueta.hpp"
#include "Sprite.h"
#include <string>
#include <iostream>

using namespace std;

class JuegoCliente : public juego {
public:
	JuegoCliente(ventana &v, int cantidadJugadores,int numeroDeJugador, puntajes &pts);
	void setMensajeDelServidor(string msj);
	void dibujar();
	void manejarEventos();
	int jugador() const;
	void establecerPuntaje(int puntaje);
	virtual ~JuegoCliente();
private:
	Parser p;
	string msjServer;
	int nivel;
	int cantJugadores;
	int numeroDeJugador;
	etiqueta etiqueta_puntos;

	SDL_Rect bala_rectOrigen;
	SDL_Rect bala_rectDestino;

	void actualizarFondos();
	void actualizarPosicionDeMiPersonaje(int numPersonaje);
	void actualizarEstadoDeMiPersonaje(int numPersonaje);
	void dibujarBalas(vector< pair<int, pair<int,int> > > balas);
	Personaje* dameAlBobyNumero(int numeroDeBoby);
	void dibujarEnemigoFinal();
	void dibujarEnemigos();
	void dibujarJugadores();
	void dibujarItems();
	void dibujarBalasEnemigas();
	void dibujameLasVidas();

	void ajustarTamanioBala(int tipoDeBala);

	vector<DatosEnemigo*> vDatosEnemigo;
	vector<Enemigo*> vEnemigos;

	vector<DatosItem*> vDatosItem;
	vector<Item*> vItems;

	vector<DatosBalaEnemiga*> vDatosBalaEnemiga;
	vector<Bullet*> vBalaEnemiga;
};

#endif /* JUEGOCLIENTE_H_ */
