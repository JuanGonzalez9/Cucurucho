#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "Juego.h"
#include "juego.hpp"
#include "iocontra.hpp"

//esto va en el archivo de configuracion
int posVentanaX = SDL_WINDOWPOS_CENTERED;
int posVentanaY = SDL_WINDOWPOS_CENTERED;
int altoVentana = 600;
int anchoVentana = 800;

int main (int argc, char *argv[]){
	registro.registrar(LogEventos::info,"Comenzo el juego");
	
	juego j;
	while (j.jugando()) {
		j.manejar_eventos ();
		j.actualizar ();
		j.dibujar ();
		j.presentar ();
	}
	return 0;
	
	 Juego* miJuego = new Juego();
	 miJuego->inicializar("CONTRA",posVentanaX,posVentanaY,anchoVentana,altoVentana);

	 while(miJuego->jugando()){
		 miJuego->manejarEventos();
		 miJuego->actualizar();
		 miJuego->renderizar();

		 SDL_Delay(70);
	 }

	 //destruyo las clases
	 miJuego->limpiar();

	registro.registrar(LogEventos::info,"Finalizo el juego");
	return 0;
}
 



