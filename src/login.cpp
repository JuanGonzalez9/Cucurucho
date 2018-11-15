#include <iostream>
#include <vector>
#include <thread>
#include "login.hpp"
#include "conector.hpp"

static const int tiempo_mensaje = 4*60; // 4 segundos
extern bool obtener_dir_puerto (char *arg, std::string &dir, unsigned short &puerto);

vlogin::vlogin (ventana &v, const std::string &d):
	dialogo (v),
	pnl (10, 400, 780, 190),
	mensaje (20, 20, 16),
	usuario (10, 55, 12),
	clave (10, 95, 12),
	direccion (10, 135, 21),
	aceptar (10, 135, "Aceptar"),
	duracion_mensaje (-1),
	mensaje_omision ("Ingrese nombre de usuario y clave, luego presione ENTER."),
	autenticando (false),
	esperando_ok (false),
	fd (-1)
{
	usuario.texto (nullptr, "usuario");
	pnl.agregar (&usuario, false);

	clave.texto (nullptr, "clave");
	clave.ocultar = true;
	pnl.agregar (&clave, false);

	direccion.texto (d.c_str(), "direccion:puerto");
	pnl.agregar (&direccion, false);
	
	aceptar.anclado_x = control::opuesto;
	aceptar.al_presionar_callback = [this]() {al_aceptar();};
	pnl.agregar (&aceptar, false);

	info (mensaje_omision.c_str(), tiempo_infinito);
	pnl.agregar (&mensaje, false);

	agregar (&pnl, false);
	
	popular_enfocables ();
}

vlogin::~vlogin()
{
}

void vlogin::correr ()
{
	inicializar_credencial ();
	dialogo::correr ();
}

void vlogin::error (const char *msg, int duracion)
{
	duracion_mensaje = duracion;
	mensaje.color ({200, 25, 25, alpha});
	mensaje.texto (msg);
}

void vlogin::info (const char *msg, int duracion)
{
	duracion_mensaje = duracion;
	mensaje.color ({200, 200, 200, alpha});
	mensaje.texto (msg);
}

void vlogin::manejar_eventos()
{
	if (duracion_mensaje >= 0) {
		duracion_mensaje--;
		if (duracion_mensaje == tiempo_infinito) {
			info (mensaje_omision.c_str());
		}
	}
	dialogo::manejar_eventos();
}

bool vlogin::manejar_evento (SDL_Event e)
{
	switch (e.type) {
		case SDL_QUIT:
			if (esperando_ok) {
				corriendo = false;
				cred.arranca_grisado = true;
				hilo.detach ();
				cred.resultado = usuario::fallido;
			} else if (!autenticando || esperando_ok) {
				corriendo = false;
			}
			return true;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					if (!autenticando) {
						al_aceptar ();
						return true;
					}
				case SDLK_ESCAPE:
					if (!autenticando) {
						corriendo = false;
						return true;
					}
			};
	}
	return dialogo::manejar_evento (e);
}

void vlogin::actualizar ()
{
	dialogo::actualizar ();
}

void vlogin::dibujar ()
{
	dialogo::dibujar ();
}

void vlogin::al_aceptar ()
{
	std::string u = usuario.texto ();
	std::string c = clave.texto ();
	const char *ws = " \t\n\r\v\f";
	u.erase(u.find_last_not_of(ws) + 1);
	u.erase(0, u.find_first_not_of(ws));
	usuario.texto (u.c_str(), nullptr);
	if (u.length() == 0) {
		error ("Debe especificar un nombre de usuario.", tiempo_mensaje);
	} else if (c.length() == 0) {
		error ("Debe especificar una clave de usuario.", tiempo_mensaje);
	} else if (c.length() < MIN_CLAVE || c.length() > MAX_CLAVE) {
		error ("La clave debe tener entre " STR(MIN_CLAVE) " y " STR(MAX_CLAVE) " caracteres.", tiempo_mensaje);
	} else if (!inicializar_credencial ()) {
		error ("Debe especificar una direccion:puerto valida.", tiempo_mensaje);
	} else {
		info ("Comprobando credencial, espere por favor...", tiempo_infinito);
		aceptar.activo = false;
		usuario.activo = clave.activo = direccion.activo = false;
		autenticando = true;
		esperando_ok = false;
		hilo = std::thread{ comprobar_credencial, this };
	}
}

