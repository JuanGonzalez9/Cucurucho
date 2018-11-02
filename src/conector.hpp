#ifndef CONECTOR_HPP
#define CONECTOR_HPP

#include <string>
#include <condition_variable>
#include "login.hpp"

int comprobar_credencial_en_servidor (ventana_login &login);
void escuchar (autenticados *a, const char *dir, int puerto, int jugadores);
void enviar_ok (int fd);
void esperar_ok (int fd);

#endif

