/*
 * Constantes.h
 *
 *  Created on: Oct 17, 2018
 *      Author: juan
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#define RESPUESTA_NIVEL 1
#define RESPUESTA_FONDO 4
#define RESPUESTA_POS 3
#define RESPUESTA_ESTADOS 7

#define TAMANIO_RESPUESTA_SERVIDOR RESPUESTA_NIVEL + RESPUESTA_FONDO * 3 + RESPUESTA_POS * 2 + RESPUESTA_ESTADOS

class Constantes{
public:
	enum BitAccion{
		derecha,
		izquierda,
		arriba,
		abajo,
		disparo,
		salto,
		agacharse,
		nivel2,
		nivel3
	};

	enum Estado{
		Quieto,
		Caminando,
		CuerpoATierra,
		HaciendoComoQueCamina
	};

	enum DireccionDisparo{
		Arriba,
		Centro,
		Abajo
	};

};

#endif /* CONSTANTES_H_ */
