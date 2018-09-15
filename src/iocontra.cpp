#include <iocontra.hpp>

// Uso el idioma Nifty Counter para asegurarme que cfg y log se inicialicen
// antes de su primer uso y se destruyan luego del último uso, en el orden
// deseado.

static int nifty_counter_registro;
static typename std::aligned_storage<sizeof (LogEventos), alignof (LogEventos)>::type buffer_registro;
LogEventos &registro = reinterpret_cast<LogEventos&> (buffer_registro);

inicializador_registro::inicializador_registro ()
{
	if (nifty_counter_registro++ == 0) new (&registro) LogEventos ();
}

inicializador_registro::~inicializador_registro ()
{
	if (--nifty_counter_registro == 0) (&registro)->~LogEventos ();
}


static int nifty_counter_cfg;
static typename std::aligned_storage<sizeof (configuracion), alignof (configuracion)>::type buffer_cfg;
configuracion &cfg = reinterpret_cast<configuracion&> (buffer_cfg);

inicializador_configuracion::inicializador_configuracion ()
{
	if (nifty_counter_cfg++ == 0) new (&cfg) configuracion ();
}

inicializador_configuracion::~inicializador_configuracion ()
{
	if (--nifty_counter_cfg == 0) (&cfg)->~configuracion ();
}

