#ifndef DATOSENEMIGO_H_
#define DATOSENEMIGO_H_

#include "Constantes.h"

class DatosEnemigo {
public:
	DatosEnemigo();

	//Dejo cosas comentadas por si cambiamos agregamos cosas a los enemigos

	//...................set..............................
	void setPosX(int x);
	void setPosY(int y);
	void setTipoEnemigo(Constantes::TipoEnemigo tipo);
	/*void setSaltando(bool saltando);
	void setDisparando(bool disparando);
	void setMirandoALaDerecha(bool mirando);
	void setActivo(bool act);
	void setGrisado(bool gris);
	void setEstado(Constantes::Estado est);
	void setDireccionDisparo(Constantes::DireccionDisparo dir);*/

	//..................get................................
	int getPosX();
	int getPosY();
	Constantes::TipoEnemigo getTipoEnemigo();
	/*bool estaSaltando();
	bool estaDisparando();
	bool estaMirandoALaDerecha();
	bool estaActivo();
	bool estaGrisado();
	Constantes::Estado getEstado();
	Constantes::DireccionDisparo getDireccionDisparo();*/

	virtual ~DatosEnemigo();
private:
	int posX;
	int posY;
	
	Constantes::TipoEnemigo tipo;
};

#endif /* DATOSENEMIGO_H_ */
