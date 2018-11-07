#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include "conector.hpp"
#include "login.hpp"
#include "cfg.hpp"

extern "C"
{
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h>
	#include <arpa/inet.h>
	#include <string.h>
	#include <unistd.h>
}

#define MAX_SOCKET_MSG (10*1024)

bool separar (char *t, std::string &a, std::string &b, char sep)
{
	char * u = t;
	char * c = u;
	while (*c && *c != sep) {
		c++;
	}
	if (*c != sep) {
		return false;		
	}
	*c++ = '\0';
	if (strlen (c) == 0) {
		return false;		
	}
	a = u;
	b = c ;
	return true;
}

static bool separar (std::string &t, std::string &a, char sep)
{
	size_t pos = t.find (sep);
	if (pos == std::string::npos) {
		return false;		
	}
	a = t.substr (pos+1);
	t.erase(pos);
	if (t.length() == 0 || a.length() == 0) {
		return false;
	}
	return true;
}

bool leer (int fd, std::string &s)
{
	std::cout << "Recibiendo en fd: " << fd << "\n";
	const int l = 11;
	int leidos = 0;
	char msg[l];
	while (leidos < l && recv (fd, msg + leidos, 1, 0) == 1) {
		leidos++;
		if (msg[leidos-1] == ':') {
			break;
		}
	}
	if (msg[leidos-1] != ':') {
		std::cout << "No encontro el :, sino " << msg[leidos-1] << "\n";
		return false;
	}
	msg[leidos-1] = '\0';
	
	int largo;
	try {
		largo = std::stoi (msg);
		std::cout << "largo: " << largo << "\n";
		if (largo > MAX_SOCKET_MSG || largo <= 0) {
			std::cout << "Largo de respuesta invalido\n";
			return false;
		}
	} catch (...) {
		return false;
	}
	std::stringstream ss;
	leidos = 0;
	int r;
	std::vector<char> buffer (largo);
	int faltan = largo;
	while (leidos < largo && (r = recv (fd, &buffer[0], faltan, 0)) > 0) {
		leidos += r;
		faltan -= r;
		buffer[leidos] = '\0';
		ss << &buffer[0];
	}
	if (leidos < largo) {
		return false;
	}
	s = ss.str();
	std::cout << "Recibido: " << s << "\n";
	return true;
}

bool escribir (int fd, std::string &s)
{
	int largo = s.length() + 1;
	std::stringstream ss;
	ss << largo << ':' << s;
	std::string tmp = ss.str();
	const char *msg = tmp.c_str();
	std::cout << "Escribiendo '" << msg << "' en fd: " << fd << "\n";
	const int l = strlen (msg)+1;
	int escritos = 0, r;
	while (escritos < l && (r = send (fd, msg+escritos, l-escritos, 0)) > 0) {
		escritos += r;
	}
	if (r == -1) {
		return false;
	}
	return true;
}

static bool leer_credencial (int fd, std::string & usuario, std::string & clave)
{
	// Hilo secundario
	leer (fd, usuario);
	if (!separar (usuario, clave, ',')) {
		std::cout << "El cliente no envio una credencial\n";
		return false;		
	}
	return true;
}

static void escribir_resultado_autenticacion (const char * msg, int fd, int orden, int jugadores)
{
	// Hilo secundario
	std::stringstream ss;
	ss << msg << "," << jugadores << "," << orden;
	std::string s = ss.str();
	escribir (fd, s); // TODO y si falla?
}

static bool leer_resultado_autenticacion (int fd, usuario::estado &estado, int &jugadores, int &orden)
{
	// Hilo secundario
	std::string e, j, o;
	leer (fd, e);
	if (!separar (e, j, ',')) {
		std::cout << "El cliente no envio una credencial\n";
		return false;		
	}
	if (!separar (j, o, ',')) {
		std::cout << "El servidor no envio una respuesta al intento de login\n";
		return false;		
	}
	try {
		jugadores = std::stoi (j);
		orden = std::stoi (o);
		if (e == "aceptado") {
			estado = usuario::aceptado;
		} else if (e == "rechazado") {
			estado = usuario::rechazado;
		} else if (e == "cupo") {
			estado = usuario::cupo;
		} else if (e == "jugando") {
			estado = usuario::jugando;
		} else {
			return false;
		}
	} catch (...) {
		std::cout << "El servidor no envio una respuesta al intento de login\n";
		return false;
	}
	return true;
}

