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
#include <thread>
#include <mutex>
#include "login.hpp"
 
#define TAMANIO_MENSAJE_TECLAS 9
 
void funcionrecibir(int numeroCliente, Socket* socket, juego* j, autenticados *a  );
void funcionEnviar(string respuesta, Socket* socket, juego* j, int  tamanio_respuestaServidor );
 
 
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
 
	int fd, cantidadJugadores;
	ventana_login ventana;
	if (!login (puerto, fd, cantidadJugadores, ventana)) {
		return 0;
	}
	int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;
	std::cout << "Cliente: fd: " << fd << ", jugadores: " << cantidadJugadores << "\n";

        Socket* soqueteCliente = new Socket(fd);
        //soqueteCliente->conectar(serverAdress,puerto);
  	std::cout << "Iniciando cliente: " << soqueteCliente->getSocketId() << "\n";

 
        char respuestaServidor[tamanio_respuestaServidor + 1];
        char respuestaCantBalas[MENSAJE_CANT_BALAS + 1];
        JuegoCliente juegoCliente("cliente", cantidadJugadores);

        while(escuchador->enAccion() && juegoCliente.jugando()){
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
		juegoCliente.manejarCierre();
 
            //}
 
        }
        soqueteCliente->~Socket();
        return 0;
    }
    else{
 
    if(strcmp(comportamiento,"servidor") == 0){
   
        cout<<"Arranca el servidor"<<endl;

	int cantidadJugadores = cfg.obtener_i("//configuracion//cantidad_jugadores",[](int i, bool omision){return i >= 1 && i <= 4;});
	int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;
	autenticados a;
	esperar_jugadores (cantidadJugadores, puerto, a);
 
	Socket* soquete = new Socket(a.usuarios[0].fd);
	soquete->setConexion(true);
	Socket* soquete2;
	Socket* soquete3;
	Socket* soquete4;

	/*
	soquete->bindAndListen(puerto);
	soquete->aceptar();
	*/

	std::thread tenviar1;
	std::thread trecibir1;
	std::thread tenviar2;
	std::thread trecibir2;
	std::thread tenviar3;
	std::thread trecibir3;
	std::thread tenviar4;
	std::thread trecibir4;

	if (cantidadJugadores >=2){
	soquete2 = new Socket(a.usuarios[1].fd);
	soquete2->setConexion(true);
	/*
	soquete2->bindAndListen(puerto+1);
	soquete2->aceptar();
	*/
	}

	if (cantidadJugadores >=3 ){
	soquete3 = new Socket(a.usuarios[2].fd);
	soquete3->setConexion(true);
	/*
	soquete3->bindAndListen(puerto+2);
	soquete3->aceptar();
	*/

	}

	if (cantidadJugadores == 4){
	soquete4 = new Socket(a.usuarios[3].fd);
	soquete4->setConexion(true);
	/*
	soquete4->bindAndListen(puerto+3);
	soquete4->aceptar();
	*/
	}
           
 	std::cout << "Iniciando ciclo\n";
 
	juego j("servidor", cantidadJugadores);
	while (j.jugando ()) {                         
		trecibir1 = std::thread{std::bind(funcionrecibir, 1, soquete, &j, &a)};

		if(cantidadJugadores >= 2){
			trecibir2 = std::thread{std::bind(funcionrecibir, 2, soquete2, &j, &a)};
		}

		if(cantidadJugadores >= 3){
			trecibir3 = std::thread{std::bind(funcionrecibir, 3 , soquete3, &j, &a)};
		}

		if(cantidadJugadores >= 4){
			trecibir4 = std::thread{std::bind(funcionrecibir, 4, soquete4, &j, &a)};
		}

		trecibir1.join();
		if(cantidadJugadores >= 2){
			trecibir2.join();
		}

		if(cantidadJugadores >= 3){
			trecibir3.join();
		}

		if(cantidadJugadores >= 4){
			trecibir4.join();
		}
		       
		j.manejar_eventos ();
		j.actualizar ();
		string respuesta = j.armarRespuesta();

		tenviar1 = std::thread{std::bind(funcionEnviar, respuesta, soquete, &j, tamanio_respuestaServidor)};

		if(cantidadJugadores >= 2){
		    tenviar2 = std::thread{std::bind(funcionEnviar, respuesta, soquete2, &j, tamanio_respuestaServidor)};
		}

		if(cantidadJugadores >= 3){
		    tenviar3 = std::thread{std::bind(funcionEnviar, respuesta, soquete3, &j, tamanio_respuestaServidor)};
		}

		if(cantidadJugadores == 4){
		    tenviar4 = std::thread{std::bind(funcionEnviar, respuesta , soquete4, &j, tamanio_respuestaServidor)};
		}



		tenviar1.join();
		if(cantidadJugadores >= 2){
			tenviar2.join();
		}

		if(cantidadJugadores >= 3){
			tenviar3.join();
		}

		if(cantidadJugadores >= 4){
			tenviar4.join();
		}
		//j.dibujar();
		//j.presentar();
	}
	soquete->~Socket();
       
 
        registro.registrar (LogEventos::info, "Finalizo el juego");
        return r ;
    }
    }
    }
}
 
 
void funcionrecibir(int numeroCliente, Socket* socket, juego* j, autenticados *a  ){
 
    	char mensaje[TAMANIO_MENSAJE_TECLAS + 1] = "000000000";
   
	static std::mutex m;
 
	m.lock();
	if(!socket->estaConectado()){
		std::lock_guard<std::mutex> lock(a->mutex);
		if (a->usuarios[numeroCliente-1].fd != -1) {
			std::cout << "Reconectando cliente: " << numeroCliente << "\n";
			socket->setSocketId(a->usuarios[numeroCliente-1].fd);
			j->desgrisarJugador(numeroCliente);
			socket->setConexion(true);
		}
	}
	
	if(socket->estaConectado()){
		int recibidos = socket->recibir(socket->getAcceptedSocket(),mensaje,TAMANIO_MENSAJE_TECLAS);
		if(recibidos == -1){
			std::lock_guard<std::mutex> lock(a->mutex);
			socket->setConexion(false);
			j->grisarJugador(numeroCliente);
			std::cout << "Anulando cliente: " << numeroCliente << "\n";
			a->usuarios[numeroCliente-1].fd = -1;
		} 
	}
	mensaje[TAMANIO_MENSAJE_TECLAS] = 0;
	j->setAcciones(mensaje,numeroCliente);
	m.unlock();
}
 
void funcionEnviar(string respuesta, Socket* socket, juego* j, int tamanio_respuestaServidor){
 
	static std::mutex m;

	if(socket->estaConectado()){
		m.lock();
		socket->enviar(socket->getAcceptedSocket(),respuesta.c_str(),tamanio_respuestaServidor);
		m.unlock();
	}
	
}



