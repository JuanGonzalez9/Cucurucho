#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
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
	leidos = 0;
	int r;
	std::unique_ptr<char> buffer (new char[largo]);
	char *b = buffer.get ();
	while (leidos < largo && (r = recv (fd, b + leidos, largo - leidos, 0)) > 0) {
		leidos += r;
	}
	if (leidos < largo) {
		return false;
	}
	b[leidos-1] = '\0';
	s = b;
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
	if (!leer (fd, e)) {
		std::cout << "Por falla en la conexion no se recibio el resultado de la autenticacion\n";
		return false;		
	}
	if (!separar (e, j, ',')) {
		std::cout << "El servidor no envio una respuesta al intento de login\n";
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
		} else if (e == "reaceptado") {
			estado = usuario::reaceptado;
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
					std::cout << "Reaceptado: " << usuario << "\n";
					return usuario::reaceptado;
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

static bool recibio_ok (autenticados *a, int orden, bool eliminar)
{
	// Antes de llamar a esta funcion debe protegerse con un a->mutex a 'a'.
	std::cout << "esperando ok\n";
	if (!esperar_ok (a->usuarios[orden].fd)) { // cancelo o fallo la conexion
		if (eliminar) {
			// Solo cuando se recibe el ok el usuario debe estar en la lista de jugadores del servidor. Lo remuevo.
			std::cout << "Cancelado: " << a->usuarios[orden].nombre << "\n";
			for (int i = orden; i < a->cantidad-1; i++) {
				memcpy (&a->usuarios[i+1], &a->usuarios[i], sizeof (struct usuario) );
			}
			a->cantidad--;
		}
		cerrar (a->usuarios[orden].fd);
		a->usuarios[orden].fd = -1;
		return false;
	}
	std::cout << "ok recibido\n";
	return true;
}

static void comprobar_credencial (autenticados *a, int fd, int jugadores)
{
	// Hilo secundario
	std::string usuario, clave;
	int orden = 0;
	if (!leer_credencial (fd, usuario, clave) )
	{
		cerrar (fd);
		return;
	}
	switch (autenticar_usuario (a, usuario, clave, orden, fd) ) {
		case usuario::aceptado: {
			escribir_resultado_autenticacion ("aceptado", fd, orden, jugadores);

			std::lock_guard<std::mutex> lock(a->mutex);
			if (recibio_ok (a, orden, true) && a->cantidad == a->requeridos) {
				std::cout << "Cupo alcanzado\n";
				if (!a->comenzo) {
					a->comenzo = true;
					std::cout << "Notifico que se cumplió el cupo de jugadores\n";
					a->condicion.notify_one();
				}
				for (int i = 0; i < a->cantidad; i++) {
					if (a->usuarios[i].esperando_ok) {
						std::cout << "enviando ok a " << a->usuarios[i].nombre << " en fd " << a->usuarios[i].fd << "\n";
						// Ya está en la lista, si la conexión falla en este último paso, tiene que empezar grisado.
						a->usuarios[i].arranca_grisado = !enviar_ok (a->usuarios[i].fd);
						std::cout << "ok enviado\n";
						a->usuarios[i].esperando_ok = false;
					}
				}
			}
			break;
		}
		case usuario::reaceptado: {
			escribir_resultado_autenticacion ("reaceptado", fd, orden, jugadores);

			std::lock_guard<std::mutex> lock(a->mutex);
			if (recibio_ok (a, orden, false)) {
				for (int i = 0; i < a->cantidad; i++) {
					if (a->usuarios[i].esperando_ok) {
						std::cout << "enviando ok a " << a->usuarios[i].nombre << " en fd " << a->usuarios[i].fd << "\n";
						// Ya está en la lista, si la conexión falla en este último paso, tiene que empezar grisado.
						a->usuarios[i].arranca_grisado = !enviar_ok (a->usuarios[i].fd);
						std::cout << "ok enviado\n";
						a->usuarios[i].esperando_ok = false;
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

int abrir_socket (const char *dir, int puerto)
{
	int fd = -1;
	bool escuchando = false;
	int intento = 1;
	int p = puerto;
	while (!escuchando && intento++ <= MAX_PUERTOS_SERVIDOR) {
		fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
		if (fd == -1) {
			std::cout << "Fallo socket: " << strerror(errno) << "\n";
			break;
		}

		sockaddr_in addr = {AF_INET, htons(p), inet_addr(dir)};
		int r = bind (fd, (sockaddr*)&addr, sizeof(struct sockaddr_in));
		if (r == -1) {
			cerrar (fd);
			if (errno == EADDRINUSE) {
				p++;
				continue;
			}
			std::cout << "Fallo bind: " << strerror(errno) << "\n";
			break;
		}

		r = listen (fd, SOMAXCONN);
		if (r == -1) {
			cerrar (fd);
			if (errno == EADDRINUSE) {
				p++;
				continue;
			}
			std::cout << "Fallo listen: " << strerror(errno) << "\n";
			break;
		}
		escuchando = true;
	}
	if (escuchando) {
		std::cout << "Arranco el servidor en " << dir << ":" << p << "\n";
		return fd;
	} else {
		std::cout << "Fallo apertura de socket en " << dir << ":" << puerto << "-" << puerto + MAX_PUERTOS_SERVIDOR - 1 << "\n";
		return -1;
	}
}

void escuchar (autenticados *a, int fd, int jugadores)
{
	while (true) {
		int fd_nuevo = accept(fd, nullptr, nullptr);
		if (fd_nuevo == -1) {
			switch (errno) {
				case EINTR:
					usleep (100000);
					continue;
				default:
					std::cout << "Fallo accept: " << strerror(errno) << "\n";
					interrumpir_servidor (*a);
					return;
			};
		} else {
			std::cout << "Cliente conectado\n";
			std::thread hilo = std::thread{ comprobar_credencial, a, fd_nuevo, jugadores };
			hilo.detach ();
		}
	}
}

void comprobar_credencial_en_servidor (credencial &cred)
{  
 
    cred.resultado = usuario::fallido;
 
    std::stringstream comando;
    comando<<"ping -c 1 "<<cred.direccion<< ">/dev/null 2>&1";
    int resultado = system(comando.str().c_str());
    printf("El resultado del ping es: %d\n", resultado );
 
    if (resultado){
        return;
    }
 
    cred.fd = socket (AF_INET, SOCK_STREAM | SOCK_CLOEXEC , 0);
    if (cred.fd == -1) {
        std::cout << "Fallo socket: " << strerror(errno) << "\n";
        return;
    }
 
 
    sockaddr_in addr = {AF_INET, htons(cred.puerto), inet_addr(cred.direccion.c_str())};
    int r = connect (cred.fd, (sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (r == -1) {
        std::cout << "Fallo connect: " << strerror(errno) << "\n";
        cerrar (cred.fd);
        return;
    } else {
        std::cout << "Conectado a " << cred.direccion << ":" << cred.puerto << "\n";
    }
 
    std::stringstream ss;
    ss << cred.usuario << "," << cred.clave;
    std::string s = ss.str();
    if (!escribir (cred.fd, s)) {
        cerrar (cred.fd);
        return;
    }
   
    if (leer_resultado_autenticacion (cred.fd, cred.resultado, cred.jugadores, cred.orden)) {
        switch (cred.resultado) {
            case usuario::aceptado:
                std::cout << "Respuesta: aceptado\n";
                return;
            case usuario::reaceptado:
                std::cout << "Respuesta: reaceptado\n";
                return;
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
    }
    cerrar (cred.fd);
}

bool enviar_ok (int fd)
{
	std::string ok = "ok";
	return escribir (fd, ok);
}

bool enviar_cancelar (int fd)
{
	std::string cancelar = "cancelar";
	return escribir (fd, cancelar);
}

bool esperar_ok (int fd)
{
	std::string ok;
	return leer (fd, ok) && ok == "ok";
}

void cerrar (int &fd)
{
	shutdown (fd, SHUT_RDWR);
	close (fd);
	fd = -1;
}





