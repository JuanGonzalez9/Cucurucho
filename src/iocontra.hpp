#ifndef IOCONTRA_HPP
#define IOCONTRA_HPP

#include <sstream>
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


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define loginfo(msg) \
	do { \
		std::stringstream ss; \
		ss << __FILENAME__ << ":" << __LINE__ << ": " << msg; \
		registro.registrar (LogEventos::info, ss.str().c_str()); \
	} while (false)

#define logerror(msg) \
	do { \
		std::stringstream ss; \
		ss << __FILENAME__ << ":" << __LINE__ << ": " << msg; \
		registro.registrar (LogEventos::error, ss.str().c_str()); \
	} while (false)

#define logdebug(msg) \
	do { \
		std::stringstream ss; \
		ss << __FILENAME__ << ":" << __LINE__ << ": " << msg; \
		registro.registrar (LogEventos::debug, ss.str().c_str()); \
	} while (false)

#endif
