#ifndef MENSAJE_CONEXION_HPP
#define MENSAJE_CONEXION_HPP

#include "mensaje.hpp"
#include "login.hpp"

class mensaje_conexion: public mensaje
{
public:
	mensaje_conexion (ventana &v, struct credencial &cred);
	virtual ~mensaje_conexion ();
	virtual void actualizar ();
	void al_terminar_hilo ();
	static void comprobar_credencial (mensaje_conexion *m);
protected:
	std::thread hilo;
	struct credencial &cred;
	int ciclo;
};

#endif

