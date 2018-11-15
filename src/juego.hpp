#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "temporizador.hpp"
#include "plataformas.hpp"
#include "Scrolleables.hpp"
#include "EntidadDibujable.h"
#include "Personaje.h"
#include "Enemigo.h"
#include "Bullet.h"
#include <vector>
#include "iocontra.hpp"
#include "traductorDelCliente.h"
#include "ArmadorDeRespuesta.h"
#include "Constantes.h"
#include "contenedor_principal.hpp"

using namespace std;

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <stdlib.h>
}

class juego: public contenedor_principal
{
public:
	juego (ventana &v, int cantidadJugadores);
	~juego ();
	void setAcciones(char* msj, int numeroCliente);
	int getCantidadDeBalas();
	void manejar_eventos ();
	void actualizar ();
	string armarRespuesta();
	void grisarJugador(int numeroDeJugador);
	void desgrisarJugador(int numeroDeJugador);
	void dibujar ();
	void presentar ();
	bool jugando ();
protected:
	bool termino, cambioNivel;
	int us, cuadros, d1, d2, d3, nivel, cascada, num_jugadores, coordenada;
	int mundo_w, mundo_h;
	temporizador t_ciclo, t_fps;
	SDL_Rect rect_origen_fondo3;
	SDL_Renderer *renderer;
	SDL_Texture *textura_fondo3, *textura_objetivo, *textura_bala, *textura_bala2, *textura_bala3, *textura_bala4;
	Plataformas plataformas;

	traductorDelCliente* cliente;
	traductorDelCliente* cliente2;
	traductorDelCliente* cliente3;
	traductorDelCliente* cliente4;

	DatosPersonaje datosBoby, datosBoby2, datosBoby3, datosBoby4;
	int maximaCoordenadaJugadores();
	int maximaPosicionJugadores();

	ArmadorDeRespuesta* armador;
	void setDatosEnemigo();
	void actualizarNivel1();
	void actualizarNivel2();
	void actualizarNivel3();

	//balas
	bool collision(SDL_Rect rect1,SDL_Rect rect2);
	int direccionDeBala;

	//probando
	EntidadDibujable fondo1, fondo2;
	Personaje boby, boby2, boby3, boby4;
	Enemigo* enemigoNivel1;
	Enemigo* enemigoNivel2;
	Enemigo* enemigoNivel3;

	vector<Bullet*> balasEnemigas;
	vector<Enemigo*> vectorEnemigos;
	//vector<Item*> vectorItems;

	bool apretandoDerecha(const Uint8* state);
	bool apretandoIzquierda(const Uint8* state);
	bool apretandoArriba(const Uint8* state);
	bool apretandoAbajo(const Uint8* state);
	bool apretandoDisparo(const Uint8* state);
	bool apretandoSalto(const Uint8* state);
	bool apretandoAgacharse(const Uint8* state);
	bool apretandoNivel2(const Uint8* state);
	bool apretandoNivel3(const Uint8* state);
	bool apretandoMultijugador2P(const Uint8* state);
	bool apretandoMultijugador4P(const Uint8* state);
	bool apretandoplayer2izquierda(const Uint8* state);
	bool apretandoplayer2derecha(const Uint8* state);
	bool apretandoplayer2salto(const Uint8* state);
	bool apretandoGrisP2(const Uint8* state);
	bool apretandoDesGrisP2(const Uint8* state);


};

#endif
