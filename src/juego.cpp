#include <iostream>
#include <iomanip>
#include "juego.hpp"


static const int periodo=1000000/60; // TODO averiguar
static const int ancho=800;
static const int alto=800;

juego::juego ():
	termino (false),
	us (periodo),
	cuadros (0),
	d1 (1),
	d2 (2),
	d3 (3),
	rect_origen_fondo3 {0, 0, 800, 600},
	ventana (nullptr),
	renderer (nullptr),
	textura_objetivo (nullptr)
{
	atexit (SDL_Quit);

	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		std::cerr << "No pudo inicializarse SDL: " << SDL_GetError () << '\n';
		return;
	}
	ventana = SDL_CreateWindow ("Titulo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	if (nullptr == ventana) {
		std::cerr << "No pudo crease la ventana: " << SDL_GetError () << '\n';
		return;
	}
	renderer = SDL_CreateRenderer (ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (nullptr == renderer) {
		std::cerr << "No pudo crease el renderer: " << SDL_GetError () << '\n';
		return;
	}

	//cosas de balas
	direccionDeBala = 0;
	SDL_Surface* superficieTemporal = IMG_Load("imagenes/bullet.png");
	if(superficieTemporal == nullptr){
			std::cerr << "No pudo crease la superficie para el imagen: " << SDL_GetError () << '\n';
		}
	Uint32 colorkey = SDL_MapRGB(superficieTemporal->format, 255, 255, 255);
	SDL_SetColorKey(superficieTemporal, SDL_TRUE, colorkey);
	textura_bala = SDL_CreateTextureFromSurface(renderer,superficieTemporal);
	if (textura_bala == nullptr) {
			std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
			return;
		}
	SDL_FreeSurface(superficieTemporal);


	fondo1.crearTextura("imagenes/_fondo_1_.png",renderer);
	fondo2.crearTextura("imagenes/_fondo_2_.png",renderer);

	textura_objetivo = SDL_CreateTexture (
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ancho,
		alto);

	boby.crearTextura("imagenes/personaje.png",renderer);
	boby.setRectOrigen(0,0,480,480);

	darthBob = new Enemigo(400,150,5);
	darthBob->crearTextura("imagenes/enemigo.png",renderer);


	// Creamos textura para pegar las plataformas
	imagen_fondo3 = IMG_Load ("imagenes/fondo3.png");
	if (nullptr == imagen_fondo3) {
		std::cerr << "No pudo crease la superficie para el imagen: " << SDL_GetError () << '\n';
		return;
	}
	SDL_Texture* textura_fondo3_temp = SDL_CreateTextureFromSurface (renderer, imagen_fondo3);
	if (nullptr == textura_fondo3_temp) {
		std::cerr << "No pudo crease la textura: " << SDL_GetError () << '\n';
		return;
	}

	plataformas.inicializar(renderer);
	textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,imagen_fondo3);
	plataformas.cargarValoresFijos(textura_fondo3,renderer);


	SDL_DestroyTexture (textura_fondo3_temp);

}

bool juego::jugando ()
{
	return !termino;
}

void juego::manejar_eventos ()
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	int bobyPosX= boby.obtenerCoordenadaX();
	int bobyPosY= boby.getPosY();
	int velocityBoby = boby.obtenerVelocidadY();

	printf("velocidad boby = %i\n", velocityBoby );


	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,34,72)){
		boby.aterrizar();
	}
	else if( velocityBoby > 1){

		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,34,72,velocityBoby);

		if(bobyPosY != -1){
			boby.actualizarPos(bobyPosY);
			boby.aterrizar();
		}
	}

	else {
		boby.caer();
		
	}
	if(apretandoDerecha(state)){
		if(boby.getPosX() <= (ancho / 2)){
			boby.avanzar();
			boby.subirCoordenadaXEn(3);
		}
		else{
			boby.subirCoordenadaXEn(d3);
			fondo1.avanzarOrigen(d1);
			fondo2.avanzarOrigen(d2);
			rect_origen_fondo3.x += d3;
			if (rect_origen_fondo3.x > imagen_fondo3->w-ancho) {
				rect_origen_fondo3.x = imagen_fondo3->w-ancho;
			}
		}
	}


	if(apretandoIzquierda(state)){
		if(boby.getPosX() > 0 ){
			boby.retroceder();
			boby.subirCoordenadaXEn(-3);
		}
		
	}
	if(apretandoSalto(state)){
		boby.saltar();
	}

	if(apretandoBajar(state)){
		boby.bajar();		
	}

	if(apretandoDisparo(state)){
		boby.pelarElChumbo();
		if(boby.puedeDisparar()){
			direccionDeBala = 0;
			if(apretandoArriba(state))
				direccionDeBala--;
			if(apretandoAbajo(state))
				direccionDeBala++;

			int posBala = 10;
			if(!boby.estaMirandoALaDerecha())
				posBala = -10;
			Bullet* nuevaBala = new Bullet(boby.getPosX(),boby.getPosY(),posBala,direccionDeBala*10);
			nuevaBala->asignarTextura(textura_bala);
			bullets.push_back(nuevaBala);

			boby.disparar();
		}
	}

	if(! apretandoDisparo(state)){
		boby.dejarDeDisparar();
	}
	if(apretandoArriba(state)){
		boby.apuntarArriba();
	}
	if(apretandoAbajo(state)){
		boby.apuntarAbajo();
	}
	if(! apretandoArriba(state) && ! apretandoAbajo(state)){
		boby.dejarDeApuntar();
	}

	while (SDL_PollEvent (&e) != 0) {
		switch(e.type){

			case (SDL_QUIT):
				termino = true;
				break;

			default:
				break;
		}
	}
}