static void establecer_usuario (struct usuario & u, int fd, const std::string &usuario)
{
	u.fd = fd;
	u.nombre = usuario;
	u.esperando_ok = true;
}

static usuario::estado autenticar_usuario (autenticados *a, std::string &usuario, std::string &clave, int &orden, int fd)
{
	std::stringstream camino;
	camino << "//configuracion//usuarios//" << usuario;
	std::cout << "Buscando cadena en " << camino.str() << "\n";
	std::string c = cfg.obtener_s (camino.str().c_str(), [](std::string &cadena, bool omision) {return true;});
	const char *ws = " \t\n\r\v\f";
	c.erase(c.find_last_not_of(ws) + 1);
	c.erase(0, c.find_first_not_of(ws));
	if (c.length () < MIN_CLAVE || c.length () > MAX_CLAVE) {
		// TODO: registrar
		std::stringstream ss;
		ss << "La clave " << c << " del usuario '" << usuario << "' debe tener desde " << MIN_CLAVE << " hasta " << MAX_CLAVE << " caracteres";
		std::cout << ss.str() << "\n";
		return usuario::rechazado;
	}
	//std::cout << "Comparando " << c << " con " << clave << "\n";
	
	if (clave == c) {
		std::lock_guard<std::mutex> lock(a->mutex);
		for (int i = 0; i < a->cantidad; i++) {
			std::cout << "Comparando '" << usuario << "' con '" << a->usuarios[i].nombre << "'\n";
			if (a->usuarios[i].nombre == usuario) {
				orden = i;
				if (a->usuarios[i].fd == -1) {
					establecer_usuario (a->usuarios[orden], fd, usuario);
					std::cout << "Reconectado: " << usuario << "\n";
					return usuario::aceptado;
				} else {
					std::cout << "Rechazado por estar jugando: " << usuario << "\n";
					return usuario::jugando;
				}
			}
		}
		if (a->cantidad < a->requeridos) {
			orden = a->cantidad;
			establecer_usuario (a->usuarios[orden], fd, usuario);
			a->cantidad++;
			std::cout << "Aceptado: " << usuario << "\n";
			return usuario::aceptado;
		} else {
			std::cout << "Rechazado por cupo alcanzado: " << usuario << "\n";
			return usuario::cupo;
		}
	}
	std::cout << "Rechazado: " << usuario << "\n";
	return usuario::rechazado;
}

static void comprobar_credencial (autenticados *a, int fd, int jugadores)
{
	// Hilo secundario
	std::string usuario, clave;
	int orden = 0;
	if (!leer_credencial (fd, usuario, clave) )
	{
		return;
	}
	switch (autenticar_usuario (a, usuario, clave, orden, fd) ) {
		case usuario::aceptado: {
			escribir_resultado_autenticacion ("aceptado", fd, orden, jugadores);
			std::lock_guard<std::mutex> lock(a->mutex);
			if (a->cantidad == a->requeridos) {
				std::cout << "Cupo alcanzado\n";
				if (!a->comenzo) {
					a->comenzo = true;
					std::cout << "Notifico que se cumplió el cupo de jugadores\n";
					a->condicion.notify_one();
				}
				for (int i = 0; i < a->cantidad; i++) {
					if (a->usuarios[i].esperando_ok) {
						std::cout << "esperando ok\n";
						esperar_ok (a->usuarios[i].fd);
						std::cout << "enviando ok a " << a->usuarios[i].nombre << " en fd " << a->usuarios[i].fd << "\n";
						a->usuarios[i].esperando_ok = false;
						enviar_ok (a->usuarios[i].fd);
						std::cout << "ok enviado\n";
					}
				}
			}
			break;
		}
		case usuario::rechazado:
			escribir_resultado_autenticacion ("rechazado", fd, orden, jugadores);
			break;
		case usuario::cupo:
			escribir_resultado_autenticacion ("cupo", fd, orden, jugadores);
			break;
		case usuario::fallido:
			escribir_resultado_autenticacion ("fallido", fd, orden, jugadores);
			break;
		case usuario::jugando:
			escribir_resultado_autenticacion ("jugando", fd, orden, jugadores);
			break;
	};
	std::cout << "Termino la autenticacion\n";
}

