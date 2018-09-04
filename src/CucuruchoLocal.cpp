#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "Juego.h"
#include "registro.hpp"

//esto va en el archivo de configuracion
int posVentanaX = SDL_WINDOWPOS_CENTERED;
int posVentanaY = SDL_WINDOWPOS_CENTERED;
int altoVentana = 600;
int anchoVentana = 800;

int main (int argc, char *argv[]){

	LogEventos log;
	log.definirTipoLog(LogEventos::debug);
	log.registrar(LogEventos::info,"Comenzo el juego");

	 Juego* miJuego = new Juego();
	 miJuego->inicializar("CONTRA",posVentanaX,posVentanaY,anchoVentana,altoVentana);

	 while(miJuego->jugando()){
		 miJuego->manejarEventos();
		 miJuego->actualizar();
		 miJuego->renderizar();

		 SDL_Delay(60);
	 }

	 //destruyo las clases
	 miJuego->limpiar();

	 return 0;
 }
