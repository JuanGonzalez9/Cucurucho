#ifndef VENTANA_HPP
#define VENTANA_HPP

#include <string>
#include "temporizador.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
}

class ventana
{
public:
	ventana (const std::string &titulo, int ancho, int alto);
	virtual ~ventana ();
	void mostrar ();
	void ocultar ();
	void presentar ();
	void titulo (const char *texto);
	SDL_Renderer *renderer ();
protected:
	SDL_Window *v;
	SDL_Renderer *r;
	int us, cuadros;
	temporizador t_ciclo, t_fps;
};

#endif
