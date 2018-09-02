#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <fstream>
#include <iostream> 
#include <stdlib.h>
#include <ctime>


class LogEventos {
public:
	const char* tipoLog;
	void registrar(const char* tipoEvento, const char* evento_ocurrido);
	void borrarEventos();
	void definirTipoLog(const char* tipo);
	void mostrarEventos();
};

#endif