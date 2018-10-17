#ifndef TRADUCTORDELCLIENTE_H_
#define TRADUCTORDELCLIENTE_H_

#include <bitset>
using namespace std;

class traductorDelCliente {
public:
	traductorDelCliente();

	void setMensajeATraducir(char* mensaje);

	bool quiereMoverDerecha();
	bool quiereMoverIzquierda();
	bool quiereSaltar();

	virtual ~traductorDelCliente();

private:
	bitset<9> mensajeSinTraducir;
};

#endif /* TRADUCTORDELCLIENTE_H_ */
