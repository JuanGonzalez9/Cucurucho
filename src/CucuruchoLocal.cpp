#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include "Juego.h"
#include "registro.hpp"
#include "cfg.hpp"

//esto va en el archivo de configuracion
int posVentanaX = SDL_WINDOWPOS_CENTERED;
int posVentanaY = SDL_WINDOWPOS_CENTERED;
int altoVentana = 600;
int anchoVentana = 800;

class Imagen;
void Prueba_en_clase ();

int main (int argc, char *argv[]){
	LogEventos log;
	log.definirTipoLog(LogEventos::debug);
	log.registrar(LogEventos::info,"Comenzo el juego");

	// Obtengo el nivel de depurado almacenado en la clave "//configuracion//debug//level"
	// Primero se valida el dato obtenido en el archivo xml y de fallar se valida la opción por
	// defecto, en este último caso no debería fallar, por lo que si lo hace se produce una excepcion.
	std::string debug_level = cfg.obtener_s ("//configuracion//debug//level", [](std::string s, bool trusted) {
		return s == "ERROR" || s == "INFO" || s == "DEBUG"; // Se valida el dato.
	});

	// Ahora un ejemplo con enteros, obtengo el nivel en el que comenzar.
	int nivel = cfg.obtener_i ("//configuracion//nivel", [](int i, bool trusted) {
		return i >= 1 && i <= 3; // Se valida el dato, esta vez es un entero.
	});
	// Ver la declaración de la clase para más tipos. De ser necesario otros, avisen.

	// Al final de este archivo hay un ejemplo donde se accede a una función miembro para
	// validar los datos de entrada.
	Prueba_en_clase ();
	
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
 
class Imagen
{
public:
	int u;
	Imagen ()
	{
		u = 0 ;
		// Obtengo la imagen de fondo para el nivel 1.
		cfg.obtener_s ("//configuracion//escenarios//nivel1//fondo1", [this](std::string s, bool trusted) {
			// Aca debo verificar que s contenga el camino a una imagen que me sirva.
			// Puedo modificar los miembros de la clase, por lo que podría aprovechar
			// y cargarla completamente.
			u++;
			return true; 
		} );
		// Imprime 1 porque el primer intento de validación (del xml) tuvo exito (ver el return true).
		std::cout << "Numero de validaciones efectuadas: " << u << "\n";
	}
};

void Prueba_en_clase () {
	Imagen i;
}

// Siempre modificar a cfg_defaults.cpp y recompilar, nunca al cfg.xml generado automáticamente.




