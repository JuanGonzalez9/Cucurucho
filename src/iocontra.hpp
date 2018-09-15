#ifndef IOCONTRA_HPP
#define IOCONTRA_HPP

#include "registro.hpp"
#include "cfg.hpp"

extern LogEventos &registro;

static class inicializador_registro
{
public:
	inicializador_registro ();
	~inicializador_registro ();
} ini_registro;



extern configuracion &cfg;

static class inicializador_configuracion
{
public:
	inicializador_configuracion ();
	~inicializador_configuracion ();
} ini_cfg;

#endif
