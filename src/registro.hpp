#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <fstream>
#include <iostream> 
#include <stdlib.h>
#include <ctime>
#include <mutex>



class LogEventos {
public:
	typedef enum {error, info, debug}TipoLog;
	
	void registrar(TipoLog tipoEvento, const char* evento_ocurrido);
	void borrarEventos();
	void definirTipoLog(TipoLog tipo);
	void mostrarEventos();
protected:
	TipoLog tipoLog;
	std::mutex mutex;
};

#ifndef IOCONTRA_HPP
	#include "iocontra.hpp"
#endif

#endif
