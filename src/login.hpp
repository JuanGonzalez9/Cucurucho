#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <thread>
#include <mutex>
#include <condition_variable>

extern "C"
{
	#include <gtk/gtk.h>
	#include <gdk/gdkkeysyms.h>
}

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MIN_CLAVE 4
#define MAX_CLAVE 12

struct usuario
{
	std::string nombre;
	int fd;
	bool esperando_ok;
	typedef enum {aceptado, rechazado, cupo, fallido} estado;
};

struct autenticados
{
	usuario usuarios[4];
	int cantidad, requeridos;
	bool comenzo; // comenzo el juego por alcanzar el cupo
	std::mutex mutex;
	std::thread hilo;
	std::condition_variable condicion;
};

typedef struct
{
	GtkApplication *app;
	GtkWidget *ventana;
	GtkWidget *etiqueta_usuario, *etiqueta_clave, *etiqueta_error;
	GtkWidget *editor_usuario, *editor_clave;
	GtkWidget *aceptar, *cancelar;
	guint id_temporizador;
	bool autenticando;
	std::string usuario, clave;
	std::thread hilo;
	usuario::estado resultado;
	int jugadores, fd;
	unsigned short puerto;
	const char *dir;
} ventana_login;

bool login (const char *dir, unsigned short puerto, int &fd, int &jugadores, ventana_login &login);
void esperar_jugadores (int jugadores, const char *dir, unsigned short puerto, autenticados &a);

#endif

