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
#define RESPUESTA_POSX 3
#define RESPUESTA_POSY 4	//el valor de Y puede ser negativo cuando se va para arriba, fuera de la ventana
#define RESPUESTA_ESTADOS 7

#define MENSAJE_CANT_BALAS 2  //dos bytes el mensaje '99'
#define MAX_BALAS 12
#define TAMANIO_POS_BALAS (RESPUESTA_POSX + RESPUESTA_POSY + 1) //posX ahora puede ser valor negativo,por eso se suma 1
#define RESPUESTA_PERSONAJE (RESPUESTA_POSX + RESPUESTA_POSY + RESPUESTA_ESTADOS)

#define TAMANIO_RESPUESTA_SERVIDOR RESPUESTA_NIVEL + RESPUESTA_FONDO * 3 + MENSAJE_CANT_BALAS + MAX_BALAS * TAMANIO_POS_BALAS


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
