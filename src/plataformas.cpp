#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include "plataformas.hpp"
#include  <list>
#include "cfg.hpp"

using namespace std;


void Plataformas::inicializar(SDL_Renderer* renderizador){

	SDL_Surface* bmpImagenPlataforma = SDL_LoadBMP("imagenes/piedra1.bmp");
	texturaPlataforma = SDL_CreateTextureFromSurface(renderizador,bmpImagenPlataforma);
	SDL_FreeSurface(bmpImagenPlataforma);
}

Plataformas::Plataformas(): texturaPlataforma (nullptr)
{

	cfg.obtener_plataformas("//configuracion//escenarios//nivel1",lista_plataformas);
}

Plataformas::~Plataformas(){
	if(texturaPlataforma){
		SDL_DestroyTexture (texturaPlataforma);
	}
}


void Plataformas::cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador){
	//Hay que pedirle al archivo configurable los valores de las plataformas ¿En que formato? (Nos pasa directamente un objeto plataforma)
	
	SDL_Rect posicion_imagen;
	int total;
	int numeroPlataformas;
	int anchoUltimaPlataforma;
	int resto;
	
	SDL_SetTextureBlendMode(textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderizador, textura_objetivo);

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;
		total = (plataformaActual.xf - plataformaActual.xi);

		posicion_imagen.y = plataformaActual.y;
		posicion_imagen.h = 600 - plataformaActual.y;
		
		if((total) < 101 ){
			posicion_imagen.x = plataformaActual.xi;
			posicion_imagen.w = (plataformaActual.xf-plataformaActual.xi)+1;
			

			SDL_RenderCopy(renderizador,texturaPlataforma,NULL,&posicion_imagen);
			
		}
		else{

			

			numeroPlataformas = total/100;
			posicion_imagen.x = plataformaActual.xi;
			posicion_imagen.w = 100;

			for(int i =0; i < numeroPlataformas;i++){

				
				posicion_imagen.x = plataformaActual.xi + 100 * (i);
				
				SDL_RenderCopy(renderizador,texturaPlataforma,NULL,&posicion_imagen);

			}

			resto = total%100;
			if(resto!=0){
				anchoUltimaPlataforma = resto;
				posicion_imagen.x = plataformaActual.xi + 100 * numeroPlataformas;
				posicion_imagen.w = anchoUltimaPlataforma;
				SDL_RenderCopy(renderizador,texturaPlataforma,NULL,&posicion_imagen);
			}
		}

		//Por default siempre debe haber una plataforma en la donde aparece el bob

		/*plataforma plataformaDefault;
		plataformaDefault.xi = 0;
		plataformaDefault.xf = 100;
		plataformaDefault.y = 400;

		posicion_imagen.x = plataformaDefault.xi;
		posicion_imagen.w = plataformaDefault.xf;
		posicion_imagen.y = plataformaDefault.y;
		posicion_imagen.h = 200;

		it = lista_plataformas.begin();
		lista_plataformas.insert(it,plataformaDefault);

		SDL_RenderCopy(renderizador,texturaPlataforma,NULL,&posicion_imagen);*/

		
		
	}

	SDL_SetRenderTarget(renderizador,NULL);

	
	

	return;	
}

SDL_Texture * Plataformas::crearTexturaParaElFondo(SDL_Texture* texturaFondo,SDL_Renderer* renderizador,SDL_Surface* bmpImagenFondo){
	SDL_Texture * textura_objetivo = SDL_CreateTexture (renderizador,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,bmpImagenFondo->w,bmpImagenFondo->h);
	SDL_SetTextureBlendMode(textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderizador, textura_objetivo);
	SDL_RenderCopy(renderizador,texturaFondo,NULL,NULL);
	SDL_SetRenderTarget(renderizador, NULL);
	return textura_objetivo;

}


