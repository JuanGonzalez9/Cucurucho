#include <iostream>
#include <iomanip>
#include <ventana.hpp>
#include <registro.hpp>

static const int periodo=1000000/60; // TODO averiguar

ventana::ventana (const std::string &titulo, int ancho, int alto):
	v (nullptr),
	r (nullptr),
	us (periodo),
	cuadros (0)
{
	atexit (SDL_Quit);

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		logerror("No pudo inicializarse SDL");
		std::cerr << "No pudo inicializarse SDL: " << SDL_GetError () << '\n';
		return;
	}
	
	Uint32 flags;
	flags = SDL_WINDOW_HIDDEN;

	v = SDL_CreateWindow (titulo.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ancho, alto, flags);

	if (nullptr == v) {
		logerror("No pudo crease la ventana");
		std::cerr << "No pudo crease la ventana: " << SDL_GetError () << '\n';
		return;
	}
	
	r = SDL_CreateRenderer (v, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (nullptr == r) {
		logerror("No pudo crease el renderer");
		std::cerr << "No pudo crease el renderer: " << SDL_GetError () << '\n';
		return;
	}
}

ventana::~ventana ()
{
	if (r) {
		SDL_DestroyRenderer (r);
	}
	if (v) {
		SDL_DestroyWindow (v);
	}
}

SDL_Renderer *ventana::renderer ()
{
	return r;
}

void ventana::titulo (const char *t)
{
	SDL_SetWindowTitle (v, t);
}

void ventana::mostrar ()
{
	SDL_ShowWindow (v);
}

void ventana::ocultar ()
{
	SDL_HideWindow (v);
}

void ventana::presentar ()
{
	loginfo("Se comienza a presentar ventana");
	SDL_RenderPresent (r);
	us += t_ciclo.microsegundos (true);
	cuadros++;
	if (t_fps.milisegundos () > 5000) {
		int ms = t_fps.milisegundos (true);
		float fps = 1000.0 * (float)cuadros / (float)ms;
		cuadros = 0;
		std::cout << "fps: " << std::fixed << std::setprecision (2) << fps << "\n";
	}
	loginfo("Se termina de presentar ventana");
}

