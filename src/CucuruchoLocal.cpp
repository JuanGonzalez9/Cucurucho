#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include "juego.hpp"
#include "JuegoCliente.h"
#include "iocontra.hpp"
#include <iostream>
#include "EscuchadorDeAcciones.h"
#include "Socket.h"
#include "Parser.h"
#include "JuegoCliente.h"
#include "Constantes.h"

#define TAMANIO_MENSAJE_TECLAS 9

void uso ()
{
	std::cout << "uso: contra [-h | -d (ERROR|INFO|DEBUG)]\n\n";
	std::cout << "  -h         Muestra esta ayuda y finaliza\n";
	std::cout << "  -d OPCION  Estable a OPCION como nivel de depuracion\n\n";
}

void enviarBalas(int cantDeBalas,Socket* soquete){
	string balas = "00";
	string nuevo = to_string(cantDeBalas);
	balas.replace(2 - nuevo.size(),nuevo.size(),nuevo);

	soquete->enviar(soquete->getAcceptedSocket(),balas.c_str(),MENSAJE_CANT_BALAS);

	//el resto
}

int main (int argc, char *argv[]){

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

	char* comportamiento = argv[3];
	unsigned short puerto = atoi(argv[4]);
	if(strcmp(comportamiento,"cliente") == 0){

		cout << "Arranca el cliente" << endl;
		Parser parser;
		EscuchadorDeAcciones* escuchador = new EscuchadorDeAcciones();
		const char* serverAdress = "127.0.0.1";
		cout<<"el puerto es "<<puerto<<" y la adress es "<<serverAdress<<endl;

		Socket* soqueteCliente = new Socket();
		soqueteCliente->conectar(serverAdress,puerto);


		char respuestaServidor[TAMANIO_RESPUESTA_SERVIDOR + 1];
		char respuestaCantBalas[MENSAJE_CANT_BALAS + 1];
		JuegoCliente juegoCliente("cliente", 2);

		while(escuchador->enAccion()){
			string acciones = escuchador->obtenerAcciones();
			//if(acciones != "000000000"){
				// mas adelante enviamos solo si esta apretando alguna tecla (si las acciones es distinto de todo 0)
				int enviados = soqueteCliente->enviar(soqueteCliente->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
				if(enviados == -1){
					cout<<"Error en la conexion"<<endl;
				}

				int llegaron = 
					soqueteCliente->recibir(soqueteCliente->getSocketId(),respuestaServidor,TAMANIO_RESPUESTA_SERVIDOR);
				respuestaServidor[TAMANIO_RESPUESTA_SERVIDOR] = 0;
				
				string respuestaSinParsear(respuestaServidor);
				juegoCliente.setMensajeDelServidor(respuestaSinParsear);
	
				juegoCliente.dibujar();
				juegoCliente.presentar();

			//}

		}
		soqueteCliente->~Socket();
		return 0;
	}
	else{

	if(strcmp(comportamiento,"servidor") == 0){
	
		cout<<"Arranca el servidor"<<endl;

		
			char mensaje[TAMANIO_MENSAJE_TECLAS + 1];
			Socket* soquete = new Socket();
			soquete->bindAndListen(puerto);
			soquete->aceptar();

			char mensaje2[TAMANIO_MENSAJE_TECLAS + 1];
			Socket* soquete2 = new Socket();
			soquete2->bindAndListen(puerto+1);
			soquete2->aceptar();


			juego j("servidor", 2);
			while (j.jugando ()) {
				
							


				int recibidos = soquete->recibir(soquete->getAcceptedSocket(),mensaje,TAMANIO_MENSAJE_TECLAS);
				mensaje[TAMANIO_MENSAJE_TECLAS] = 0;
				j.setAcciones(mensaje,1);
				
		
				int recibidos2 = soquete2->recibir(soquete2->getAcceptedSocket(),mensaje2,TAMANIO_MENSAJE_TECLAS);	
				mensaje2[TAMANIO_MENSAJE_TECLAS] = 0;
				j.setAcciones(mensaje2,2);
				

				j.manejar_eventos ();
				j.actualizar ();

				string respuesta = j.armarRespuesta(1);
				soquete->enviar(soquete->getAcceptedSocket(),respuesta.c_str(),TAMANIO_RESPUESTA_SERVIDOR);

				string respuesta2 = j.armarRespuesta(2);
				soquete2->enviar(soquete2->getAcceptedSocket(),respuesta2.c_str(),TAMANIO_RESPUESTA_SERVIDOR);

				//j.dibujar ();
				//j.presentar ();
			}
			soquete->~Socket();
		

		registro.registrar (LogEventos::info, "Finalizo el juego");
		return r ;
	}
	}
	}
}
 