bool Plataformas::hayColision(int otroX, int otroY, int otroW, int otroH){


	int parte_inferior = otroY+otroH;
	int parte_superior = otroY;
	int parte_lateral_izq = otroX;
	int parte_lateral_der = otroX+otroW;

	int parte_inferior_plataforma;
	int parte_superior_plataforma;
	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;

		parte_inferior_plataforma = 600 - plataformaActual.y; //Esto en algun momento se va a cambiar
		parte_superior_plataforma = plataformaActual.y;
		parte_lateral_izq_plataforma = plataformaActual.xi;
		parte_lateral_der_plataforma = plataformaActual.xf;

		//Podemos observar que pueden ocurrir 8 tipos de colisiones, 4 simples y 4 complejas.
		//Colisiones simples: laterales, inferior y superior
		//Colisiones complejas: lateral y superior, lateral e inferior (2y2); (En punta)


		//Colision superior (Personaje arriba de la plataforma)
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_izq_plataforma < parte_lateral_izq && parte_lateral_der_plataforma > parte_lateral_der)){
			return true;
		}

		//Colision inferior (de la plataforma)
		if(parte_superior == parte_inferior_plataforma && (parte_lateral_izq_plataforma < parte_lateral_izq && parte_lateral_der_plataforma > parte_lateral_der)){
			return true;
		}

		//Colision lateral izq (de la plataforma)

		if(parte_lateral_der == parte_lateral_izq_plataforma && (parte_superior_plataforma < parte_superior && parte_inferior_plataforma > parte_inferior)){
			return true;
		}  

		//Colision lateral derecha( de la plataforma)
		if(parte_lateral_izq == parte_lateral_der_plataforma && (parte_superior_plataforma < parte_superior && parte_inferior_plataforma > parte_inferior)){
			return true;
		}
		//Esquina izquierda superior
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_der_plataforma > parte_lateral_der && parte_lateral_izq_plataforma < parte_lateral_der)){
			return true;
		}

		//Esquina derecha superior
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_izq_plataforma < parte_lateral_izq && parte_lateral_der_plataforma > parte_lateral_izq)){
			return true;
		}

		//Esquina izquierda inferior
		if(parte_superior == parte_inferior_plataforma && (parte_lateral_der_plataforma > parte_lateral_der && parte_lateral_izq_plataforma < parte_lateral_der)){
			return true;
		}

		//Esquina derecha inferior
		if(parte_superior == parte_inferior_plataforma && (parte_lateral_izq_plataforma < parte_lateral_izq && parte_lateral_der_plataforma > parte_lateral_izq)){
			return true;
		}
	}

	return false;
}

int Plataformas::hayPlataformaEn(int otroX){

	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;
		
		parte_lateral_izq_plataforma = plataformaActual.xi;
		parte_lateral_der_plataforma = plataformaActual.xf;

		if (otroX < plataformaActual.xf && otroX > plataformaActual.xi){
			return plataformaActual.y;
		}

	}

	return -1;
}


bool Plataformas::hayColisionSuperior(int otroX,int otroY,int otroW,int otroH){
	int parte_inferior = otroY+otroH;
	int parte_superior = otroY;
	int parte_lateral_izq = otroX;
	int parte_lateral_der = otroX+otroW;

	int parte_inferior_plataforma;
	int parte_superior_plataforma;
	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;

	printf("%i, %i,\n", otroX,otroY );
	printf("Parte inferior personaje = %i\n", parte_inferior );

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;

		parte_inferior_plataforma = 600 - plataformaActual.y; //Esto en algun momento se va a cambiar
		parte_superior_plataforma = plataformaActual.y;
		parte_lateral_izq_plataforma = plataformaActual.xi;
		parte_lateral_der_plataforma = plataformaActual.xf;

		//Colision superior (Personaje arriba de la plataforma)
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_izq_plataforma <= parte_lateral_izq && parte_lateral_der_plataforma >= parte_lateral_der)){
			printf("estoy en el medio de la plataforma\n");
			return true;
		}
		//Esquina izquierda superior
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_der_plataforma > parte_lateral_der && parte_lateral_izq_plataforma < parte_lateral_der)){
			printf("Estoy en la esq sup izq de la plataforma\n");
			return true;
		}

		//Esquina derecha superior
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_izq_plataforma < parte_lateral_izq && parte_lateral_der_plataforma > parte_lateral_izq)){
			printf("Estoy en la esquina sup der de la plataforma\n");
			return true;
		}
	}

	return false;
}


int Plataformas::aproximarPosicionAPlataforma(int otroX,int otroY,int otroW,int otroH, int velocidad){

	for (int i=1; i < velocidad;i++){

		if (hayColisionSuperior(otroX,otroY-i,otroW,otroH)){

			return otroY-i;
		}
	}

	return -1;
}
