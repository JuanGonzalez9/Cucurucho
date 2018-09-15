#include "temporizador.hpp"

temporizador::temporizador ()
{
	ahora ();
}

temporizador::~temporizador ()
{
}

void temporizador::ahora ()
{
	struct timezone tz = {0};
	gettimeofday (&comienzo, &tz);
}


int temporizador::milisegundos (bool reiniciar)
{
	struct timeval fin;
	struct timezone tz;
	gettimeofday (&fin, &tz);

	int ms;
	if (comienzo.tv_sec == fin.tv_sec) {
		ms = (fin.tv_usec - comienzo.tv_usec) / 1000;
	} else {  
		ms = (fin.tv_usec + (1000000 - comienzo.tv_usec)) / 1000;
		if (comienzo.tv_sec + 1 < fin.tv_sec) {
			ms += ((fin.tv_sec - comienzo.tv_sec - 1) * 1000);
		}
	}
	if (reiniciar) comienzo = fin;
	return ms;
}

int temporizador::microsegundos (bool reiniciar)
{
	struct timeval fin;
	struct timezone tz;
	gettimeofday (&fin, &tz);

	int us;
	if (comienzo.tv_sec == fin.tv_sec) {
		us = fin.tv_usec - comienzo.tv_usec;
	} else {  
		us = fin.tv_usec + (1000000 - comienzo.tv_usec);
		if (comienzo.tv_sec + 1 < fin.tv_sec) {
			us += ((fin.tv_sec - comienzo.tv_sec - 1) * 1000000);
		}
	}
	if (reiniciar) comienzo = fin;
	return us;
}

