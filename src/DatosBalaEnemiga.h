#ifndef DATOSBALAENEMIGA_H_
#define DATOSBALAENEMIGA_H_

#include "Constantes.h"

class DatosBalaEnemiga {
public:
	DatosBalaEnemiga();

	//...................set..............................
	void setPosX(int x);
	void setPosY(int y);
	void setTipoArma(Constantes::TipoArma tipo);

	//..................get................................
	int getPosX();
	int getPosY();
	Constantes::TipoArma getTipoArma();

	virtual ~DatosBalaEnemiga();
private:
	int posX;
	int posY;
	
	Constantes::TipoArma tipo;
};

#endif /* DATOSBALAENEMIGA_H_ */
