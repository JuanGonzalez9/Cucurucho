#ifndef CONECTOR_HPP
#define CONECTOR_HPP

#include <string>
#include <condition_variable>
#include "login.hpp"

void comprobar_credencial_en_servidor (credencial &cred);
void escuchar (autenticados *a, const char *dir, int puerto, int jugadores);
bool enviar_ok (int fd);
bool enviar_cancelar (int fd);
bool esperar_ok (int fd);
bool leer (int fd, std::string &s);
bool escribir (int fd, std::string &s);
bool separar (char *t, std::string &a, std::string &b, char sep);
void cerrar (int &fd);

#endif

