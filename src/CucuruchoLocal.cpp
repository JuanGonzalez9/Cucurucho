#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include "juego.hpp"
#include "iocontra.hpp"
#include <iostream>
#include "EscuchadorDeAcciones.h"
#include "Socket.h"

#define TAMANIO_MENSAJE_TECLAS 9

void uso ()
{
	std::cout << "uso: contra [-h | -d (ERROR|INFO|DEBUG)]\n\n";
	std::cout << "  -h         Muestra esta ayuda y finaliza\n";
	std::cout << "  -d OPCION  Estable a OPCION como nivel de depuracion\n\n";
}

int main (int argc, char *argv[]){

	char* comportamiento = argv[3];
	unsigned short puerto = atoi(argv[4]);
	if(strcmp(comportamiento,"cliente") == 0){

		cout << "Arranca el cliente" << endl;
		EscuchadorDeAcciones* escuchador = new EscuchadorDeAcciones();
		SDL_Window* ventana = SDL_CreateWindow ("Titulo", 900, 600, 100, 100, 0);
		const char* serverAdress = "127.0.0.1";
		cout<<"el puerto es "<<puerto<<" y la adress es "<<serverAdress<<endl;

		Socket* soqueteCliente = new Socket();
		soqueteCliente->conectar(serverAdress,puerto);

		while(escuchador->enAccion()){
			string acciones = escuchador->obtenerAcciones();
			//if(acciones != "000000000"){
				int enviados = soqueteCliente->enviar(soqueteCliente->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
				cout<<"bytes enviados: "<<enviados<<endl;
				//cout<<"La accion que queres hacer es esta: "<<acciones<<endl;
				// si enviados = -1 error en el servidor
				if(enviados == -1){
					cout<<"Error en la conexion"<<endl;
				}
			//}

			SDL_Delay(20);
		}
		soqueteCliente->~Socket();
		return 0;
	}
	else{

	if(strcmp(comportamiento,"servidor") == 0){
	
		cout<<"Arranca el servidor"<<endl;

		registro.borrarEventos();
		registro.registrar (LogEventos::info, "Comenzo el juego");

		int r = 1;
		if (argc == 5 && argv[1][0] == '-') {
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
			char mensaje[TAMANIO_MENSAJE_TECLAS + 1];
			Socket* soquete = new Socket();
			soquete->bindAndListen(puerto);
			soquete->aceptar();
			juego j;
			while (j.jugando ()) {
				int recibidos = soquete->recibir(soquete->getAcceptedSocket(),mensaje,TAMANIO_MENSAJE_TECLAS);
				mensaje[TAMANIO_MENSAJE_TECLAS] = 0;
				j.setAcciones(mensaje);
				j.manejar_eventos ();
				j.actualizar ();
				j.dibujar ();
				j.presentar ();
			}
			soquete->~Socket();
		}

		registro.registrar (LogEventos::info, "Finalizo el juego");
		return r ;
	}
	}
}
 



