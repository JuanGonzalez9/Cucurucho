#ifndef DIALOGO_HPP
#define DIALOGO_HPP

#include "contenedor_principal.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

class dialogo: public contenedor_principal
{
public:
	dialogo (ventana &v);
	virtual ~dialogo ();
	virtual void actualizar();
	virtual void dibujar ();
protected:
	int c, d;
	int ba, bx, bd;
};

#endif
