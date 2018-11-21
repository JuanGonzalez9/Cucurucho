#ifndef DATOSITEM_H_
#define DATOSITEM_H_

#include "Constantes.h"

class DatosItem {
public:
	DatosItem();

	//...................set..............................
	void setPosX(int x);
	void setPosY(int y);
	void setTipoItem(Constantes::TipoItem tipo);

	//..................get................................
	int getPosX();
	int getPosY();
	Constantes::TipoItem getTipoItem();

	virtual ~DatosItem();
private:
	int posX;
	int posY;
	
	Constantes::TipoItem tipo;
};

#endif /* DATOSITEM_H_ */
