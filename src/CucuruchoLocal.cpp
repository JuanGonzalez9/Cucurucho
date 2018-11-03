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
#include <signal.h>
 
void funcionrecibir(int numeroCliente, Socket* socket, juego* j, autenticados *a  );
void funcionEnviar(string respuesta, Socket* socket, juego* j, int  tamanio_respuestaServidor );

volatile bool salir = false; 
autenticados *pa = nullptr;
void al_interrumpir (int s)
{
	if (pa && s == SIGINT) {
		std::cout << "Interrupcion manejada por el servidor\n";
		salir = true;
		std::unique_lock<std::mutex> lock(pa->mutex);
		pa->cantidad = pa->requeridos; // Fuerzo condicion solo para que salga
		pa->condicion.notify_one();
	}
}
 
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

void comunicar_cliente (autenticados *a, int i, int tamanio_respuestaServidor);
 
int main (int argc, char *argv[]){
 
	registro.borrarEventos();
	registro.registrar (LogEventos::info, "Comenzo el juego");
   
	int r = 1;
	if (argc >= 5 && argv[1][0] == '-') {
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
	
	const char* serverAdress = "127.0.0.1";
	if (argc == 6) {
		serverAdress = argv[5];
	}
	
	char* comportamiento = argv[3];
	unsigned short puerto = atoi(argv[4]);
	if(strcmp(comportamiento,"cliente") == 0){

        cout << "Arranca el cliente en " << serverAdress << ":" << puerto << "\n";
        Parser parser;
        EscuchadorDeAcciones* escuchador = new EscuchadorDeAcciones();

	ventana_login ventana;
	if (!login (serverAdress, puerto, ventana)) {
		return 0;
	}
	int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * ventana.jugadores;
	std::cout << "Cliente: fd: " << ventana.fd << ", jugadores: " << ventana.jugadores << "\n";

	std::stringstream titulo;
	titulo << ventana.usuario << "(" << ventana.orden << ") - Contra";

        Socket* soqueteCliente = new Socket(ventana.fd);
        //soqueteCliente->conectar(serverAdress,puerto);
  	std::cout << "Iniciando cliente: " << soqueteCliente->getSocketId() << "\n";

 
        char respuestaServidor[tamanio_respuestaServidor + 1];
        char respuestaCantBalas[MENSAJE_CANT_BALAS + 1];
        JuegoCliente juegoCliente(titulo.str(), "cliente", ventana.jugadores,ventana.orden);

        while(escuchador->enAccion() && juegoCliente.jugando()){
            string acciones = escuchador->obtenerAcciones();
            //if(acciones != "000000000"){
                // mas adelante enviamos solo si esta apretando alguna tecla (si las acciones es distinto de todo 0)
//                std::cout << "enviar: " << acciones << "\n";
                int enviados = soqueteCliente->enviar(soqueteCliente->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
//                std::cout << "enviadas\n";
                if(enviados == -1){
                    cout<<"Error en la conexion"<<endl;
                }
 
//                std::cout << "recibe\n";
                int llegaron =
                    soqueteCliente->recibir(soqueteCliente->getSocketId(),respuestaServidor,tamanio_respuestaServidor);
                respuestaServidor[tamanio_respuestaServidor] = 0;
//                std::cout << "recibido: " << respuestaServidor << "\n";
               
                string respuestaSinParsear(respuestaServidor);
                juegoCliente.setMensajeDelServidor(respuestaSinParsear);
   
//                std::cout << "5\n";
                juegoCliente.dibujar();
                juegoCliente.presentar();
		juegoCliente.manejarCierre();
//                std::cout << "6\n";
 
            //}
 
        }
        soqueteCliente->~Socket();
        return 0;
    }
    else{
 
    if(strcmp(comportamiento,"servidor") == 0){
    	signal (SIGINT, al_interrumpir);
   
        cout << "Arranca el servidor en " << serverAdress << ":" << puerto << "\n";

	int cantidadJugadores = cfg.obtener_i("//configuracion//cantidad_jugadores",[](int i, bool omision){return i >= 1 && i <= 4;});
	int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;
	autenticados a;
	pa = &a;
	esperar_jugadores (cantidadJugadores, serverAdress, puerto, a);

 	std::string titulo = "Contra";
	juego j(titulo, "servidor", cantidadJugadores);

 	std::cout << "Crea un mundo inicial\n";
	j.manejar_eventos ();
	j.actualizar ();
	a.mundo = j.armarRespuesta();

	std::unique_lock<std::mutex> lock(a.mutex);
	for (int i = 0; i < a.cantidad; i++) {
		a.usuarios[i].conector = new Socket (a.usuarios[i].fd);
		a.usuarios[i].conectado = true;
		a.usuarios[i].hilo = std::thread {comunicar_cliente, &a, i, tamanio_respuestaServidor};
		a.usuarios[i].tmp.ahora ();
		a.usuarios[i].recien_conectado = 100;
		memcpy (a.usuarios[i].teclas, "000000000", 10);
	}
	lock.unlock ();


	std::cout << "Iniciando ciclo\n";
 	while (!salir && j.jugando ()) {                         
		cfg.esperar_vblank ();

		std::unique_lock<std::mutex> lock(a.mutex);
		for (int i = 0; i < a.cantidad; i++) {
			std::lock_guard<std::mutex> lock_tmp(a.usuarios[i].mutex_tmp);
			//std::cout << "tiempo: " << a.usuarios[i].tmp.milisegundos () << "\n";
			int espera;
			if (a.usuarios[i].recien_conectado > 0) {
				espera = 10*MAX_TIEMPO_RESPUESTA;
				a.usuarios[i].recien_conectado--;
			} else {
				espera = MAX_TIEMPO_RESPUESTA;
			}
			if (a.usuarios[i].tmp.milisegundos () > espera) {
				if (a.usuarios[i].fd != -1) {
					if (!a.usuarios[i].conectado) {
						std::cout << "Reconectando cliente: " << i << "\n";
						j.desgrisarJugador(i+1);
						a.usuarios[i].conector = new Socket (a.usuarios[i].fd);
						a.usuarios[i].conectado = true;
						a.usuarios[i].tmp.ahora ();
						a.usuarios[i].recien_conectado = 100;
						a.usuarios[i].hilo = std::thread {comunicar_cliente, &a, i, tamanio_respuestaServidor};
					} else {
						std::cout << "Anulando cliente: " << i << "\n";
						j.grisarJugador(i+1);
						a.usuarios[i].fd = -1;
						a.usuarios[i].conectado = false;
						a.usuarios[i].hilo.detach ();
						delete (a.usuarios[i].conector);
					}
				}
			}
		}
		lock.unlock ();

		char teclas[a.cantidad][TAMANIO_MENSAJE_TECLAS + 1];
		for (int i = 0; i < a.cantidad; i++) {
			std::lock_guard<std::mutex> lock_tmp(a.usuarios[i].mutex_teclas);
			memcpy (teclas[i], a.usuarios[i].teclas, TAMANIO_MENSAJE_TECLAS+1);
		}

		for (int i = 0; i < a.cantidad; i++) {
			teclas[i][TAMANIO_MENSAJE_TECLAS] = 0;
			j.setAcciones (teclas[i], i+1);
		}

		j.manejar_eventos ();
		j.actualizar ();
		string mundo = j.armarRespuesta();

		std::unique_lock<std::mutex> lock_mundo(a.mutex_mundo);
		a.mundo = mundo;
		lock_mundo.unlock ();
		#if 0
		
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
		#endif
	}
	if (salir) {
		std::cout << "Saliendo por señal de interrupción\n";
	}
	lock.lock ();
	a.hilo.detach ();
	for (int i = 0; i < a.cantidad; i++) {
		delete (a.usuarios[i].conector);
	}
	lock.unlock ();
       
 
        registro.registrar (LogEventos::info, "Finalizo el juego");
        return r ;
    }
    }
    }
}

void comunicar_cliente (autenticados *a, int i, int tamanio_respuestaServidor)
{
	while (true) {
		// Recibe teclas
		char teclas[TAMANIO_MENSAJE_TECLAS + 1] = "000000000";
		std::cout << "recibe\n";
		int r = a->usuarios[i].conector->recibir(a->usuarios[i].conector->getAcceptedSocket(),teclas,TAMANIO_MENSAJE_TECLAS);
		std::cout << "recibido: " << teclas << "\n";
		if (r == -1) {
			break;
		}
		// Almacena teclas
		std::unique_lock<std::mutex> lock_teclas(a->usuarios[i].mutex_teclas);
		memcpy (a->usuarios[i].teclas, teclas, TAMANIO_MENSAJE_TECLAS+1);
		lock_teclas.unlock ();
		// Lee mundo
		std::string mundo;
		std::unique_lock<std::mutex> lock_mundo(a->mutex_mundo);
		mundo = a->mundo;
		lock_mundo.unlock ();
		// Envia mundo
//		std::cout << "envia: " << mundo << "\n";
		a->usuarios[i].conector->enviar(a->usuarios[i].conector->getAcceptedSocket(),mundo.c_str(),tamanio_respuestaServidor);
//		std::cout << "enviados\n";
		// Almacena estampa temporal
		std::lock_guard<std::mutex> lock_tmp(a->usuarios[i].mutex_tmp);
		a->usuarios[i].tmp.ahora ();
//		std::cout << "tiempo: " << a->usuarios[i].tmp.milisegundos () << "\n";
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



