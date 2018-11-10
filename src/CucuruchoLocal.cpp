#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
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
#include "mensaje_conexion.hpp"
#include "conector.hpp"
#include "ventana.hpp"

extern "C"
{
	#include <signal.h>
	#include "ctype.h"
	#include <stdio.h>
}

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
    std::cout << "uso: contra [-h] | ([-d (ERROR|INFO|DEBUG)] (-s|-c) (puerto|dir:puerto))\n\n";
    std::cout << "  -h             Muestra esta ayuda y finaliza\n";
    std::cout << "  -d OPCION      Estable a OPCION como nivel de depuracion\n";
    std::cout << "  -s puerto      Ejecuta contra como servidor en 127.0.0.1:puerto\n";
    std::cout << "  -s dir:puerto  Ejecuta contra como servidor en dir:puerto\n";
    std::cout << "  -c puerto      Ejecuta contra como cliente en 127.0.0.1:puerto\n";
    std::cout << "  -c dir:puerto  Ejecuta contra como cliente en dir:puerto\n";
    std::cout << "  -S puerto      Ejecuta como servidor en 127.0.0.1:puerto y muestra ventana\n\n";
}

#if 0 
void enviarBalas(int cantDeBalas,Socket* soquete){
    string balas = "00";
    string nuevo = to_string(cantDeBalas);
    balas.replace(2 - nuevo.size(),nuevo.size(),nuevo);
 
    soquete->enviar(soquete->getAcceptedSocket(),balas.c_str(),MENSAJE_CANT_BALAS);
 
    //el resto
}
#endif


