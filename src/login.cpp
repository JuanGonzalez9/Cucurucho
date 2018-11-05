#include <iostream>
#include <vector>
#include <thread>
#include "login.hpp"
#include "conector.hpp"

static const int tiempo_mensaje = 4*60; // 4 segundos
extern bool obtener_dir_puerto (char *arg, std::string &dir, unsigned short &puerto);

vlogin::vlogin (ventana &v, const std::string &d):
	contenedor (v),
	usuario (20, 120, 12),
	clave (20, 70, 12),
	direccion (20, 20, 21),
	mensaje (20, 170, 16),
	duracion_mensaje (-1),
	mensaje_omision ("Ingrese nombre de usuario y clave, luego presione ENTER."),
	autenticando (false),
	fd (-1)
{
	usuario.anclado_y = editor::opuesto;
	usuario.texto (nullptr, "usuario");
	agregar (&usuario, false);

	clave.anclado_y = editor::opuesto;
	clave.texto (nullptr, "clave");
	clave.ocultar = true;
	agregar (&clave, false);

	direccion.anclado_y = editor::opuesto;
	direccion.texto (d.c_str(), "direccion");
	agregar (&direccion, false);
	
	mensaje.anclado_y = editor::opuesto;
	mensaje.texto (mensaje_omision.c_str());
	agregar (&mensaje, false);
	
	enfocar (&usuario);
}

vlogin::~vlogin()
{
}

void vlogin::correr ()
{
	inicializar_credencial ();
	contenedor::correr ();
}

void vlogin::error (const char *msg, int duracion)
{
	duracion_mensaje = duracion;
	mensaje.color ({150, 5, 5, 255});
	mensaje.texto (msg);
}

void vlogin::info (const char *msg, int duracion)
{
	duracion_mensaje = duracion;
	mensaje.color ({15, 15, 15, 255});
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
	ejecutar_sincronizada ();
	contenedor::manejar_eventos();
}

void vlogin::manejar_evento (SDL_Event e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
				case SDLK_RETURN:
					al_aceptar ();
					return;
			};
		      break;
	}
	contenedor::manejar_evento (e);
}

void vlogin::actualizar()
{
}

void vlogin::dibujar()
{
	SDL_SetRenderDrawColor (v.renderer (), 160, 180, 250, 255);
	SDL_RenderClear (v.renderer ());
	contenedor::dibujar ();
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
	} else {
		info ("Comprobando credencial, espere por favor...", tiempo_infinito);
		/*
		gtk_widget_set_sensitive (login->aceptar, false);
		gtk_widget_set_sensitive (login->cancelar, false);
		gtk_widget_set_sensitive (login->editor_usuario, false);
		gtk_widget_set_sensitive (login->editor_clave, false);
		*/
		autenticando = true;
		inicializar_credencial ();
		hilo = std::thread{ comprobar_credencial, this };
	}
}

void vlogin::inicializar_credencial ()
{
	cred.fd = -1;
	cred.resultado = usuario::rechazado;
	cred.usuario = usuario.texto ();
	cred.clave = clave.texto ();

	std::string s = direccion.texto ();
	std::vector<char> buffer (s.begin (), s.end ());
	buffer.push_back ('\0');
	if (!obtener_dir_puerto (&buffer[0], cred.direccion, cred.puerto)) {
		// TODO impedir
		std::cout << "El usuario no puso una direccion valida\n";
	}
}

void vlogin::comprobar_credencial (vlogin *vl)
{
	// Hilo secundario
	comprobar_credencial_en_servidor (vl->cred);
	if (vl->cred.resultado == usuario::aceptado) {
		vl->sincronizada ([vl](){vl->al_ser_aceptado ();});
		std::cout << "enviando ok\n";
		enviar_ok (vl->cred.fd);
		std::cout << "esperando ok para " << vl->cred.usuario << " en fd " << vl->cred.fd << "\n";
		esperar_ok (vl->cred.fd);
		std::cout << "ok recibido\n";
	}
	vl->sincronizada ([vl](){vl->al_terminar_hilo ();});
}

void vlogin::sincronizada (std::function<void()> funcion)
{
	std::unique_lock<std::mutex> lock(mutex_sinc);
	//std::cout << "Espero ejecucion de funcion sincronizada\n";
	funcion_sinc = funcion;
	ejecutada_sinc = false;
	condicion_sinc.wait (lock, [this]{return this->ejecutada_sinc;});
	//std::cout << "Fin: Espero ejecucion de funcion sincronizada\n";
}

void vlogin::ejecutar_sincronizada ()
{
	std::unique_lock<std::mutex> lock(mutex_sinc);
	if (funcion_sinc) {
		//std::cout << "Ejecuto funcion sincronizada\n";
		funcion_sinc ();
		funcion_sinc = nullptr;
		ejecutada_sinc = true;
		condicion_sinc.notify_all ();
		//std::cout << "Fin: Ejecuto funcion sincronizada\n";
	}
}

void vlogin::al_ser_aceptado ()
{
	std::string u = usuario.texto ();
	info ("Espere el inicio de la partida.", tiempo_infinito);
	/*
	gtk_widget_set_sensitive (login->aceptar, false);
	gtk_widget_set_sensitive (login->cancelar, false);
	gtk_widget_set_sensitive (login->editor_usuario, false);
	gtk_widget_set_sensitive (login->editor_clave, false);
	*/
}

void vlogin::al_terminar_hilo ()
{
	std::cout << "al_terminar_hilo\n";
	autenticando = false;
	hilo.detach (); // Se que saldra
	switch (cred.resultado) {
		case usuario::aceptado:
			std::cout << "Credencial aceptada\n";
			corriendo = false;
			break;
		case usuario::rechazado:
			std::cout << "Credencial inválida\n";
			error ("Credencial invalida.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			#endif
			break;
		case usuario::cupo:
			std::cout << "La partida ya está llena\n";
			info ("La partida ya esta llena.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			#endif
			break;
		case usuario::jugando:
			std::cout << "El usuario ya está jugando\n";
			info ("El usuario ya esta jugando.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			#endif
			break;
		case usuario::fallido:
			std::cout << "Falló la conexión, inténtelo más tarde\n";
			error ("Fallo la conexion, intentelo mas tarde.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			#endif
			break;
	};
}

void esperar_jugadores (int jugadores, const char *dir, unsigned short puerto, autenticados &a)
{
	a.cantidad = 0;
	a.requeridos = jugadores;
	a.comenzo = false;
	a.hilo = std::thread{ escuchar, &a, dir, puerto, jugadores };
	std::unique_lock<std::mutex> lock(a.mutex);
	std::cout << "Esperando que se cumpla el cupo de jugadores\n";
	a.condicion.wait (lock, [&a]{return a.cantidad == a.requeridos;});
	std::cout << "Cupo de jugadores alcanzado\n";
}