void escuchar (autenticados *a, const char *dir, int puerto, int jugadores)
{
	int fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd == -1) {
		std::cout << "Fallo socket: " << strerror(errno) << "\n";
		return;
	}

	sockaddr_in addr = {AF_INET, htons(puerto), inet_addr(dir)};
	int r = bind (fd, (sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (r == -1) {
		std::cout << "Fallo bind: " << strerror(errno) << "\n";
		return;
	}

	r = listen (fd, SOMAXCONN);
	if (r == -1) {
		std::cout << "Fallo listen: " << strerror(errno) << "\n";
		if (errno == EADDRINUSE) {
			std::cout << "Probar otro puerto\n";
		}
		return;
	}

	int conectados = 0;
	std::list<std::thread> hilos;
	
	while (true) {
		int fd_nuevo = accept(fd, nullptr, nullptr);
		if (fd_nuevo == -1) {
			std::cout << "Fallo accept: " << strerror(errno) << "\n";
			return;
		} else {
			std::cout << "Cliente conectado\n";
			hilos.push_back (std::thread{ comprobar_credencial, a, fd_nuevo, jugadores });
			conectados++;
		}
		// TODO terminar hilos colgados y unirse a los que terminaron
		//pthread_cancel(thrd.native_handle());
	}
	/*
	std::cout << "Esperando fin de hilos\n";
	for (int i = 0; i < 3; i++)
	{
		if (hilos[i].joinable()) {
			hilos[i].join();
		}
	}
	*/
}

void comprobar_credencial_en_servidor (credencial &cred)
{
	cred.resultado = usuario::fallido;
	cred.fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (cred.fd == -1) {
		std::cout << "Fallo socket: " << strerror(errno) << "\n";
		return;
	}

	std::cout << "Conectado a " << cred.direccion << ":" << cred.puerto << "\n";
	sockaddr_in addr = {AF_INET, htons(cred.puerto), inet_addr(cred.direccion.c_str())};
	int r = connect (cred.fd, (sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (r == -1) {
		std::cout << "Fallo connect: " << strerror(errno) << "\n";
		return;
	} else {
		std::cout << "Conectado a servidor\n";
	}


	std::stringstream ss;
	ss << cred.usuario << "," << cred.clave;
	std::string s = ss.str();
	if (!escribir (cred.fd, s)) {
		return;
	}
	
	if (leer_resultado_autenticacion (cred.fd, cred.resultado, cred.jugadores, cred.orden)) {
		switch (cred.resultado) {
			case usuario::aceptado:
				std::cout << "Respuesta: aceptado\n";
				break;
			case usuario::rechazado:
				std::cout << "Respuesta: rechazado\n";
				break;
			case usuario::cupo:
				std::cout << "Respuesta: cupo\n";
				break;
			case usuario::jugando:
				std::cout << "Respuesta: jugando\n";
				break;
			case usuario::fallido:
				std::cout << "Respuesta: fallido\n";
				break;
		};
		return;
	}
}

void enviar_ok (int fd)
{
	std::string ok = "ok";
	escribir (fd, ok);
}

void esperar_ok (int fd)
{
	std::string ok;
	leer (fd, ok);
}







