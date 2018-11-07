#include <iostream>
#include "dialogo.hpp"
#include "cfg.hpp"
#include "Constantes.h"

static const int min_color_fondo = 30;
static const int max_color_fondo = 60;
static const int pasos_fondo = 8;
static const int logo_c_x_final = 209;
static const int logo_ontra_x_final = 285;
static const int logo_c_delta = 9;
static const int logo_ontra_delta = -24;
static const int nube_min_delta = -2; // -1
static const int nube_max_delta = -6; // -5

dialogo::dialogo (ventana &v):
	contenedor_principal (v),
	c (0),
	d (1),
	p (0)
{
	logo_c_textura = cfg.obtener_textura ("//configuracion//dialogo//logo_c", v.renderer ());
	SDL_QueryTexture (logo_c_textura, nullptr, nullptr, &logo_c_ancho, &logo_c_alto);
	logo_ontra_textura = cfg.obtener_textura ("//configuracion//dialogo//logo_ontra", v.renderer ());
	SDL_QueryTexture (logo_ontra_textura, nullptr, nullptr, &logo_ontra_ancho, &logo_ontra_alto);
	std::stringstream ss;
	for (int i = 0; i < nube_cantidad; ++i) {
		ss << "//configuracion//dialogo//nube" << i+1;
		nube_textura[i] = cfg.obtener_textura (ss.str ().c_str (), v.renderer ());
		SDL_QueryTexture (nube_textura[i], nullptr, nullptr, &nube_ancho[i], &nube_alto[i]);
		nube_x[i] = cfg.random (-0.9 * MUNDO_ANCHO, 0.9 * MUNDO_ANCHO);
		nube_y[i] = cfg.random (60, MUNDO_ALTO - (0.2 * nube_alto[i]));
		nube_delta[i] = cfg.random (nube_max_delta, nube_min_delta);
		ss.str ("");
	}
	logo_c_x = logo_c_x_final - 600;
	logo_ontra_x = logo_ontra_x_final + 1600;
}

dialogo::~dialogo ()
{
	for (int i = 0; i < nube_cantidad; ++i) {
		SDL_DestroyTexture (nube_textura[i]);
	}
	SDL_DestroyTexture (logo_c_textura);
	SDL_DestroyTexture (logo_ontra_textura);
}

void dialogo::actualizar()
{
	// ATENCION: acá solo se actualiza la logica de la animación.
	if (p >= pasos_fondo) {
		c += d;
		if (c > max_color_fondo) {
			d *= -1;
			c = max_color_fondo - 1;
		} else if (c < min_color_fondo) {
			d *= -1;
			c = min_color_fondo + 1;
		}
		p = 0;
	}
	p++;

	for (int i = 0; i < nube_cantidad; ++i) {
		nube_x[i] += nube_delta[i];
		if (nube_x[i] + nube_ancho[i] <= 0) {
			nube_x[i] = cfg.random (MUNDO_ANCHO, 1.5 * MUNDO_ANCHO);
			nube_y[i] = cfg.random (60, MUNDO_ALTO - (0.2 * nube_alto[i]));
			nube_delta[i] = cfg.random (nube_max_delta, nube_min_delta);
		}
	}

	if (logo_c_x < logo_c_x_final) {
		logo_c_x += logo_c_delta;
	}
	if (logo_ontra_x > logo_ontra_x_final) {
		logo_ontra_x += logo_ontra_delta;
	}
}

void dialogo::dibujar_nubes (bool delante)
{
	int desde = !delante ? 0 : nube_cantidad/3;
	int hasta = !delante ? nube_cantidad/3 : nube_cantidad;
	for (int i = desde; i < hasta; ++i) {
		SDL_Rect ro = {0, 0, nube_ancho[i], nube_alto[i]};
		SDL_Rect rd = {nube_x[i], nube_y[i], nube_ancho[i], nube_alto[i]};
		SDL_RenderCopy (v.renderer (), nube_textura[i], &ro, &rd);
	}
}

void dialogo::dibujar ()
{
	// ATENCION: acá solo se dibuja la animación.
	SDL_SetRenderDrawColor (v.renderer (), c/3, c/3, c, 255);
	SDL_RenderClear (v.renderer ());

	activar_blend (v.renderer ());

	dibujar_nubes (false);
	
	SDL_Rect ro = {0, 0, logo_c_ancho, logo_c_alto};
	SDL_Rect rd = {logo_c_x, 40, logo_c_ancho, logo_c_alto};
	SDL_RenderCopy (v.renderer (), logo_c_textura, &ro, &rd);

	ro = {0, 0, logo_ontra_ancho, logo_ontra_alto};
	rd = {logo_ontra_x, 40, logo_ontra_ancho, logo_ontra_alto};
	SDL_RenderCopy (v.renderer (), logo_ontra_textura, &ro, &rd);

	dibujar_nubes (true);

	restaurar_blend (v.renderer ());

	contenedor_principal::dibujar ();
}

