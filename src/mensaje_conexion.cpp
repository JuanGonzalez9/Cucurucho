#include <iostream>
#include <sstream>
#include "mensaje_conexion.hpp"
#include "conector.hpp"

static const int infinito = -1;

mensaje_conexion::mensaje_conexion (ventana &v, struct credencial &cred):
	mensaje (v),
	cred (cred),
	ciclo (0)
{
	aceptar.texto ("Salir");
	// Como el dialogo continua intentado reconectarse indefinidamente, si el resultado del login
	// es fallido, se que el usuario cancelo la operación.
	cred.resultado = usuario::fallido;
}

mensaje_conexion::~mensaje_conexion()
{
	if (hilo.joinable ()) {
		std::cout << "hilo.join 2()\n";
		hilo.join ();
	}
	std::cout << "Sale de mensaje_conexion::~mensaje_conexion()\n";
}

void mensaje_conexion::actualizar ()
{
	mensaje::actualizar ();
	if (ciclo != infinito) {
		ciclo++;
	}
	if (ciclo > INTERVALO_RECONEXION && !hilo.joinable ()) {
		std::cout << "comprobar_credencial...\n";
		ciclo = infinito;
		hilo = std::thread{ comprobar_credencial, this };
	}
}

void mensaje_conexion::comprobar_credencial (mensaje_conexion *m)
{
    // Hilo secundario
    //bool primer_intento = true;
    //comienzo:
    std::cout << "comprobar_credencial_en_servidor...\n";
    comprobar_credencial_en_servidor (m->cred);
    if (m->cred.resultado == usuario::reaceptado) {
        /*if (primer_intento) {
            std::cout << "enviando cancelar\n";
            if (!enviar_cancelar (m->cred.fd)) {
                std::cout << "cancelar fallo\n";
            }
            std::cout << "cancelar enviado\n";
            primer_intento = false;
            std::cout << "Esperando...\n";
            sleep (5);
            goto comienzo;
        } else {*/
            std::cout << "enviando ok\n";
            if (!enviar_ok (m->cred.fd)) {
                std::cout << "esperar_ok fallo\n";
                m->cred.resultado = usuario::fallido;
            }else{
                std::cout << "esperando ok para " << m->cred.usuario << " en fd " << m->cred.fd << "\n";
                if (!esperar_ok (m->cred.fd)) {
                    std::cout << "esperar_ok fallo\n";
                    m->cred.resultado = usuario::fallido;
                }
                else{
                    std::cout << "ok recibido\n";
                }
               
            }
           
        //}
    } else if (m->cred.resultado == usuario::aceptado) {
        std::cout << "enviando cancelar\n";
        enviar_cancelar (m->cred.fd);
        std::cout << "cancelar enviado\n";
    }
    m->sincronizada ([m](){m->al_terminar_hilo ();});
}

void mensaje_conexion::al_terminar_hilo ()
{
	std::cout << "al_terminar_hilo\n";
	hilo.detach ();
	switch (cred.resultado) {
		case usuario::reaceptado:
			std::cout << "Credencial reaceptada\n";
			corriendo = false;
			break;
		case usuario::aceptado:
			std::cout << "Credencial aceptada\n";
			corriendo = false;
			break;
		case usuario::rechazado:
			std::cout << "Credencial inválida\n";
			corriendo = false;
			//error ("Credencial invalida.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			#endif
			break;
		case usuario::cupo:
			std::cout << "La partida ya está llena\n";
			corriendo = false;
			//info ("La partida ya esta llena.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			#endif
			break;
		case usuario::jugando:
			std::cout << "El usuario ya está jugando\n";
			corriendo = false;
			//info ("El usuario ya esta jugando.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, false);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, false);
			gtk_widget_set_sensitive (login->editor_clave, false);
			#endif
			break;
		case usuario::fallido:
			std::cout << "Falló la conexión, inténtelo más tarde\n";
			//error ("Fallo la conexion, intentelo mas tarde.", tiempo_mensaje);
			#if 0
			gtk_widget_set_sensitive (login->aceptar, true);
			gtk_widget_set_sensitive (login->cancelar, true);
			gtk_widget_set_sensitive (login->editor_usuario, true);
			gtk_widget_set_sensitive (login->editor_clave, true);
			#endif
			break;
	};
	ciclo = 0;
}

