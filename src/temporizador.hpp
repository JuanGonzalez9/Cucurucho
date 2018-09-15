#ifndef TEMPORIZADOR_HPP
#define TEMPORIZADOR_HPP

extern "C"
{
	#include <sys/time.h>
}

class temporizador
{
public:
	temporizador ();
	~temporizador ();

	int milisegundos (bool reiniciar = false);
	int microsegundos (bool reiniciar = false);
protected:
	struct timeval comienzo;
	void ahora ();
};

#endif

