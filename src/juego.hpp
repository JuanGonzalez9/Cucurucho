#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "temporizador.hpp"
#include "score.hpp"
#include "plataformas.hpp"
#include "EntidadDibujable.h"
#include "Personaje.h"
#include "Enemigo.h"
#include "SubclasesEnemigos.h"
#include "Bullet.h"
#include "Item.hpp"
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
	juego (ventana &v, int cantidadJugadores, puntajes &pts);
	~juego ();
	void setAcciones(char* msj, int numeroCliente);
	void setCantidadDeBalas();
	void manejar_eventos ();
	void actualizar ();
	void setDatosEnemigos();
	void setDatosItems();
	void setDatosBalasEnemigas();
	string armarRespuesta();
	void grisarJugador(int numeroDeJugador);
	void desgrisarJugador(int numeroDeJugador);
	void dibujar ();
	void presentar ();
	bool jugando ();
	void agregarBalaEnemigo(Bullet* nuevaBala);
	SDL_Renderer* dameElRender();
	SDL_Rect rect_origen_fondo3;

	int obtenerPuntaje(int jugador, bool resetear = false);
	void serializar_puntaje (std::stringstream &ss);
	int obtenerCantidadDeEnemigos(int nivel);

protected:
	puntajes &pts;
	bool termino, cambioNivel;
	int us, cuadros, d1, d2, d3, nivel, cascada, num_jugadores, coordenada;
	int mundo_w, mundo_h;
	temporizador t_ciclo, t_fps;
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
	void setDatosEnemigoFinal();
	void actualizarNivel1();
	void actualizarNivel2();
	void actualizarNivel3();

	void cargarEnemigosNivel1(int cantEnemigos);
	void cargarEnemigosNivel2(int cantEnemigos);
	void cargarEnemigosNivel3(int cantEnemigos);

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
	vector<Item*> vectorItems;
	
	//iconos de vidas
	EntidadDibujable vida11, vida12, vida13, vida21, vida22, vida23, vida31, vida32, vida33;
	EntidadDibujable vida41, vida42, vida43, gameover1, gameover2, gameover3, gameover4;

	EntidadDibujable cartelLv1, cartelLv2, cartelLv3;

	void dibujarVidas();

	bool tengoQueEnviarEnemigo(int i);

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
	void sumarChunkDisparo(Personaje::Arma arma);
	
	//God Mode
	bool apretandoGodMode(const Uint8* state);
	bool alguienQuiereGodMode();
	void godModeParaTodos();
	void godModeDesactivadoParaTodos();
	int sePuedePedirGodMode;
	bool algunoEstaEnGodMode();
};

#endif
