#include <iostream>
#include <thread>
#include "login.hpp"
#include "conector.hpp"

static gboolean al_cumplir_tiempo (gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	gtk_label_set_text (GTK_LABEL(login->etiqueta_error), "");
	login->id_temporizador = 0;
	return false;
}

static void mostrar_error (ventana_login *login, const char *msg)
{
	GdkColor color;
	color.red = 40000;
	color.green = 0;
	color.blue = 0;
	gtk_widget_modify_fg (login->etiqueta_error, GTK_STATE_NORMAL, &color);
	gtk_label_set_text (GTK_LABEL(login->etiqueta_error), msg);
	if( login->id_temporizador > 0 ) {
		g_source_remove (login->id_temporizador);
	}
	login->id_temporizador = g_timeout_add_seconds (7, al_cumplir_tiempo, login);
}

static void mostrar_info (ventana_login *login, const char *msg)
{
	GdkColor color;
	color.red = 0;
	color.green = 0;
	color.blue = 0;
	gtk_widget_modify_fg (login->etiqueta_error, GTK_STATE_NORMAL, &color);
	gtk_label_set_text (GTK_LABEL(login->etiqueta_error), msg);
	if( login->id_temporizador > 0 ) {
		g_source_remove (login->id_temporizador);
		login->id_temporizador = 0;
	}
}

static void ocultar_info (ventana_login *login, const char *msg)
{
	gtk_label_set_text (GTK_LABEL(login->etiqueta_error), "");
	if( login->id_temporizador > 0 ) {
		g_source_remove (login->id_temporizador);
		login->id_temporizador = 0;
	}
}

static gboolean al_ser_aceptado (gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	std::string usuario = gtk_entry_get_text (GTK_ENTRY(login->editor_usuario));
	mostrar_info (login, "Espere el inicio de la partida");
	gtk_widget_set_sensitive (login->aceptar, false);
	gtk_widget_set_sensitive (login->cancelar, false);
	gtk_widget_set_sensitive (login->editor_usuario, false);
	gtk_widget_set_sensitive (login->editor_clave, false);
}

static gboolean al_terminar_hilo (gpointer data)
{
	std::cout << "al_terminar_hilo\n";
	ventana_login *login = (ventana_login*)data;
	login->autenticando = false;
	login->hilo.join ();
	switch (login->resultado) {
		case usuario::aceptado:
			std::cout << "aceptado1: " << sizeof(login->app) << " " << (void*)login->app << "\n";
			gtk_widget_destroy (login->ventana);
			std::cout << "aceptado2\n";
			break;
		case usuario::rechazado:
			mostrar_error (login, "Credencial inválida");
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			break;
		case usuario::cupo:
			mostrar_info (login, "La partida ya está llena");
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			break;
		case usuario::jugando:
			mostrar_info (login, "El usuario ya está jugando");
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			break;
		case usuario::fallido:
			mostrar_error (login, "Falló la conexión, inténtelo más tarde");
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			break;
	};
	return false;
}

static void comprobar_credencial (ventana_login *login)
{
	// Hilo secundario
	login->fd = comprobar_credencial_en_servidor (*login);
	if (login->resultado == usuario::aceptado) {
		gdk_threads_enter ();
		gdk_threads_add_idle (al_ser_aceptado, login);
		gdk_threads_leave ();
		std::cout << "esperando ok para " << login->usuario << " en fd " << login->fd << "\n";
		esperar_ok (login->fd);
		std::cout << "ok recibido\n";
	}
	gdk_threads_enter ();
	gdk_threads_add_idle (al_terminar_hilo, login);
	gdk_threads_leave ();
}

static void al_aceptar (GtkWidget *widget, gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	login->usuario = gtk_entry_get_text (GTK_ENTRY(login->editor_usuario));
	login->clave = gtk_entry_get_text (GTK_ENTRY(login->editor_clave));
	const char *ws = " \t\n\r\v\f";
	login->usuario.erase(login->usuario.find_last_not_of(ws) + 1);
	login->usuario.erase(0, login->usuario.find_first_not_of(ws));
	gtk_entry_set_text (GTK_ENTRY(login->editor_usuario), login->usuario.c_str());
	if (login->usuario.length() == 0) {
		mostrar_error (login, "Debe especificar un nombre de usuario");
	} else if (login->clave.length() == 0) {
		mostrar_error (login, "Debe especificar una clave de usuario");
	} else if (login->clave.length() < MIN_CLAVE || login->clave.length() > MAX_CLAVE) {
		mostrar_error (login, "La clave debe tener entre " STR(MIN_CLAVE) " y " STR(MAX_CLAVE) " caracteres");
	} else {
		mostrar_info (login, "Comprobando credencial, espere por favor...");
		gtk_widget_set_sensitive (login->aceptar, false);
		gtk_widget_set_sensitive (login->cancelar, false);
		gtk_widget_set_sensitive (login->editor_usuario, false);
		gtk_widget_set_sensitive (login->editor_clave, false);
		login->autenticando = true;
		login->hilo = std::thread{ comprobar_credencial, login };
	}
}

static void al_cancelar (GtkWidget *widget, gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	std::cout << "al_cancelar: " << login->autenticando << "\n";
	if (!login->autenticando) {
		gtk_widget_destroy (login->ventana);
		//g_application_quit (G_APPLICATION (login->app));
	}
}

static gboolean al_cerrar (GtkWidget *widget, GdkEvent *event, gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	std::cout << "al_cerrar: " << login->autenticando << "\n";
	return login->autenticando;
}

static gboolean al_presionar_tecla (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	ventana_login *login = (ventana_login*)data;
	if (event->keyval == GDK_KEY_Escape) {
		al_cancelar (widget, login);
		return true;
	} else if (event->keyval == GDK_KEY_Return) {
		al_aceptar (widget, login);
		return true;
	}
	return false;
}

