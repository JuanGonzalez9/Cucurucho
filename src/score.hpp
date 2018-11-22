#ifndef SCORE_HPP
#define SCORE_HPP

#include <list>
#include "etiqueta.hpp"
#include "panel.hpp"
#include "boton.hpp"
#include "dialogo.hpp"

extern "C"
{
}

class score: public dialogo
{
public:
	score (ventana &v, int jugadores);
	virtual ~score ();
	virtual void manejar_eventos ();
	virtual bool manejar_evento (SDL_Event e);
	virtual void actualizar ();
	virtual void dibujar ();
	virtual void al_aceptar ();
protected:
	panel pnl;
	etiqueta nivel_1, nivel_2, nivel_3, total, jugador_1, jugador_2, jugador_3, jugador_4;
	boton aceptar;
};

#endif