bool vlogin::inicializar_credencial ()
{
	cred.fd = -1;
	cred.resultado = usuario::rechazado;
	cred.usuario = usuario.texto ();
	cred.clave = clave.texto ();

	std::string s = direccion.texto ();
	std::vector<char> buffer (s.begin (), s.end ());
	buffer.push_back ('\0');
	return obtener_dir_puerto (&buffer[0], cred.direccion, cred.puerto);
}

void vlogin::comprobar_credencial (vlogin *vl)
{
	// Hilo secundario
	comprobar_credencial_en_servidor (vl->cred);
	if (vl->cred.resultado == usuario::aceptado || vl->cred.resultado == usuario::reaceptado) {
		std::cout << "enviando ok\n";
		if (enviar_ok (vl->cred.fd)) {
			// Solo cuando se envía el ok se que el usuario estará en la lista de jugadores del servidor.
			vl->sincronizada ([vl](){vl->al_ser_aceptado ();});
			std::cout << "esperando ok para " << vl->cred.usuario << " en fd " << vl->cred.fd << "\n";
			// Cuando el servidor no puede enviar el ok, deja al usuario en la lista y lo arranca grisado.
			// Hago que se muestre el diálogo de reconexión.
			vl->cred.arranca_grisado = !esperar_ok (vl->cred.fd);
			std::cout << "ok recibido\n";
		} else {
			// Como no se pudo enviar el ok informo el fallo.
			vl->cred.resultado = usuario::fallido;
		}
	}
	vl->sincronizada ([vl](){vl->al_terminar_hilo ();}, false);
}

void vlogin::al_ser_aceptado ()
{
	info ("Espere el inicio de la partida.", tiempo_infinito);
	aceptar.activo = false;
	usuario.activo = clave.activo = direccion.activo = false;
	esperando_ok = true;
}

void vlogin::al_terminar_hilo ()
{
	std::cout << "al_terminar_hilo\n";
	autenticando = false;
	hilo.join ();
	switch (cred.resultado) {
		case usuario::aceptado:
		case usuario::reaceptado:
			std::cout << "Credencial aceptada\n";
			corriendo = false;
			break;
		case usuario::rechazado:
			std::cout << "Credencial inválida\n";
			error ("Credencial invalida.", tiempo_mensaje);
			aceptar.activo = true;
			usuario.activo = clave.activo = direccion.activo = true;
			break;
		case usuario::cupo:
			std::cout << "La partida ya está llena\n";
			info ("La partida ya esta llena.", tiempo_infinito);
			aceptar.activo = false;
			usuario.activo = clave.activo = direccion.activo = false;
			break;
		case usuario::jugando:
			std::cout << "El usuario ya está jugando\n";
			info ("El usuario ya esta jugando.", tiempo_mensaje);
			aceptar.activo = true;
			usuario.activo = clave.activo = direccion.activo = true;
			break;
		case usuario::fallido:
			std::cout << "Falló la conexión, inténtelo más tarde\n";
			error ("Fallo la conexion, intentelo mas tarde.", tiempo_mensaje);
			aceptar.activo = true;
			usuario.activo = clave.activo = direccion.activo = true;
			break;
	};
}

void interrumpir_servidor (autenticados &a)
{
	std::unique_lock<std::mutex> lock(a.mutex);
	a.salir = true;
	a.condicion.notify_one();
}

void esperar_jugadores (int jugadores, const char *dir, unsigned short puerto, autenticados &a)
{
	std::unique_lock<std::mutex> lock(a.mutex);
	a.cantidad = 0;
	a.requeridos = jugadores;
	a.comenzo = false;
	for (int i = 0; i < 4; i++) {
		a.usuarios[i].conector = nullptr;
	}
	int fd = abrir_socket (dir, puerto);
	if (fd == -1) {
		a.salir = true;
		return;
	}
	a.hilo_1 = std::thread{ escuchar, &a, fd, jugadores };
	if (std::string ("127.0.0.1") != dir) {
		int fd = abrir_socket ("127.0.0.1", puerto);
		if (fd == -1) {
			a.salir = true;
			return;
		}
		a.hilo_2 = std::thread{ escuchar, &a, fd, jugadores };
	}
	std::cout << "Esperando que se cumpla el cupo de jugadores\n";
	a.condicion.wait (lock, [&a]{return a.salir || a.cantidad == a.requeridos;});
	if (a.cantidad == a.requeridos) {
		std::cout << "Cupo de jugadores alcanzado\n";
	} else {
		std::cout << "Se interrumpio el inicio de la partida\n";
	}
}

