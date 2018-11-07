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
	int c, d, p;
	SDL_Texture *logo_c_textura;
	int logo_c_ancho, logo_c_alto, logo_c_x;
	SDL_Texture *logo_ontra_textura;
	int logo_ontra_ancho, logo_ontra_alto, logo_ontra_x;
	static const int nube_cantidad = 10;
	SDL_Texture *nube_textura[nube_cantidad];
	int nube_ancho[nube_cantidad], nube_alto[nube_cantidad], nube_delta[nube_cantidad],
		nube_x[nube_cantidad], nube_y[nube_cantidad], nube_z[nube_cantidad];
	void dibujar_nubes (bool delante);
};

#endif