// Comunicación desde el cliente hacia el servidor
void comunicar_servidor (Socket *conector, EscuchadorDeAcciones *escuchador, JuegoCliente *juegoCliente,
				 std::condition_variable *condicion_presento, std::mutex *mutex_presento,
				 bool *presento, int tamanio_respuestaServidor)
{
	char respuestaServidor[tamanio_respuestaServidor + 1];
	while (escuchador->enAccion() && juegoCliente->jugando()) {
		string acciones = escuchador->obtenerAcciones();
		std::cout << "enviar: " << acciones << "\n";
		int enviados = conector->enviar(conector->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
		std::cout << "enviadas\n";
		if (enviados == -1) {
			cout << "Error en la conexion al enviar" << endl;
			return;
		}

		std::cout << "recibe\n";
		int llegaron = conector->recibir(conector->getSocketId(),respuestaServidor,tamanio_respuestaServidor);
		if (llegaron != tamanio_respuestaServidor) {
			cout << "Error en la conexion al recibir" << endl;
			return;
		}
		respuestaServidor[tamanio_respuestaServidor] = 0;
		std::cout << "recibido: " << respuestaServidor << "\n";

		string respuestaSinParsear(respuestaServidor);
		juegoCliente->setMensajeDelServidor(respuestaSinParsear);

		std::cout << "5\n";
		juegoCliente->manejarEventos();
		juegoCliente->dibujar();
		juegoCliente->presentar();
		std::cout << "6\n";

		std::unique_lock<std::mutex> lock_presento(*mutex_presento);
		*presento = true;
		lock_presento.unlock ();
		condicion_presento->notify_all ();
		std::cout << "7\n";
	}
	std::cerr << "sale hilo secundario\n";
}

// Comunicación desde el servidor hacia el cliente
void comunicar_cliente (autenticados *a, int i, int tamanio_respuestaServidor)
{
	while (true) {
		// Recibe teclas
		char teclas[TAMANIO_MENSAJE_TECLAS + 1] = "000000000";
		 std::cout << "recibe" << i << ":\n";
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
		 std::cout << "envia" << i << ": " << mundo << "\n";
		a->usuarios[i].conector->enviar(a->usuarios[i].conector->getAcceptedSocket(),mundo.c_str(),tamanio_respuestaServidor);
		 std::cout << "enviados\n";
		// Almacena estampa temporal
		std::lock_guard<std::mutex> lock_tmp(a->usuarios[i].mutex_tmp);
		a->usuarios[i].tmp.ahora ();
		// std::cout << "tiempo: " << a->usuarios[i].tmp.milisegundos () << "\n";
	}
}

bool establecer_opcion_depurado (const char *depurado)
{
	std::cout << "depurado: " << depurado << "\n";
	if (!strcmp (depurado, "ERROR")) {
		registro.definirTipoLog (LogEventos::error);
	} else if (!strcmp (depurado, "INFO")) {
		registro.definirTipoLog (LogEventos::info);
	} else if (!strcmp (depurado, "DEBUG")) {
		registro.definirTipoLog (LogEventos::debug);
	} else {
		std::stringstream ss;
		ss << "Opcion de depurado '" << depurado << "' no reconida";
		registro.registrar (LogEventos::error, ss.str().c_str());
		return false;
	}
	return true;
}

static bool consumir_byte (istringstream &is)
{
	int i;
	try {
		is >> i;
		if (i < 0 || i > 255) {
			return false;
		}
	} catch (...) {
		return false;
	}
	return true;
}

static bool consumir (istringstream &is, char c)
{
	char aux;
	try {
		is >> aux;
		if (aux != c) {
			return false;
		}
	} catch (...) {
		return false;
	}
	return true;
}

static bool consumir_puerto (istringstream &is, unsigned short &puerto)
{
	int i;
	try {
		is >> i;
		if (i < 1024 || i > 65535) {
			return false;
		}
		puerto = (unsigned short)i;
	} catch (...) {
		return false;
	}
	return true;
}

bool obtener_dir_puerto (char *arg, std::string &dir, unsigned short &puerto)
{
	istringstream is (arg);
	if (consumir_byte (is)
		&& consumir (is, '.')
		&& consumir_byte (is)
		&& consumir (is, '.')
		&& consumir_byte (is)
		&& consumir (is, '.')
		&& consumir_byte (is)
		&& consumir (is, ':'))
	{
		int p = is.tellg();
		arg[p-1] = '\0';
		dir = arg;
	} else {
		dir = "127.0.0.1";
		is.seekg (0, is.beg);
	}
	if (!consumir_puerto (is, puerto)) {
		return false;
	}
	return true;
}

int main (int argc, char *argv[]) {
	registro.registrarNuevoJuego();

	bool ayuda = false;
	bool como_servidor = false;
	bool como_cliente = false;
	bool mostrar_ventana_servidor = false;
	std::string dir;
	unsigned short puerto;
	while (!ayuda && *argv) {
		if ((*argv)[0] == '-') switch ((*argv)[1]) {
			case 'd':
				argv++;
				if (!*argv || !establecer_opcion_depurado (*argv)) {
					ayuda = true;
				}
				break;
			case 'S':
				mostrar_ventana_servidor = true;
			case 's':
				argv++;
				if (!*argv || !obtener_dir_puerto (*argv, dir, puerto)) {
					ayuda = true;
				}
				como_servidor = true;
				break;
			case 'c':
				argv++;
				if (!*argv || !obtener_dir_puerto (*argv, dir, puerto)) {
					ayuda = true;
				}
				como_cliente = true;
				break;
			default:
				ayuda = true;
				break;
		};
		argv++;
	}
	if (ayuda || (como_servidor == como_cliente)) {
		uso ();
		return 1;
	}

	if (como_cliente) {
		std::stringstream ss;
		ss << dir << ":" << puerto;
		cout << "Arranca el cliente en " << ss.str() << "\n";
		Parser parser;
		EscuchadorDeAcciones* escuchador = new EscuchadorDeAcciones();

		ventana v (std::string ("Login - Contra"), MUNDO_ANCHO, MUNDO_ALTO);

		vlogin vl (v, ss.str());
		vl.correr ();
		std::cout << "Usuario: " << vl.cred.usuario << "\n";
		std::cout << "Clave: " << vl.cred.clave << "\n";
		std::cout << "Direccion: " << vl.cred.direccion << "\n";
		std::cout << "Puerto: " << vl.cred.puerto << "\n";
		if (vl.cred.resultado != usuario::aceptado && vl.cred.resultado != usuario::reaceptado) {
			return 0;
		}

		int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * vl.cred.jugadores;
		std::cout << "Cliente: fd: " << vl.cred.fd << ", jugadores: " << vl.cred.jugadores << "\n";

		std::stringstream titulo;
		titulo << vl.cred.usuario << "(" << vl.cred.orden << ") - Contra";
		v.titulo (titulo.str().c_str());

		Socket* soqueteCliente = new Socket(vl.cred.fd);
	  	std::cout << "Iniciando cliente: " << soqueteCliente->getSocketId() << "\n";
	 
		JuegoCliente juegoCliente(v, vl.cred.jugadores, vl.cred.orden);

		std::mutex mutex_mundo;

		std::condition_variable condicion_presento;
		std::mutex mutex_presento;
		bool presento = false;
		std::thread hilo = std::thread {
			comunicar_servidor,
			soqueteCliente,
			escuchador,
			&juegoCliente,
			&condicion_presento,
			&mutex_presento,
			&presento,
			tamanio_respuestaServidor
		};
		bool recien_conectado = true;
		while(escuchador->enAccion() && juegoCliente.jugando()){
			std::unique_lock<std::mutex> lock_presento(mutex_presento);
			if (!condicion_presento.wait_for (
				lock_presento,
				std::chrono::milliseconds(recien_conectado ? MAX_TIEMPO_RESPUESTA_NUEVO : MAX_TIEMPO_RESPUESTA),
				[&presento]{return presento;}
			)) {
				std::cerr << "delete (soqueteCliente)\n";
				delete (soqueteCliente);
				soqueteCliente = nullptr;
				std::cerr << "hilo.join ()\n";
				if (hilo.joinable()) {
					hilo.join ();
				}

				ss.str (std::string());
				std::string usr = vl.cred.usuario;
				usr[0] = toupper (usr[0]);
				ss << "Lo sentimos " << usr << ". Se perdio la conexion con el servidor.\nVerifique que el mismo se encuentre corriendo en "
				   << vl.cred.direccion << ':' << vl.cred.puerto << "\ny que tiene acceso a la red. Intentando reconectarse...";
				mensaje_conexion msg (v, vl.cred);
				msg.correr (ss.str());
				if (vl.cred.resultado != usuario::reaceptado) {
					ss.str (std::string());
					std::string usr = vl.cred.usuario;
					usr[0] = toupper (usr[0]);
					ss << "Lo sentimos " << usr << ". La partida en la que participabas ya no existe mas.";
					// TODO opcion login
					mensaje msg (v);
					msg.correr (ss.str());
					break;
				}

				soqueteCliente = new Socket(vl.cred.fd);
				presento = false;
				recien_conectado = true;
				std::cerr << "lanzo hilo\n";
				hilo = std::thread {
					comunicar_servidor,
					soqueteCliente,
					escuchador,
					&juegoCliente,
					&condicion_presento,
					&mutex_presento,
					&presento,
					tamanio_respuestaServidor
				};
				std::cerr << "lanzado\n";
			} else {
				presento = false;
				recien_conectado = false;
			}
		}
		if (soqueteCliente) {
			std::cerr << "delete (soqueteCliente)\n";
			delete (soqueteCliente);
		}
		if (hilo.joinable()) {
			std::cerr << "hilo.join ()\n";
			hilo.join ();
		}
	} else { // como_servidor
		signal (SIGINT, al_interrumpir);

		cout << "Arranca el servidor en " << dir << ":" << puerto << "\n";

		int cantidadJugadores = cfg.obtener_i("//configuracion//cantidad_jugadores",[](int i, bool omision){return i >= 1 && i <= 4;});
		int tamanio_respuestaServidor = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;
		autenticados a;
		pa = &a;
		esperar_jugadores (cantidadJugadores, dir.c_str(), puerto, a);

		ventana v (std::string ("Servidor - Contra"), MUNDO_ANCHO, MUNDO_ALTO);
		if (mostrar_ventana_servidor) {
			v.mostrar ();
		}
		juego j(v, cantidadJugadores);

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
	 		if (!mostrar_ventana_servidor) {
				cfg.esperar_vblank ();
			}
			
			std::unique_lock<std::mutex> lock(a.mutex);
			for (int i = 0; i < a.cantidad; i++) {
				std::lock_guard<std::mutex> lock_tmp(a.usuarios[i].mutex_tmp);
				//std::cout << "tiempo: " << a.usuarios[i].tmp.milisegundos () << "\n";
				int espera;
				if (a.usuarios[i].recien_conectado > 0) {
					espera = MAX_TIEMPO_RESPUESTA_NUEVO;
					a.usuarios[i].recien_conectado--;
				} else {
					espera = MAX_TIEMPO_RESPUESTA;
				}
				if (a.usuarios[i].tmp.milisegundos () > espera) {
					if (a.usuarios[i].fd != -1 && !a.usuarios[i].esperando_ok) {
						if (!a.usuarios[i].conectado) {
							std::cout << "Reconectando cliente: " << i << "\n";
							j.desgrisarJugador(i+1);
							a.usuarios[i].conector = new Socket (a.usuarios[i].fd);
							a.usuarios[i].conectado = true;
							a.usuarios[i].tmp.ahora ();
							a.usuarios[i].recien_conectado = MAX_TIEMPO_RESPUESTA_NUEVO/17;
							a.usuarios[i].hilo = std::thread {comunicar_cliente, &a, i, tamanio_respuestaServidor};
						} else {
							std::cout << "Anulando cliente: " << i << "\n";
							j.grisarJugador(i+1);
							a.usuarios[i].fd = -1;
							a.usuarios[i].conectado = false;
							a.usuarios[i].hilo.detach ();
							delete (a.usuarios[i].conector);
							a.usuarios[i].conector = nullptr;
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

			if (mostrar_ventana_servidor) {
				j.dibujar();
				j.presentar();
			}
		}
		if (salir) {
			std::cout << "Saliendo por señal de interrupción\n";
		}
		lock.lock ();
		std::cerr << "a.hilo.detach ()\n";
		a.hilo.detach ();
		for (int i = 0; i < a.cantidad; i++) {
			std::cerr << "delete (a.usuarios[i].conector)\n";
			delete (a.usuarios[i].conector);
			if (a.usuarios[i].hilo.joinable ()) {
				std::cerr << "a.usuarios[i].hilo.detach ()\n";
				a.usuarios[i].hilo.detach ();
			}
		}
		lock.unlock ();
		registro.registrarFinJuego();
	}
	std::cerr << "sale hilo principal\n";



	return 0;
}

