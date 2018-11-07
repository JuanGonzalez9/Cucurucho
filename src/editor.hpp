#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <string>
#include "control.hpp"
#include "fuente.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
}

class editor: public control
{
public:
	editor (int x, int y, int largo);
	virtual ~editor ();
	virtual bool manejar_evento (SDL_Event &e);
	virtual void dibujar (int cx, int cy, int cw, int ch, SDL_Renderer *renderer);
	void texto (const char *texto, const char *etiqueta);
	std::string texto () const;
	void limitar (unsigned int largo);
	virtual void foco (bool enfocado);
	virtual bool enfocable ();
	virtual void al_presionar (int x, int y);
	virtual void al_mover (int x, int y);
	bool ocultar;
protected:
	std::string entrada, etiqueta;
	unsigned int cursor;
	bool cursor_visible;
	int cursor_conteo;
	unsigned int largo;
	int padding_x, padding_y, borde_x, borde_y, char_w, char_h;
	fuente f;
	SDL_Color color_texto, color_borde, color_borde_foco, color_fondo, color_cursor, color_etiqueta;
	void escribir (char c);
	void cursor_izquierda ();
	void cursor_derecha ();
	void cursor_principio ();
	void cursor_fin ();
	void borrar_izquierda ();
	void borrar_derecha ();
	void mostrar ();
	void actualizar_cursor ();
	void establecer (const char *texto, std::string &s);
};

#endif
