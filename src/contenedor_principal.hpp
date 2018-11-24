#ifndef CONTENEDOR_PRINCIPAL_HPP
#define CONTENEDOR_PRINCIPAL_HPP

#include <list>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "ventana.hpp"
#include "contenedor.hpp"
#include "Musica.h"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

class contenedor_principal: public contenedor
{
public:
	contenedor_principal (ventana &v);
	virtual ~contenedor_principal ();
	virtual void correr ();
	virtual void manejar_eventos ();
	virtual bool manejar_evento (SDL_Event e);
	virtual void actualizar() {}
	virtual void dibujar ();
	virtual void presentar ();
	virtual void popular_enfocables ();
	void sincronizada (std::function<void()> funcion, bool esperar = true);
	void ejecutar_sincronizada ();
	void enfocar (control *c);
	void enfocar_siguiente ();
	void enfocar_anterior ();
	void enfocar (int x, int y);
	void resaltar (int x, int y);

protected:
	ventana &v;
	bool corriendo;
	std::list<control*> enfocables;
	control *control_resaltado;
	std::mutex mutex_sinc;
	std::function<void()> funcion_sinc;
	std::condition_variable condicion_sinc;
	volatile bool ejecutada_sinc;
	Musica* miMusica;
};

#endif