static void activate (GtkApplication* app, gpointer user_data)
{
	ventana_login * login = (ventana_login*)user_data;
	
	login->autenticando = false;
	login->id_temporizador = 0;
	login->resultado = usuario::rechazado;

	int x = 20, y = 10, w = 385, h = 135;
	login->ventana = gtk_application_window_new (app);
	
	gtk_window_set_title (GTK_WINDOW (login->ventana), "Ingrese nombre de usuario y clave");
	gtk_window_set_default_size (GTK_WINDOW (login->ventana), w, h);
	gtk_window_set_resizable (GTK_WINDOW(login->ventana), false);
	g_signal_connect (GTK_WINDOW(login->ventana), "key_press_event", G_CALLBACK(al_presionar_tecla), login);
	g_signal_connect (GTK_WINDOW(login->ventana), "delete-event", G_CALLBACK(al_cerrar), login);

  	
	GtkWidget *fixed = gtk_fixed_new();

	login->etiqueta_usuario = gtk_label_new("Usuario");
	gtk_fixed_put(GTK_FIXED(fixed), login->etiqueta_usuario, x, y);

	login->etiqueta_clave = gtk_label_new("Clave");
	gtk_fixed_put(GTK_FIXED(fixed), login->etiqueta_clave, x+180, y);

	login->editor_usuario = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed), login->editor_usuario, x, y+20);
	gtk_widget_set_size_request(login->editor_usuario, 140, 35);

	login->editor_clave = gtk_entry_new();
	gtk_fixed_put(GTK_FIXED(fixed), login->editor_clave, x+180, y+20);
	gtk_widget_set_size_request(login->editor_clave, 140, 35);
	gtk_entry_set_max_length (GTK_ENTRY(login->editor_clave), MAX_CLAVE);
	gtk_entry_set_visibility (GTK_ENTRY(login->editor_clave), false);

	login->etiqueta_error = gtk_label_new("");
	gtk_fixed_put(GTK_FIXED(fixed), login->etiqueta_error, x, y+60);

	login->aceptar = gtk_button_new_with_label("Aceptar");
	gtk_fixed_put(GTK_FIXED(fixed), login->aceptar, x+175, y+85);
	gtk_widget_set_size_request(login->aceptar, 80, 30);

	login->cancelar = gtk_button_new_with_label("Cancelar");
	gtk_fixed_put(GTK_FIXED(fixed), login->cancelar, x+265, y+85);
	gtk_widget_set_size_request(login->cancelar, 80, 30);

	gtk_container_add (GTK_CONTAINER (login->ventana), fixed);
	
	g_signal_connect (login->aceptar, "clicked", G_CALLBACK (al_aceptar), login);
	g_signal_connect (login->cancelar, "clicked", G_CALLBACK (al_cancelar), login);

	gtk_window_set_position (GTK_WINDOW(login->ventana), GTK_WIN_POS_CENTER);
	gtk_widget_show_all (login->ventana);
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

bool login (const char *dir, unsigned short puerto, ventana_login &login)
{
	login.app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	login.puerto = puerto;
	login.dir = dir;
	std::cout << "login.app: " << sizeof(login.app) << " " << (void*)login.app << "\n";
	g_signal_connect (login.app, "activate", G_CALLBACK (activate), &login);
	g_application_run (G_APPLICATION (login.app), 0, nullptr);
	g_object_unref (login.app);
	std::cout << "sale login\n";
	return login.resultado == usuario::aceptado;
}


#if 0
#include <QStyle>
#include <QDesktopWidget>
#include <iostream>
#include "login.hpp"

login::login ()
{
}

login::~login ()
{
}
	
void login::al_aceptar (int e)
{
	std::cout << "aceptar\n";
}

void login::al_cancelar ()
{
	std::cout << "cancelar\n";
}

bool login::show (int argc, char *argv[])
{
	QApplication app (argc, argv);
	QWidget ventana;
	QLabel etiqueta_error (QObject::tr("Usuario y/o clave incorrectos"), &ventana);
	QLabel etiqueta_usuario (QObject::tr("Usuario"), &ventana);
	QLabel etiqueta_clave (QObject::tr("Clave"), &ventana);
	QLineEdit editor_usuario (&ventana);
	QLineEdit editor_clave (&ventana);
	QPushButton aceptar ("Aceptar", &ventana);
	QPushButton cancelar ("Cancelar", &ventana);
	
	ventana.setFixedSize (340, 130);
	ventana.setGeometry (QStyle::alignedRect (Qt::LeftToRight, Qt::AlignCenter, ventana.size(), qApp->desktop()->availableGeometry()));
    	ventana.setWindowTitle (QObject::tr("Ingrese nombre de usuario y clave"));

	int x = 20, y = 5;
	etiqueta_usuario.setGeometry (x, 5, 80, 30);
	editor_usuario.setGeometry (x, 30, 140, 30);
	
	etiqueta_clave.setGeometry (x+160, 5, 80, 30);
	editor_clave.setGeometry (x+160, 30, 140, 30);
	editor_clave.setMaxLength (8);
	editor_clave.setEchoMode (QLineEdit::Password);

	etiqueta_error.setGeometry (x, 60, 500, 30);
	
	aceptar.setGeometry (x+130, 90, 80, 30);
	cancelar.setGeometry (x+220, 90, 80, 30);
	QObject::connect (&aceptar, SIGNAL (clicked ()), this, SLOT (al_aceptar (2)));
	QObject::connect (&cancelar, SIGNAL (clicked ()), this, SLOT (al_cancelar ()));

	ventana.show ();
	return app.exec ();
}
#endif


