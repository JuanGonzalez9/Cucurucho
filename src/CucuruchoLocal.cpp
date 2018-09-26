#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include "juego.hpp"
#include "iocontra.hpp"

void uso ()
{
	std::cout << "uso: contra [-h | -d (ERROR|INFO|DEBUG)]\n\n";
	std::cout << "  -h         Muestra esta ayuda y finaliza\n";
	std::cout << "  -d OPCION  Estable a OPCION como nivel de depuracion\n\n";
}

int main (int argc, char *argv[]){
	registro.registrar (LogEventos::info, "Comenzo el juego");

	int r = 1;
	if (argc == 3 && argv[1][0] == '-') {
		if (argv[1][1] == 'd') {
			if (!strcmp (argv[2], "ERROR")) {
				registro.definirTipoLog (LogEventos::error);
				r = 0;
			} else if (!strcmp (argv[2], "INFO")) {
				registro.definirTipoLog (LogEventos::info);
				r = 0;
			} else if (!strcmp (argv[2], "DEBUG")) {
				registro.definirTipoLog (LogEventos::debug);
				r = 0;
			} else {
				std::stringstream ss;
				ss << "Opcion de depurado '" << argv[2] << "' no reconida";
				registro.registrar (LogEventos::error, ss.str().c_str());
			}
		}
	} else {
		r = argc != 1;
	}

	if (r) {
		uso ();
	} else {
		juego j;
		while (j.jugando ()) {
			j.manejar_eventos ();
			j.actualizar ();
			j.dibujar ();
			j.presentar ();
		}
	}

	registro.registrar (LogEventos::info, "Finalizo el juego");
	return r ;
}
 



