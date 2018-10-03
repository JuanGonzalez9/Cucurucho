#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include "plataformas.hpp"
#include  <list>
#include "cfg.hpp"

using namespace std;

ImagePlataforma::ImagePlataforma ():
	textura (nullptr),
	delta (0)
{
}

ImagePlataforma::~ImagePlataforma()
{
	if(textura){
		SDL_DestroyTexture (textura);
	}
	else{
		registro.registrar(LogEventos::error, "No se pudo destruir una textura");
	}
}

void ImagePlataforma::inicializar (const char *camino, SDL_Renderer* renderizador)
{
	textura = cfg.obtener_textura (camino, renderizador);
	SDL_QueryTexture (textura, nullptr, nullptr, &ancho, &alto);
}

void Plataformas::inicializar(SDL_Renderer* renderizador)
{
	for (int i = 0; i < plataforma::cantidad; i++) {
		imagenes[i].inicializar (plataforma::caminos[i], renderizador);
	}
	registro.registrar(LogEventos::info, "Se cargaron las imagenes de las plataformas");
}

Plataformas::Plataformas()
{
	plataforma p = {plataforma::piedras, 0, 100, 550};
	cfg.obtener_plataformas("//configuracion//escenarios//nivel1",lista_plataformas_nivel1, p);
	p.t = plataforma::piedra;
	p.y = 3600-50;
	cfg.obtener_plataformas("//configuracion//escenarios//nivel2",lista_plataformas_nivel2, p);
	p.t = plataforma::nieve;
	p.y = 550;
	p.xf = 100;
	cfg.obtener_plataformas("//configuracion//escenarios//nivel3",lista_plataformas_nivel3, p);

}

Plataformas::~Plataformas(){
}


void Plataformas::cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador, int nivel){
	//Hay que pedirle al archivo configurable los valores de las plataformas ¿En que formato? (Nos pasa directamente un objeto plataforma)
	
	SDL_Rect posicion_imagen, src_rect;
	int total;
	int numeroPlataformas;
	int anchoUltimaPlataforma;
	int resto;
	list<plataforma> lista_plataformas;

	if (nivel == 1){
		lista_plataformas = lista_plataformas_nivel1;
	}
	else if (nivel == 2){
		lista_plataformas = lista_plataformas_nivel2;
	}
	else if (nivel == 3){
		lista_plataformas = lista_plataformas_nivel3;
	}


	SDL_SetTextureBlendMode(textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderizador, textura_objetivo);

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;
		total = (plataformaActual.xf - plataformaActual.xi);

		posicion_imagen.y = plataformaActual.y;
		posicion_imagen.h = 600 - plataformaActual.y;
		
		if (nivel == 2){
			posicion_imagen.h = 100;
		}
		
		int anchoPlataforma = 100;
		
		if (plataformaActual.t == plataforma::nieve_arboles) std::cout << "ancho: " << anchoPlataforma << "\n";
		numeroPlataformas = total/anchoPlataforma;
		posicion_imagen.x = plataformaActual.xi;
		posicion_imagen.w = anchoPlataforma;

		for(int i =0; i < numeroPlataformas;i++){
			posicion_imagen.x = plataformaActual.xi + anchoPlataforma * (i);
			SDL_RenderCopy(renderizador,imagenes[plataformaActual.t].textura,NULL,&posicion_imagen);
		}

		resto = total%anchoPlataforma;
		if(resto!=0){
			anchoUltimaPlataforma = resto;
			posicion_imagen.x = plataformaActual.xi + anchoPlataforma * numeroPlataformas;
			posicion_imagen.w = anchoUltimaPlataforma;
			SDL_RenderCopy(renderizador,imagenes[plataformaActual.t].textura,NULL,&posicion_imagen);
		}
	}
	registro.registrar (LogEventos::info, "Se cargaron las plataformas y se renderizaron en el mapa");
	SDL_SetRenderTarget(renderizador,NULL);
}

SDL_Texture * Plataformas::crearTexturaParaElFondo(SDL_Texture* texturaFondo,SDL_Renderer* renderizador,int w,int h){
	SDL_Texture * textura_objetivo = SDL_CreateTexture (renderizador,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w, h);
	SDL_SetTextureBlendMode(textura_objetivo, SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(renderizador, textura_objetivo);
	SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 0);
	SDL_RenderClear(renderizador);
	SDL_RenderCopy(renderizador,texturaFondo,NULL,NULL);
	SDL_SetRenderTarget(renderizador, NULL);

	registro.registrar (LogEventos::info, "se creo la textura para el fondo de las plataformas");

	return textura_objetivo;

}


bool Plataformas::hayColision(int otroX, int otroY, int otroW, int otroH, int nivel){


	int parte_inferior = otroY+otroH;
	int parte_superior = otroY;
	int parte_lateral_izq = otroX;
	int parte_lateral_der = otroX+otroW;

	int parte_inferior_plataforma;
	int parte_superior_plataforma;
	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;
	list<plataforma> lista_plataformas;

	if (nivel == 1){
		lista_plataformas = lista_plataformas_nivel1;
	}
	else if (nivel == 2){
		lista_plataformas = lista_plataformas_nivel2;
	}
	else if (nivel == 3){
		lista_plataformas = lista_plataformas_nivel3;
	}

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

int Plataformas::hayPlataformaEn(int otroX,int nivel){

	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;

	list<plataforma> lista_plataformas;
	
	if (nivel == 1){
		lista_plataformas = lista_plataformas_nivel1;
	}
	else if (nivel == 2){
		lista_plataformas = lista_plataformas_nivel2;
	}
	else if (nivel == 3){
		lista_plataformas = lista_plataformas_nivel3;
	}

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


bool Plataformas::hayColisionSuperior(int otroX,int otroY,int otroW,int otroH, int nivel){
	int parte_inferior = otroY+otroH;
	int parte_superior = otroY;
	int parte_lateral_izq = otroX;
	int parte_lateral_der = otroX+otroW;

	int parte_inferior_plataforma;
	int parte_superior_plataforma;
	int parte_lateral_izq_plataforma;
	int parte_lateral_der_plataforma;

	list<plataforma> lista_plataformas;
	
	if (nivel == 1){
		lista_plataformas = lista_plataformas_nivel1;
	}
	else if (nivel == 2){
		lista_plataformas = lista_plataformas_nivel2;
	}
	else if (nivel == 3){
		lista_plataformas = lista_plataformas_nivel3;
	}


	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;

		parte_inferior_plataforma = 600 - plataformaActual.y; //Esto en algun momento se va a cambiar
		parte_superior_plataforma = plataformaActual.t == plataforma::nieve_arboles ? plataformaActual.y + 10 : plataformaActual.y;
		parte_lateral_izq_plataforma = plataformaActual.xi;
		parte_lateral_der_plataforma = plataformaActual.xf;

		//Colision superior (Personaje arriba de la plataforma)
		//std::cout << parte_inferior << " " << parte_superior_plataforma << "\n";
		
		if(parte_inferior == parte_superior_plataforma && (parte_lateral_izq_plataforma <= parte_lateral_izq && parte_lateral_der_plataforma >= parte_lateral_der)){
			
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
	}

	return false;
}


int Plataformas::aproximarPosicionAPlataforma(int otroX,int otroY,int otroW,int otroH, int velocidad, int nivel){

	for (int i=1; i < velocidad;i++){

		if (hayColisionSuperior(otroX,otroY-i,otroW,otroH,nivel)){

			return otroY-i;
		}
	}

	return -1;
}
