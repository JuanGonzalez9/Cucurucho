#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <list>
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

static bool separar (char *t, std::string & a, std::string & b)
{
	char * u = t;
	char * c = u;
	while (*c && *c != ',') {
		c++;
	}
	if (*c != ',') {
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

static bool leer_credencial (int fd, std::string & usuario, std::string & clave)
{
	// Hilo secundario
	std::cout << "Recibiendo credencial en fd: " << fd << "\n";
	const int l = 64;
	char msg[l+1];
	int leidos = 0, r;
	while (leidos < l && (r = recv (fd, msg+leidos, l-leidos, 0)) > 0) {
		leidos += r;
		if (msg[leidos-1] == '\0') {
			break;
		}
	}
	if (r == -1) {
		std::cout << "Fallo recv: " << strerror(errno) << "\n";
		return false;
	}
	msg[leidos] = '\0';
	std::cout << "Leido: " << msg << "\n";
	if (!separar (msg, usuario, clave)) {
		std::cout << "El cliente no envio una credencial\n";
		return false;		
	}
	return true;
}

static void escribir_resultado_autenticacion (int fd, const char * msg, int jugadores)
{
	// Hilo secundario
	std::stringstream ss;
	ss << msg << "," << jugadores;
	std::string s = ss.str();
	msg = s.c_str();
	std::cout << "Escribiendo '" << msg << "' en fd: " << fd << "\n";
	const int l = strlen (msg)+1;
	int escritos = 0, r;
	while (escritos < l && (r = send (fd, msg+escritos, l-escritos, 0)) > 0) {
		escritos += r;
	}
	if (r == -1) {
		std::cout << "Fallo send: " << strerror(errno) << "\n";
	}
}

static bool leer_resultado_autenticacion (int fd, usuario::estado & estado, int & jugadores)
{
	// Hilo secundario
	std::cout << "Recibiendo resultado en fd: " << fd << "\n";
	const int l = 64;
	char msg[l+1];
	int leidos = 0, r;
	while (leidos < l && (r = recv (fd, msg+leidos, l-leidos, 0)) > 0) {
		leidos += r;
		if (msg[leidos-1] == '\0') {
			break;
		}
	}
	if (r == -1) {
		std::cout << "Fallo recv: " << strerror(errno) << "\n";
		return false;
	}
	msg[leidos] = '\0';
	std::cout << "Recibido: " << msg << "\n";
	
	std::string e, j;
	if (!separar (msg, e, j)) {
		std::cout << "El servidor no envio una respuesta al intento de login\n";
		return false;		
	}
	try {
		jugadores = std::stoi (j);
		if (e == "aceptado") {
			estado = usuario::aceptado;
		} else if (e == "rechazado") {
			estado = usuario::rechazado;
		} else if (e == "cupo") {
			estado = usuario::cupo;
		} else {
			return false;
		}
	} catch (...) {
		std::cout << "El servidor no envio una respuesta al intento de login\n";
		return false;
	}
	return true;
}

static usuario::estado autenticar_usuario (autenticados *a, std::string & usuario, std::string & clave, int fd)
{
	std::stringstream camino;
	camino << "//configuracion//usuarios//" << usuario;
	std::cout << "Buscando cadena en " << camino.str() << "\n";
	std::string c = cfg.obtener_s (camino.str().c_str(), [](std::string & cadena, bool omision) {return true;});
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
		if (a->cantidad < a->requeridos) {
			a->usuarios[a->cantidad].fd = fd;
			a->usuarios[a->cantidad].nombre = usuario;
			a->usuarios[a->cantidad].esperando_ok = true;
			a->cantidad++;
			std::cout << "Aceptado: " << usuario << "\n";
			return usuario::aceptado;
		} else {
			for (int i = 0; i < a->cantidad; i++) {
				std::cout << "Comparando '" << usuario << "' con '" << a->usuarios[i].nombre << "'\n";
				if (a->usuarios[i].nombre == usuario) {
					a->usuarios[i].fd = fd;
					a->usuarios[i].nombre = usuario;
					a->usuarios[i].esperando_ok = true;
					std::cout << "Reconectando: " << usuario << "\n";
					return usuario::aceptado;
				}
			}
			// TODO aceptar a usuario ya registrado
			std::cout << "Cupo: " << usuario << "\n";
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
	if (!leer_credencial (fd, usuario, clave) )
	{
		return;
	}
	switch (autenticar_usuario (a, usuario, clave, fd) ) {
		case usuario::aceptado: {
			escribir_resultado_autenticacion (fd, "aceptado", jugadores);
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
						std::cout << "enviando ok a " << a->usuarios[i].nombre << " en fd " << a->usuarios[i].fd << "\n";
						a->usuarios[i].esperando_ok = false;
						enviar_ok (a->usuarios[i].fd);
					}
				}
				// TODO: iniciar hilo que se comunique con los clientes aceptados
			}
			break;
		}
		case usuario::rechazado:
			escribir_resultado_autenticacion (fd, "rechazado", jugadores);
			break;
		case usuario::cupo:
			escribir_resultado_autenticacion (fd, "cupo", jugadores);
			break;
		case usuario::fallido:
			escribir_resultado_autenticacion (fd, "fallido", jugadores);
			break;
	};
	std::cout << "Termino la autenticacion\n";
}

