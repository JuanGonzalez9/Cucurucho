#include "Utils.h"

Utils::Utils() {
}

string Utils::pasarAStringDeTamanio(int tamanio,int valor){

	string respuesta(tamanio,'0');

	string nuevo = to_string(valor);
	respuesta.replace(tamanio - nuevo.size(),nuevo.size(),nuevo);

	return respuesta;
}

