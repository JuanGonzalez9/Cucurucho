#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "temporizador.hpp"
#include "editor.hpp"
#include "etiqueta.hpp"
#include "panel.hpp"
#include "dialogo.hpp"
#include "boton.hpp"
#include "Constantes.h"
#include "Socket.h"

extern "C"
{
	#include <sys/types.h>
	#include <sys/socket.h>
}

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MIN_CLAVE 4
#define MAX_CLAVE 12

struct usuario
{
	std::string nombre;
	int fd;
	bool esperando_ok, conectado;
	int recien_conectado;
	std::mutex mutex_tmp;
	temporizador tmp;
	Socket *conector;
	std::thread hilo;
	std::mutex mutex_teclas;
	char teclas[TAMANIO_MENSAJE_TECLAS + 1];
	typedef enum {aceptado, reaceptado, rechazado, cupo, jugando, fallido} estado;
};

struct credencial
{
	unsigned short puerto;
	std::string usuario, clave, direccion;
	int jugadores, fd, orden;
	usuario::estado resultado;
};

struct autenticados
{
	usuario usuarios[4];
	int cantidad, requeridos;
	bool comenzo; // comenzo el juego por alcanzar el cupo
	std::mutex mutex;
	std::thread hilo;
	std::condition_variable condicion;
	std::mutex mutex_mundo;
	std::string mundo;		
};

class vlogin: public dialogo
{
public:
	vlogin (ventana &v, const std::string &direccion);
	virtual ~vlogin();
	virtual void correr();
	virtual void manejar_eventos();
	virtual bool manejar_evento (SDL_Event e);
	virtual void actualizar ();
	virtual void dibujar ();
	void al_aceptar ();
	void al_ser_aceptado ();
	void al_terminar_hilo ();
	static void comprobar_credencial (vlogin *vl);
	credencial cred;
	static const int tiempo_infinito = -1;
protected:
	panel pnl;
	etiqueta mensaje;
	editor usuario, clave, direccion;
	boton aceptar;
	int duracion_mensaje;
	std::string mensaje_omision;
	bool autenticando;
	std::thread hilo;
	int fd;
	bool inicializar_credencial ();
	void error (const char *msg, int duracion = tiempo_infinito);
	void info (const char *msg, int duracion = tiempo_infinito);
};

void esperar_jugadores (int jugadores, const char *dir, unsigned short puerto, autenticados &a);

#endif