void escuchar (autenticados *a, int puerto, int jugadores)
{
	int fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd == -1) {
		std::cout << "Fallo socket: " << strerror(errno) << "\n";
		return;
	}

	sockaddr_in addr = {AF_INET, htons(puerto), inet_addr("127.0.0.1")};
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

int comprobar_credencial_en_servidor (int puerto, const std::string &usuario, const std::string &clave,
							usuario::estado & estado, int & jugadores)
{
	estado = usuario::fallido;
	int fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd == -1) {
		std::cout << "Fallo socket: " << strerror(errno) << "\n";
		return fd;
	}

	sockaddr_in addr = {AF_INET, htons(puerto), inet_addr("127.0.0.1")};
	int r = connect (fd, (sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (r == -1) {
		std::cout << "Fallo connect: " << strerror(errno) << "\n";
		return fd;
	} else {
		std::cout << "Conectado a servidor\n";
	}
	
	std::stringstream ss;
	ss << usuario << "," << clave;
	std::string s = ss.str();
	std::cout << "Escribiendo: " << s.c_str() << "\n";
	const char *msg = s.c_str();
	const int l = strlen(msg)+1;
	int escritos = 0;
	while (escritos < l && (r = send (fd, msg+escritos, l-escritos, 0)) != -1) {
		escritos += r;
	}
	if (r == -1) {
		std::cout << "Fallo send: " << strerror(errno) << "\n";
		return fd;
	}
	std::cout << "Escrito: " << msg << "\n";
	
	if (leer_resultado_autenticacion (fd, estado, jugadores)) {
		switch (estado) {
			case usuario::aceptado:
				std::cout << "Respuesta: aceptado\n";
				break;
			case usuario::rechazado:
				std::cout << "Respuesta: rechazado\n";
				break;
			case usuario::cupo:
				std::cout << "Respuesta: cupo\n";
				break;
		};
		return fd;
	}
	return fd;
}

void enviar_ok (int fd)
{
	const int l = 1;
	char msg[l];
	int escritos = 0, r;
	while (escritos == 0 && (r = send (fd, msg, l, 0)) != -1) {
		escritos += r;
	}
	if (r == -1) {
		std::cout << "Fallo send: " << strerror(errno) << "\n";
	}
}

void esperar_ok (int fd)
{
	const int l = 1;
	char msg[l];
	int leidos = 0, r;
	while (leidos == 0 && (r = recv (fd, msg, l, 0)) > 0) {
		leidos += r;
	}
	if (r == -1) {
		std::cout << "Fallo recv: " << strerror(errno) << "\n";
	}
}

/*
 Comprobar que devuelva cero recv
 Pensar que hacer al fallar bind por TIME_WAIT
 Poder leer y escribir cadenas de longitud indeterminada
 */






