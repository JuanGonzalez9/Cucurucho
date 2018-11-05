#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <string>
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class control
{
public:
	control (int x, int y);
	virtual ~control ();
	virtual bool manejar_evento (SDL_Event &e);
	virtual void dibujar (SDL_Renderer *renderer) {}
	virtual void foco (bool enfocado);
	virtual bool foco () const;
	virtual bool enfocable ();
	typedef enum {normal, medio, opuesto} anclado;
	anclado anclado_x, anclado_y;
protected:
	int x, y;
	bool enfocado;
};

#endif
