#include <iostream>
#include "juego.hpp"

static const int periodo=1000000/60; // TODO averiguar
static const int ancho=800;
static const int alto=800;

juego::juego ():
	termino (false),
	us (periodo),
	cuadros (0),
	fps (60),
	d1 (1),
	d2 (2),
	rect_origen_fondo1 {0, 0, 800, 600},
	rect_origen_fondo2 {0, 0, 800, 600},
	ventana (nullptr),
	renderer (nullptr),
	imagen_fondo1 (nullptr),
	imagen_fondo2 (nullptr),
	imagen_bob (nullptr),
	textura_fondo1 (nullptr),
	textura_fondo2 (nullptr),
	textura_bob (nullptr),
	textura_objetivo (nullptr)
{
	atexit (SDL_Quit);
	
	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		std::cerr << "No pudo inicializarse SDL: " << SDL_GetError () << '\n';
		return;
	}
	ventana = SDL_CreateWindow ("Titulo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	if (nullptr == ventana) {
		std::cerr << "No pudo crease la ventana: " << SDL_GetError () << '\n';
		return;
	}
	renderer = SDL_CreateRenderer (ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (nullptr == renderer) {
		std::cerr << "No pudo crease el renderer: " << SDL_GetError () << '\n';
		return;
	}
	imagen_fondo1 = IMG_Load ("imagenes/_fondo_1_.png"); 
	if (nullptr == imagen_fondo1) {
		std::cerr << "No pudo crease la superficie para el imagen: " << SDL_GetError () << '\n';
		return;
	}
	textura_fondo1 = SDL_CreateTextureFromSurface (renderer, imagen_fondo1);
	if (nullptr == textura_fondo1) {
		std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
		return;
	}
	imagen_fondo2 = IMG_Load ("imagenes/_fondo_2_.png"); 
	if (nullptr == imagen_fondo2) {
		std::cerr << "No pudo crease la superficie para el imagen: " << SDL_GetError () << '\n';
		return;
	}
	textura_fondo2 = SDL_CreateTextureFromSurface (renderer, imagen_fondo2);
	if (nullptr == textura_fondo2) {
		std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
		return;
	}
	textura_objetivo = SDL_CreateTexture (
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ancho,
		alto);
	imagen_bob = IMG_Load ("imagenes/bob.png"); 
	if (nullptr == imagen_bob) {
		std::cerr << "No pudo crease la superficie para el imagen: " << SDL_GetError () << '\n';
		return;
	}
	textura_bob = SDL_CreateTextureFromSurface (renderer, imagen_bob);
	if (nullptr == textura_bob) {
		std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
		return;
	}
	rect_origen_bob = {0, 0, imagen_bob->w, imagen_bob->h};
}

juego::~juego ()
{
}

bool juego::jugando ()
{
	return !termino;
}

void juego::manejar_eventos ()
{
	while (SDL_PollEvent (&e) != 0) {
		if (e.type == SDL_QUIT) {
			termino = true;
		}
	}
}

void juego::actualizar ()
{
	while (us >= periodo*0.9 ) {
		us -= periodo;
		// Actualizo la posicion del fondo1
		rect_origen_fondo1.x += d1;
		// Actualizo la posicion del fondo
		rect_origen_fondo2.x += d2;
		if (d2 > 0 ) {
			if (rect_origen_fondo2.x > imagen_fondo2->w-ancho) {
				d2 = -2;
				rect_origen_fondo2.x = imagen_fondo2->w-ancho;
				d1 = -1;
			}
		} else {
			if (rect_origen_fondo2.x < 0) {
				d2 = 2;
				rect_origen_fondo2.x = 0;
				d1 = 1;
			}
		}
		// Actualizo la posicion de bob
		rect_destino_bob = {400-imagen_bob->w/12, 295, imagen_bob->w/6, imagen_bob->h/6};
	}
}

void juego::dibujar ()
{
	std::cout << "fps: " << fps << "\n";
	SDL_SetTextureBlendMode (textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget (renderer, textura_objetivo);
	// Copio el fondo1
	SDL_RenderCopy (renderer, textura_fondo1, &rect_origen_fondo1, nullptr);
	// Copio el fondo2
	SDL_RenderCopy (renderer, textura_fondo2, &rect_origen_fondo2, nullptr);
	// Copio a bob
	SDL_RenderCopy (renderer, textura_bob, &rect_origen_bob, &rect_destino_bob);
	SDL_SetRenderTarget (renderer, nullptr);
	// Copio el resultado
	SDL_RenderCopy (renderer, textura_objetivo, nullptr, nullptr);
}

void juego::presentar ()
{
	SDL_RenderPresent (renderer);
	us += t_ciclo.microsegundos (true);
	cuadros++;
	if (t_fps.milisegundos () > 5000) {
		int ms = t_fps.milisegundos (true);
		fps = 1000 * cuadros / ms;
		cuadros = 0;
	}
}
