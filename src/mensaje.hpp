#ifndef MENSAJE_HPP
#define MENSAJE_HPP

#include <list>
#include "etiqueta.hpp"
#include "panel.hpp"
#include "dialogo.hpp"

extern "C"
{
}

class mensaje: public dialogo
{
public:
	mensaje (ventana &v);
	virtual ~mensaje ();
	virtual void correr (const std::string &msj);
	virtual void manejar_eventos ();
	virtual bool manejar_evento (SDL_Event e);
	virtual void actualizar ();
	virtual void dibujar ();
protected:
	panel pnl;
	std::list<etiqueta*> etiquetas;
	void vaciar ();
};

#endif
