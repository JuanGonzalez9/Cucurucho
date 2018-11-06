#ifndef CONTENEDOR_PRINCIPAL_HPP
#define CONTENEDOR_PRINCIPAL_HPP

#include <list>
#include "ventana.hpp"
#include "contenedor.hpp"

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

	virtual void popular_enfocables ();
	void enfocar (control *c);
	void enfocar_siguiente ();
	void enfocar_anterior ();

protected:
	ventana &v;
	bool corriendo;
	std::list<control*> enfocables;
};

#endif
