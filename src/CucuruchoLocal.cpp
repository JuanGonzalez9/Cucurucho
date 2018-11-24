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
#include "JuegoCliente.h"
#include "Constantes.h"
#include <thread>
#include <mutex>
#include "login.hpp"
#include "mensaje_conexion.hpp"
#include "conector.hpp"
#include "ventana.hpp"
#include "score.hpp"


extern "C"
{
	#include <signal.h>
	#include "ctype.h"
	#include <stdio.h>
}

autenticados *pa = nullptr;
void al_interrumpir (int s)
{
	if (pa && s == SIGINT) {
		std::cout << "Interrupcion manejada por el servidor\n";
		interrumpir_servidor (*pa);
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

typedef struct
{
	Socket *conector;
	EscuchadorDeAcciones escuchador;
	int tamanio_respuesta;
	bool recien_conectado;
} datos_comunicacion;

typedef struct
{
	std::thread hilo;
	std::condition_variable condicion_presento;
	std::mutex mutex_presento;
	bool presento, finalizo;
} datos_sincronizacion;

// Comunicación desde el cliente hacia el servidor
void comunicar_servidor (datos_comunicacion *dc, datos_sincronizacion *ds, JuegoCliente *juego, score *puntuacion)
{
	char respuestaServidor[dc->tamanio_respuesta + 1];
	while (dc->escuchador.enAccion() && juego->jugando()) {
		string acciones = dc->escuchador.obtenerAcciones();
		//std::cout << "enviar: " << acciones << "\n";
		int enviados = dc->conector->enviar(dc->conector->getSocketId(),acciones.c_str(),TAMANIO_MENSAJE_TECLAS);
		//std::cout << "enviadas\n";
		if (enviados != TAMANIO_MENSAJE_TECLAS) {
			cout << "Error en la conexion al enviar" << endl;
			break;
		}

		//std::cout << "recibe\n";
		int llegaron = dc->conector->recibir(dc->conector->getSocketId(),respuestaServidor,dc->tamanio_respuesta);
		if (llegaron != dc->tamanio_respuesta) {
			cout << "Error en la conexion al recibir" << endl;
			break;
		}
		respuestaServidor[dc->tamanio_respuesta] = 0;
		//std::cout << "recibido\n";
		//std::cout << "recibido: " << respuestaServidor << "\n";
		
		if (respuestaServidor[0] == 's' || respuestaServidor[0] == 'S') {
			if (respuestaServidor[0] == 's') {
				std::stringstream ss(respuestaServidor+1);
				puntuacion->desserializar(ss);
				respuestaServidor[0] = 'S';
			}
			puntuacion->manejar_eventos();
			puntuacion->actualizar();
			puntuacion->dibujar();
			puntuacion->presentar();
		} else if (respuestaServidor[0] == 'f') {
			std::stringstream ss(respuestaServidor+1);
			puntuacion->desserializar(ss);
		} else {
			string respuestaSinParsear(respuestaServidor);
			juego->setMensajeDelServidor(respuestaSinParsear);
			juego->manejarEventos();
			juego->dibujar();
			juego->presentar();
		}

		std::unique_lock<std::mutex> lock_presento(ds->mutex_presento);
		ds->presento = true;
		if (respuestaServidor[0] == 'f') {
			ds->finalizo = true;
			ds->condicion_presento.notify_all ();
			break;
		} else {
			ds->condicion_presento.notify_all ();
		}
	}
	std::cout << "sale de comunicar_servidor\n";
}

// Comunicación desde el servidor hacia el cliente
void comunicar_cliente (autenticados *a, int i, int tamanio_respuesta)
{
	while (true) {
		// Recibe teclas
		char teclas[TAMANIO_MENSAJE_TECLAS + 1] = "000000000";
		//std::cout << "recibe" << i << ":\n";
		int r = a->usuarios[i].conector->recibir(a->usuarios[i].conector->getAcceptedSocket(),teclas,TAMANIO_MENSAJE_TECLAS);
		//std::cout << "recibido: " << teclas << "\n";
		if (r != TAMANIO_MENSAJE_TECLAS) {
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
		//std::cout << "envia" << i << ": " << mundo << "\n";
		r = a->usuarios[i].conector->enviar(a->usuarios[i].conector->getAcceptedSocket(),mundo.c_str(),tamanio_respuesta);
		if (r != tamanio_respuesta) {
			break;
		}
		if (mundo[0] == 'f') {
			break;
		}
		//std::cout << "enviados\n";
		// Almacena estampa temporal
		std::lock_guard<std::mutex> lock_tmp(a->usuarios[i].mutex_tmp);
		a->usuarios[i].tmp.ahora ();
		// std::cout << "tiempo: " << a->usuarios[i].tmp.milisegundos () << "\n";
	}
	std::cout << "sale de comunicar_cliente\n";
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

void inicializar (datos_comunicacion &dc, datos_sincronizacion &ds, int fd, int tamanio_respuesta, JuegoCliente &juego, score &puntuacion)
{
	dc.conector = new Socket (fd);
	dc.tamanio_respuesta = tamanio_respuesta;
	dc.recien_conectado = true;
	ds.presento = false;
	ds.finalizo = false;
	ds.hilo = std::thread {
		comunicar_servidor,
		&dc,
		&ds,
		&juego,
		&puntuacion
	};
}

void finalizar (datos_comunicacion &dc, datos_sincronizacion &ds)
{
	if (dc.conector) {
		dc.conector->cerrar ();
	}
	if (ds.hilo.joinable()) {
		std::cerr << "hilo.join ()\n";
		ds.hilo.join ();
	}
	if (dc.conector) {
		std::cerr << "delete (conector)\n";
		delete (dc.conector);
		dc.conector = nullptr;
	}
}

bool informar_perdida_conexion (ventana &v, credencial &cred)
{
	std::stringstream ss;
	std::string usr = cred.usuario;
	usr[0] = toupper (usr[0]);
	ss << "Lo sentimos " << usr << ". Se perdio la conexion con el servidor.\nVerifique que el mismo se encuentre corriendo en "
	   << cred.direccion << ':' << cred.puerto << "\ny que tiene acceso a la red. Intentando reconectarse...";
	mensaje_conexion msg (v, cred);
	msg.correr (ss.str());
	// Como el dialogo continua intentado reconectarse indefinidamente, si el resultado del login
	// es fallido, se que el usuario cancelo la operación.
	return cred.resultado != usuario::fallido;
}

void informar_partida_finalizada (ventana &v, credencial &cred)
{
	std::stringstream ss;
	std::string usr = cred.usuario;
	usr[0] = toupper (usr[0]);
	ss << usr << ", la partida en la que participabas ya no existe mas.";
	// TODO opcion login
	mensaje msg (v);
	msg.correr (ss.str());
}

void informar_jugando (ventana &v, credencial &cred)
{
	std::stringstream ss;
	std::string usr = cred.usuario;
	usr[0] = toupper (usr[0]);
	ss << usr << " se encuentra jugando en otro cliente.";
	// TODO opcion login
	mensaje msg (v);
	msg.correr (ss.str());
}

void correr_cliente (std::string dir, unsigned short puerto)
{
	std::stringstream ss;
	ss << dir << ":" << puerto;
	cout << "Arranca el cliente en " << ss.str() << "\n";

	ventana v (std::string ("Login - Contra"), MUNDO_ANCHO, MUNDO_ALTO);

	vlogin vl (v, ss.str());
	vl.correr ();
	std::cout << "Usuario: " << vl.cred.usuario << "\n";
	std::cout << "Clave: " << vl.cred.clave << "\n";
	std::cout << "Direccion: " << vl.cred.direccion << "\n";
	std::cout << "Puerto: " << vl.cred.puerto << "\n";
	if (vl.cred.resultado == usuario::aceptado || vl.cred.resultado == usuario::reaceptado) {
		int tamanio_respuesta = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * vl.cred.jugadores;
		std::cout << "Cliente: fd: " << vl.cred.fd << ", jugadores: " << vl.cred.jugadores << "\n";

		std::stringstream titulo;
		titulo << vl.cred.usuario << "(" << vl.cred.orden << ") - Contra";
		v.titulo (titulo.str().c_str());

		score puntuacion (v, vl.cred.orden, vl.cred.jugadores);
		JuegoCliente juego(v, vl.cred.jugadores, vl.cred.orden, puntuacion);

		datos_comunicacion dc;
		datos_sincronizacion ds;
		inicializar (dc, ds, vl.cred.fd, tamanio_respuesta, juego, puntuacion);

		std::mutex mutex_mundo;
		while (dc.escuchador.enAccion() && juego.jugando()){
			std::unique_lock<std::mutex> lock_presento(ds.mutex_presento);
			if (vl.cred.arranca_grisado || (!ds.presento && !ds.condicion_presento.wait_for (
				lock_presento,
				std::chrono::milliseconds(dc.recien_conectado ? MAX_TIEMPO_RESPUESTA_NUEVO : MAX_TIEMPO_RESPUESTA),
				[&ds]{return ds.presento;}
			))) {
				vl.cred.arranca_grisado = false;
				finalizar (dc, ds);

				if (!informar_perdida_conexion (v, vl.cred)) {
					// Cerro la ventana, cancelando la reconexion.
					break;
				} else if (vl.cred.resultado == usuario::jugando) {
					// No pudo registrarse porque el usuario ya se encuentra jugando.
					informar_jugando (v, vl.cred);
					break;
				} else if (vl.cred.resultado != usuario::reaceptado) {
					// Termino la partida porque al registrarse se acepto, se rechazo o no habia cupo.
					informar_partida_finalizada (v, vl.cred);
					break;
				}
				// Solo ahora se que fue reaceptado.
				inicializar (dc, ds, vl.cred.fd, tamanio_respuesta, juego, puntuacion);
			} else {
				ds.presento = false;
				dc.recien_conectado = false;
			}
			if (ds.finalizo) {
				std::cout << "Finalizacion detectada en cliente\n";
				break;
			}
		}
		finalizar (dc, ds);
		if (ds.finalizo) {
			puntuacion.agregar_boton_aceptar ();
			puntuacion.correr ();
		}
	}
}

void finalizar (usuario &u)
{
	if (u.conector) {
		u.conector->cerrar ();
	}
	u.conectado = false;
	if (u.hilo.joinable ()) {
		std::cerr << "u.hilo.join ()\n";
		u.hilo.join ();
	}
	if (u.conector) {
		delete (u.conector);
		u.conector = nullptr;
	}
	u.fd = -1;
}

void finalizar (autenticados &a)
{
	std::unique_lock<std::mutex> lock(a.mutex);
	std::cerr << "a.hilo.detach ()\n";
	if (a.hilo_1.joinable ()) {
		a.hilo_1.detach ();
	}
	if (a.hilo_2.joinable ()) {
		a.hilo_2.detach ();
	}
	for (int i = 0; i < a.cantidad; i++) {
		finalizar (a.usuarios[i]);
	}
}

void inicializar (usuario &u, autenticados &a, int i, int tamanio_respuesta)
{
	u.conector = new Socket (u.fd);
	u.conectado = true;
	u.hilo = std::thread {comunicar_cliente, &a, i, tamanio_respuesta};
	u.tmp.ahora ();
	u.recien_conectado = MAX_TIEMPO_RESPUESTA_NUEVO/17;
	memcpy (u.teclas, "000000000", 10);
	if (u.arranca_grisado) {
		std::cout << "finalizar (u)\n";
		finalizar (u);
	}
}

void inicializar (autenticados &a, int tamanio_respuesta)
{
	std::unique_lock<std::mutex> lock(a.mutex);
	for (int i = 0; i < a.cantidad; i++) {
		inicializar (a.usuarios[i], a, i, tamanio_respuesta);
	}
	lock.unlock ();
}

void verificar_clientes (autenticados &a, juego &j, int tamanio_respuesta)
{
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
		if (a.usuarios[i].tmp.milisegundos () > espera && a.usuarios[i].fd != -1 && !a.usuarios[i].esperando_ok) {
			if (!a.usuarios[i].conectado) {
				std::cout << "Reconectando cliente: " << i << "\n";
				j.desgrisarJugador(i+1);
				inicializar (a.usuarios[i], a, i, tamanio_respuesta);
			} else {
				std::cout << "Anulando cliente: " << i << "\n";
				finalizar (a.usuarios[i]);
				memcpy (a.usuarios[i].teclas, "000000000", 10);
				j.grisarJugador(i+1);
			}
		}
	}
}

void correr_servidor (std::string dir, unsigned short puerto, bool mostrar_ventana)
{
	/*
	ventana v(std::string ("Servidor - Contra"), MUNDO_ANCHO, MUNDO_ALTO);
	score s(v, 4);
	std::stringstream ss("Barbara 400 500 1100;Dalma 50 180 -1;Juan 940 -1 -1;Martin 520 20 510;");
	//s.agregar_boton_aceptar ();
	s.desserializar (ss);
	s.correr();
	return;
	*/

	autenticados a;
	a.salir = false;
	signal (SIGINT, al_interrumpir);

	int cantidadJugadores = cfg.obtener_i("//configuracion//cantidad_jugadores",[](int i, bool omision){return i >= 1 && i <= 4;});
	int tamanio_respuesta = TAMANIO_RESPUESTA_SERVIDOR + RESPUESTA_PERSONAJE * cantidadJugadores;
	pa = &a;
	esperar_jugadores (cantidadJugadores, dir.c_str(), puerto, a);
	if (!a.salir ) {
		ventana v (std::string ("Servidor - Contra"), MUNDO_ANCHO, MUNDO_ALTO);
		if (mostrar_ventana) {
			v.mostrar ();
		}
		puntajes pts (cantidadJugadores);
		for (int i = 0; i < a.cantidad; i++) {
			pts.nombre (i, a.usuarios[i].nombre);
		}
		juego j(v, cantidadJugadores, pts);

	 	std::cout << "Crea un mundo inicial\n";
		j.manejar_eventos ();
		j.actualizar ();
		j.setDatosEnemigos();
		j.setDatosItems();
		j.setDatosBalasEnemigas();
		a.mundo = j.armarRespuesta();

		inicializar (a, tamanio_respuesta);

		std::cout << "Iniciando ciclo\n";
		int mostrando_score = 0;
	 	while (!a.salir && (j.jugando () || mostrando_score)) {
	 		if (!mostrar_ventana) {
				cfg.esperar_vblank ();
			}

			// Griso y desgriso clientes.
			verificar_clientes (a, j, tamanio_respuesta); 

			if (mostrando_score) {
				mostrando_score--;
			} else {
				// Leo de la memoria el estado de las teclas de todos los jugadores.
				// Esta informacion se recibe del usuario en comunicar_cliente que corre en a.usuarios[i].hilo.
				char teclas[a.cantidad][TAMANIO_MENSAJE_TECLAS + 1];
				for (int i = 0; i < a.cantidad; i++) {
					std::lock_guard<std::mutex> lock_tmp(a.usuarios[i].mutex_teclas);
					memcpy (teclas[i], a.usuarios[i].teclas, TAMANIO_MENSAJE_TECLAS+1);
				}

				// Guardo el estado de las teclas.
				for (int i = 0; i < a.cantidad; i++) {
					teclas[i][TAMANIO_MENSAJE_TECLAS] = 0;
					j.setAcciones (teclas[i], i+1);
				}

				// Actualizo el mundo.
				j.manejar_eventos ();
				j.actualizar ();
				
				// Creo la respuesta para el cliente con la información para que dibuje la pantalla.
				j.setDatosEnemigos();
				j.setDatosItems();
				j.setDatosBalasEnemigas();
				string mundo = j.armarRespuesta();
				static char prev_nivel = '1';
				if (mundo[0] != prev_nivel || !j.jugando()) {
					std::cout << "Nivel: " << mundo[0] << "\n";
					prev_nivel = mundo[0];
					mostrando_score = TIEMPO_SCORE;
					std::stringstream ss;
					ss << (j.jugando() ? 's' : 'f');
					j.serializar_puntaje (ss);
					mundo = ss.str();
				}

				// Almaceno la respuesta en memoria.
				// Esta informacion se envia al usuario en comunicar_cliente que corre en a.usuarios[i].hilo.
				std::unique_lock<std::mutex> lock_mundo(a.mutex_mundo);
				a.mundo = mundo;
				lock_mundo.unlock ();
			}
			if (mostrar_ventana) {
				j.dibujar();
				j.presentar();
			}
		}
	}
	finalizar (a);
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
		correr_cliente (dir, puerto);
	} else {
		correr_servidor (dir, puerto, mostrar_ventana_servidor);
	}
	registro.registrarFinJuego();
	std::cerr << "sale hilo principal\n";
	return 0;
}

