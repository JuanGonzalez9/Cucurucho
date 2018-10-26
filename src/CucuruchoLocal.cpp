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
	//int cantidadJugadores = cfg.obtener_i("//configuracion//cantidad_jugadores",[](int i, bool omision){return i >= 1 && i <= 4;});
	int cantidadJugadores = 2;
	
	int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;

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


		char respuestaServidor[tamanio_respuestaServidor + 1];
		char respuestaCantBalas[MENSAJE_CANT_BALAS + 1];
		JuegoCliente juegoCliente("cliente", cantidadJugadores);

		while(escuchador->enAccion()){
			string acciones = escuchador->obtenerAcciones();
			//if(acciones != "000000000"){
				// mas adelante enviamos solo si esta apretando alguna tecla (si las acciones es distinto de todo 0)
				int enviados = soqueteCliente->enviar(soqueteCliente->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
				if(enviados == -1){
					cout<<"Error en la conexion"<<endl;
				}

				int llegaron = 
					soqueteCliente->recibir(soqueteCliente->getSocketId(),respuestaServidor,tamanio_respuestaServidor);
				respuestaServidor[tamanio_respuestaServidor] = 0;
				
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
			char mensaje2[TAMANIO_MENSAJE_TECLAS + 1];
			char mensaje3[TAMANIO_MENSAJE_TECLAS + 1];
			char mensaje4[TAMANIO_MENSAJE_TECLAS + 1];

			Socket* soquete = new Socket();
			Socket* soquete2;
			Socket* soquete3;
			Socket* soquete4;


			soquete->bindAndListen(puerto);
			soquete->aceptar();

			
			if (cantidadJugadores >=2){
				soquete2 = new Socket();
				soquete2->bindAndListen(puerto+1);
				soquete2->aceptar();
			}

			if (cantidadJugadores >=3 ){
				soquete3 = new Socket();
				soquete3->bindAndListen(puerto+2);
				soquete3->aceptar();
			}

			if (cantidadJugadores == 4){
				soquete4 = new Socket();
				soquete4->bindAndListen(puerto+3);
				soquete4->aceptar();
			}
			


			juego j("servidor", cantidadJugadores);
			while (j.jugando ()) {							
				int recibidos = soquete->recibir(soquete->getAcceptedSocket(),mensaje,TAMANIO_MENSAJE_TECLAS);
				mensaje[TAMANIO_MENSAJE_TECLAS] = 0;
				j.setAcciones(mensaje,1);
				
				if(cantidadJugadores >= 2){
					int recibidos2 = soquete2->recibir(soquete2->getAcceptedSocket(),mensaje2,TAMANIO_MENSAJE_TECLAS);	
					mensaje2[TAMANIO_MENSAJE_TECLAS] = 0;
					j.setAcciones(mensaje2,2);
				}

				if(cantidadJugadores >= 3){
					int recibidos3 = soquete3->recibir(soquete3->getAcceptedSocket(),mensaje3,TAMANIO_MENSAJE_TECLAS);	
					mensaje3[TAMANIO_MENSAJE_TECLAS] = 0;
					j.setAcciones(mensaje3,3);
				}

				if(cantidadJugadores >= 4){
					int recibidos4 = soquete4->recibir(soquete4->getAcceptedSocket(),mensaje4,TAMANIO_MENSAJE_TECLAS);	
					mensaje4[TAMANIO_MENSAJE_TECLAS] = 0;
					j.setAcciones(mensaje4,4);
				}
				

				j.manejar_eventos ();
				j.actualizar ();

				string respuesta = j.armarRespuesta();
				soquete->enviar(soquete->getAcceptedSocket(),respuesta.c_str(),tamanio_respuestaServidor);

				if(cantidadJugadores >= 2){
					string respuesta2 = j.armarRespuesta();
					soquete2->enviar(soquete2->getAcceptedSocket(),respuesta2.c_str(),tamanio_respuestaServidor);
				}

				if(cantidadJugadores >= 3){
					string respuesta3 = j.armarRespuesta();
					soquete3->enviar(soquete3->getAcceptedSocket(),respuesta3.c_str(),tamanio_respuestaServidor);
				}

				if(cantidadJugadores == 4){
					string respuesta4 = j.armarRespuesta();
					soquete4->enviar(soquete4->getAcceptedSocket(),respuesta4.c_str(),tamanio_respuestaServidor);
				}

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
 