void juego::actualizar ()
{
	boby.actualizar();
	while (us >= periodo*0.9 ) {
		us -= periodo;
		/*// Actualizo la posicion del fondo1
		fondo1.avanzarOrigen(d1);
		// Actualizo la posicion del fondo2
		fondo2.avanzarOrigen(d2);
		//Actualizo la posicion del fondo3
		rect_origen_fondo3.x += d3;
		if (d3 > 0 ) {
			if (rect_origen_fondo3.x > ancho) {
				//imagen_fondo3->w-ancho
				d3 = -3;
				d2 = -2;
				rect_origen_fondo3.x = ancho;
				d1 = -1;
			}
		}else if (rect_origen_fondo3.x < 0) {
			d3=3;
			d2 = 2;
			rect_origen_fondo3.x = 0;
			d1 = 1;*/
	}

	//Actualizo balas
	for(int i = 0; i < bullets.size(); i++){
		bullets[i]->move();
	}

	//borro las bals que exceden su rango para que no sigan hasta el infinito
	for(unsigned i = 0; i < bullets.size(); i++){
		if(bullets[i]->getDuracion() == 0){
			bullets.erase(bullets.begin() + i);
		}
	}

	//veo si el jugador toca al enemigo
	if(! darthBob->derrotado() && collision(boby.getRectaDestino(),darthBob->getRectaDestino())){
		if(boby.getInvincibilityFrames() == 0)
			boby.perderVida();
	}

	//refresco el tiempo de invincibilidad
	boby.refreshIFrames();

	//actualiza el shootTimer del jugador (para que no tire 500 tiros por segundo)
	boby.refreshBullets();

	//veo si las balas le pegan al enemigo
	for(unsigned i = 0;i < bullets.size();i++){
		if(! darthBob->derrotado() && collision(bullets[i]->getRectaDestino(),darthBob->getRectaDestino())){
			darthBob->perderVida();
			bullets.erase(bullets.begin() + i);
		}

	}
}

void juego::dibujar ()
{
	SDL_SetTextureBlendMode (textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget (renderer, textura_objetivo);
	// Copio el fondo1
	fondo1.dibujarFondo(renderer);
	// Copio el fondo2
	fondo2.dibujarFondo(renderer);
	//Copio el fondo3
	SDL_RenderCopy (renderer, textura_fondo3, &rect_origen_fondo3, nullptr);
	// Copio a bob

	SDL_SetRenderTarget (renderer, nullptr);
	// Copio el resultado
	SDL_RenderCopy (renderer, textura_objetivo, nullptr, nullptr);

	if(!darthBob->derrotado())
		darthBob->dibujar(renderer);
	else{
		darthBob->~Enemigo();
	}

	if((boby.getInvincibilityFrames()/2) %2 == 0)
		boby.dibujar(renderer);


	for(unsigned i = 0; i < bullets.size();i++){
		bullets[i]->dibujar(renderer);
	}
}

void juego::presentar ()
{

	SDL_RenderPresent (renderer);
	us += t_ciclo.microsegundos (true);
	cuadros++;
	if (t_fps.milisegundos () > 5000) {
		int ms = t_fps.milisegundos (true);
		float fps = 1000.0 * (float)cuadros / (float)ms;
		cuadros = 0;
		std::cout << "fps: " << std::fixed << std::setprecision (2) << fps << "\n";
	}
}

bool juego::apretandoDerecha(const Uint8* state){
	//controla tambien que no este la tecla izquierda
	return (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]);
}

bool juego::apretandoIzquierda(const Uint8* state){
	//controla tambien que no este la tecla derecha
	return (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]);
}

bool juego::apretandoArriba(const Uint8* state){
	return state[SDL_SCANCODE_UP];
}

bool juego::apretandoDisparo(const Uint8* state){
	return state[SDL_SCANCODE_Z];
}

bool juego::apretandoSalto(const Uint8* state){
	return state[SDL_SCANCODE_X];
}

bool juego::apretandoAbajo(const Uint8* state){
	return state[SDL_SCANCODE_DOWN];
}

bool juego::apretandoBajar(const Uint8* state){
	return state[SDL_SCANCODE_C];
}


bool juego::collision(SDL_Rect rect1,SDL_Rect rect2){
	if(rect1.y >= rect2.y + rect2.h)
		return false;
	if(rect1.x >= rect2.x + rect2.w)
		return false;
	if(rect1.y + rect1.h <= rect2.y)
		return false;
	if(rect1.x + rect1.w <= rect2.x)
		return false;
	return true;
}

juego::~juego ()
{
	SDL_FreeSurface (imagen_fondo3);
	SDL_DestroyTexture (textura_fondo3);
	SDL_DestroyTexture (textura_objetivo);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (ventana);
	SDL_DestroyTexture(textura_bala);
	if(! darthBob->derrotado())
		darthBob->~Enemigo();

	for(unsigned i = 0;i < bullets.size();i++){
		bullets[i]->~Bullet();
	}
}
