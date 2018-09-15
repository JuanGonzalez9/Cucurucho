#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include "plataformas.hpp"
#include  <list>
#include "cfg.hpp"

using namespace std;


void Plataformas::inicializar(SDL_Renderer* renderizador){

	SDL_Surface* bmpImagenPlataforma = SDL_LoadBMP("imagenes/plataformis.bmp");
	texturaPlataforma = SDL_CreateTextureFromSurface(renderizador,bmpImagenPlataforma);
	SDL_FreeSurface(bmpImagenPlataforma);
}

Plataformas::Plataformas(): texturaPlataforma (nullptr){

}

Plataformas::~Plataformas(){
	if(texturaPlataforma){
		SDL_DestroyTexture (texturaPlataforma);
	}
}


void Plataformas::cargarValoresFijos(SDL_Texture* textura_objetivo, SDL_Renderer* renderizador){
	//Hay que pedirle al archivo configurable los valores de las plataformas ¿En que formato? (Nos pasa directamente un objeto plataforma)
	
	SDL_Rect posicion_imagen;

	list<plataforma> lista_plataformas;
	cfg.obtener_plataformas("//configuracion//escenarios//nivel1",lista_plataformas);
	
	SDL_SetTextureBlendMode(textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderizador, textura_objetivo);

	for(list<plataforma>::iterator it=lista_plataformas.begin(); it!=lista_plataformas.end();++it){
		plataforma &plataformaActual = *it;
		posicion_imagen.x = plataformaActual.xi;
		posicion_imagen.y = plataformaActual.y;
		posicion_imagen.w = (plataformaActual.xf-plataformaActual.xi)+1;
		posicion_imagen.h = 600 - plataformaActual.y;


		SDL_RenderCopy(renderizador,texturaPlataforma,NULL,&posicion_imagen);
		
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



