#ifndef TRADUCTORDELCLIENTE_H_
#define TRADUCTORDELCLIENTE_H_

#include <bitset>
#include "Constantes.h"
using namespace std;

class traductorDelCliente {
public:
	traductorDelCliente();

	void setMensajeATraducir(char* mensaje);

	bool quiereAccion(Constantes::BitAccion accion);

	virtual ~traductorDelCliente();

private:
	bitset<9> mensajeSinTraducir;
	bitset<9> desconectado;
};

#endif /* TRADUCTORDELCLIENTE_H_ */
