#ifndef CONECTOR_HPP
#define CONECTOR_HPP

#include <string>
#include <condition_variable>
#include "login.hpp"

int comprobar_credencial_en_servidor (int puerto, const std::string &usuario, const std::string &clave,
							usuario::estado & estado, int & jugadores);
void escuchar (autenticados *a, int puerto, int jugadores);
void enviar_ok (int fd);
void esperar_ok (int fd);

#endif

