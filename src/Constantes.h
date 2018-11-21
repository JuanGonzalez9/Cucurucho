/*
 * Constantes.h
 *
 *  Created on: Oct 17, 2018
 *      Author: juan
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

#define MUNDO_ANCHO	800
#define MUNDO_ALTO	600

#define RESPUESTA_NIVEL 1
#define RESPUESTA_FONDO 4
#define RESPUESTA_POSX 3
#define RESPUESTA_POSY 4	//el valor de Y puede ser negativo cuando se va para arriba, fuera de la ventana
#define RESPUESTA_ESTADOS 7

#define MENSAJE_CANT_BALAS 2  //dos bytes el mensaje '99'
#define MAX_BALAS 24
#define TAMANIO_POS_BALAS (RESPUESTA_POSX + RESPUESTA_POSY + 1) //posX ahora puede ser valor negativo,por eso se suma 1
#define RESPUESTA_PERSONAJE (RESPUESTA_POSX + RESPUESTA_POSY + RESPUESTA_ESTADOS)
#define RESPUESTA_ENEMIGO 1
#define TAMANIO_MENSAJE_TECLAS 9

#define MAX_ENEMIGOS_EN_ESCENA 5
#define MENSAJE_ENEMIGOS (1 + RESPUESTA_POSY * 2 * MAX_ENEMIGOS_EN_ESCENA)

#define MAX_ITEMS_EN_ESCENA 5
#define MENSAJE_ITEMS (1 + RESPUESTA_POSY * 2 * MAX_ITEMS_EN_ESCENA)

#define MAX_BALAS_ENEMIGAS 10
#define MENSAJE_BALAS_ENEMIGAS (1 + RESPUESTA_POSY * 2 * MAX_BALAS_ENEMIGAS)

#define TAMANIO_RESPUESTA_SERVIDOR RESPUESTA_NIVEL + RESPUESTA_FONDO * 3 + MENSAJE_CANT_BALAS + MAX_BALAS * TAMANIO_POS_BALAS + RESPUESTA_ENEMIGO + MENSAJE_ENEMIGOS + MENSAJE_ITEMS + MENSAJE_BALAS_ENEMIGAS

#define INTERVALO_RECONEXION (2*60)

#define MAX_TIEMPO_RESPUESTA		1500
#define MAX_TIEMPO_RESPUESTA_NUEVO	6000

#define MAX_PUERTOS_SERVIDOR		100

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

	enum TipoEnemigo{
		sinEnemigo,
		marcianito,
		ovni
	};

	enum TipoItem{
		sinItem,
		vidaExtra,
		ametralladora,
		escopeta,
		bazooka
	};

	enum TipoArma{
		sinBala,
		normal
	};

};

#endif /* CONSTANTES_H_ */
