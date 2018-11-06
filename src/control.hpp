#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include <list>
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class contenedor;

const int alpha = 200;

class control
{
public:
	control (int x, int y);
	virtual ~control ();
	virtual bool manejar_evento (SDL_Event &e);
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer) {}
	virtual void foco (bool enfocado);
	virtual bool foco () const;
	virtual bool enfocable ();
	virtual void popular (std::list<control*> &enfocables);
	void establecer_padre (contenedor *c);
	typedef enum {normal, medio, opuesto} anclado;
	anclado anclado_x, anclado_y;
protected:
	int x, y;
	bool enfocado;
	contenedor *padre;
	SDL_BlendMode blend_mode;
	bool blend_ok;
	void activar_blend (SDL_Renderer *renderer);
	void restaurar_blend (SDL_Renderer *renderer);
};

#endif
