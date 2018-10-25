#ifndef DATOSPERSONAJE_H_
#define DATOSPERSONAJE_H_

#include "Constantes.h"

class DatosPersonaje {
public:
	DatosPersonaje();

	//...................set..............................
	void setPosX(int x);
	void setPosY(int y);
	void setSaltando(bool saltando);
	void setDisparando(bool disparando);
	void setMirandoALaDerecha(bool mirando);
	void setActivo(bool act);
	void setGrisado(bool gris);
	void setEstado(Constantes::Estado est);
	void setDireccionDisparo(Constantes::DireccionDisparo dir);

	//..................get................................
	int getPosX();
	int getPosY();
	bool estaSaltando();
	bool estaDisparando();
	bool estaMirandoALaDerecha();
	bool estaActivo();
	bool estaGrisado();
	Constantes::Estado getEstado();
	Constantes::DireccionDisparo getDireccionDisparo();

	virtual ~DatosPersonaje();
private:
	int posX;
	int posY;
	bool saltando;
	bool disparando;
	bool mirandoALaDerecha;
	bool activo;
	bool grisado;
	Constantes::Estado estado;
	Constantes::DireccionDisparo direccionDisparo;
};

#endif /* DATOSPERSONAJE_H_ */
