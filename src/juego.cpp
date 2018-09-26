#include <iostream>
#include <iomanip>
#include "juego.hpp"


static const int periodo=1000000/60; // TODO averiguar
static const int ancho=800;
static const int alto=800;

juego::juego ():
	termino (false),
	cambioNivel(false),
	us (periodo),
	cuadros (0),
	d1 (1),
	d2 (2),
	d3 (3),
	nivel (1),
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
	textura_bala = cfg.obtener_textura ("//configuracion//items//bala//sprite", renderer);

	//fondo1.crearTextura("imagenes/_fondo_1_.png",renderer);
	//fondo2.crearTextura("imagenes/_fondo_2_.png",renderer);
	fondo1.obtenerTextura("//configuracion//escenarios//nivel1//fondo1", renderer);
	fondo2.obtenerTextura("//configuracion//escenarios//nivel1//fondo2", renderer);

	textura_objetivo = SDL_CreateTexture (
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		ancho,
		alto);

	boby.obtenerTextura("//configuracion//personajes//heroe//sprite", renderer);
	boby.setRectOrigen(0,0,480,480);

	//darthBob->crearTextura("imagenes/enemigo.png",renderer);
	darthBob = new Enemigo(500,150,5);
	darthBob->obtenerTextura("//configuracion//personajes//enemigo//sprite", renderer);


	// Creamos textura para pegar las plataformas
	// Creamos textura para pegar las plataformas
	registro.registrar (LogEventos::info, "Se llama a plataformas para inicializar");
	plataformas.inicializar(renderer);


	registro.registrar (LogEventos::info, "Se llama para obtener la textura para el fondo de las plataformas");
	SDL_Texture* textura_fondo3_temp = cfg.obtener_textura ("//configuracion//escenarios//nivel1//fondo3", renderer);
	SDL_QueryTexture (textura_fondo3_temp, nullptr, nullptr, &mundo_w, &mundo_h);

	registro.registrar (LogEventos::info, "Se llama para crear la textura para el fondo de las plataformas");
	textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,mundo_w,mundo_h);
	SDL_DestroyTexture (textura_fondo3_temp);
	
	
	registro.registrar (LogEventos::info, "Se llama para cargar los valores de las plataformas en el nivel 1");
	plataformas.cargarValoresFijos(textura_fondo3,renderer,1);
	
	registro.registrar (LogEventos::info, "Se construyo juego");



}

bool juego::jugando ()
{
	return !termino;
}

void juego::manejar_eventos ()
{

	registro.registrar (LogEventos::info, "Se comienza a manejar eventos de juego");
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	int bobyPosX= nivel == 2 ? boby.getPosX() : boby.obtenerCoordenadaX();
	int bobyPosY= nivel == 2 ? boby.obtenerCoordenadaY() : boby.getPosY();
	int velocityBoby = boby.obtenerVelocidadY();

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,34,72,nivel)){
		boby.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,34,72,velocityBoby,nivel);

		if(bobyPosY != -1){
			boby.actualizarPos(bobyPosY, nivel);
			boby.aterrizar();
		}
	}
	else {
		boby.caer();
		
	}
	if(apretandoDerecha(state)){
		if(nivel!=2){	
			if(boby.getPosX() <= (ancho / 2)){
				boby.avanzar();
				boby.subirCoordenadaXEn(3);
			}
			else{
				boby.hacerComoQueCamina();
				boby.subirCoordenadaXEn(d3);
				fondo1.avanzarOrigen(d1);
				fondo2.avanzarOrigen(d2);
				rect_origen_fondo3.x += d3;
				if (rect_origen_fondo3.x > mundo_w-ancho) {
					rect_origen_fondo3.x = mundo_h-ancho;
				}
			}
		}
		else{
			if(boby.getPosX() < 760){
				boby.avanzar();
				boby.subirCoordenadaXEn(3);
			}
		}
	}


	if(apretandoIzquierda(state)){
		if(boby.getPosX() > 0 ){
			boby.retroceder();
			boby.subirCoordenadaXEn(-3);
		}
		
	}

	if(apretandoSalto(state) && apretandoAbajo(state) && !boby.estaSaltando()){
		boby.bajar();		
	}
	else{
		if(apretandoSalto(state)){
				boby.saltar();
			}
	}

	if(apretandoAgacharse(state) && !(apretandoDerecha(state) || apretandoIzquierda(state))){
		boby.agacharse();
	}

	if(apretandoNivel(state)){
		if(nivel==1){
			cambioNivel=true;
		}
	}
	if(apretandoFlotar(state)){
		boby.flotar();
		if((nivel==2)&&(apretandoNivel(state))){
			cambioNivel=true;
		}
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
			Bullet* nuevaBala;
			if(boby.getEstado() == Personaje::Estado::CuerpoATierra)
				nuevaBala = new Bullet(boby.getPosX(),boby.getPosY() + 34,posBala,direccionDeBala*10);
			else
				nuevaBala = new Bullet(boby.getPosX(),boby.getPosY(),posBala,direccionDeBala*10);
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

	//PASA DE NIVEL SI LLEGA AL FINAL
	if((nivel==1)&&(boby.obtenerCoordenadaX()>=5300)){
			cambioNivel=true;
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

	registro.registrar (LogEventos::info, "Se termina de manejar eventos de juego");
}

void juego::actualizar ()
{
	registro.registrar (LogEventos::info, "Se comienza actualizar juego");
	//scroll vertical
	if((boby.getPosY() <=(200))&&(nivel==2)&&(boby.obtenerVelocidadY()<0)){
		boby.subirCoordenadaYEn(boby.obtenerVelocidadY());
		/**/boby.decrementarPosY(boby.obtenerVelocidadY());		
		//por ahora todo con la velocidad Y de boby, no hay parallax
		fondo1.avanzarOrigenY(boby.obtenerVelocidadY());
		printf("origen x = %i\n", fondo1.getRectaOrigen().x);
		printf("origen y = %i\n", fondo1.getRectaOrigen().y);
		printf("origen w = %i\n", fondo1.getRectaOrigen().w);
		printf("origen h = %i\n", fondo1.getRectaOrigen().h);
		fondo2.avanzarOrigenY(boby.obtenerVelocidadY());
		rect_origen_fondo3.y += boby.obtenerVelocidadY();
		
		std::cout << "rect_origen_fondo3: " << rect_origen_fondo3.y << "\n";
	}
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


	if((nivel==1)&&cambioNivel)
	{
		nivel=2;
		fondo1.obtenerTextura("//configuracion//escenarios//nivel2//fondo1", renderer);
		fondo2.obtenerTextura("//configuracion//escenarios//nivel2//fondo2", renderer);

		SDL_Texture* textura_fondo3_temp = cfg.obtener_textura ("//configuracion//escenarios//nivel2//fondo3", renderer);
		SDL_QueryTexture (textura_fondo3_temp, nullptr, nullptr, &mundo_w, &mundo_h);
		textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,mundo_w,mundo_h);
		SDL_DestroyTexture (textura_fondo3_temp);
		plataformas.cargarValoresFijos(textura_fondo3,renderer,2);
		std::cout << "dib: " << mundo_w << "x" << mundo_h << "\n";

		fondo1.setRectOrigen(0,3600-600,800,600);
		fondo2.setRectOrigen(0,3600-600,800,600);
		rect_origen_fondo3={0,3600-600,800,600};
		boby.setPosX(50);
		boby.setPosY(400);
		boby.setCoordenadaX(0+50);
		boby.setCoordenadaY(3600-(600-400));
		cambioNivel=false;
		
	}

	if((nivel==2)&&cambioNivel)
	{
		nivel=3;		
		fondo1.obtenerTextura("//configuracion//escenarios//nivel3//fondo1", renderer);
		fondo2.obtenerTextura("//configuracion//escenarios//nivel3//fondo2", renderer);

		plataformas.cargarValoresFijos(textura_fondo3,renderer,3);

		fondo1.setRectOrigen(0,0,800,600);
		fondo2.setRectOrigen(0,0,800,600);
		rect_origen_fondo3={0,0,800,600};
		boby.setPosX(50);
		boby.setPosY(280);	
		boby.setCoordenadaX(0+50);
		boby.setCoordenadaY(280);
		cambioNivel=false;
		
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
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		if(! darthBob->derrotado() && collision(boby.getRectaDestino(),darthBob->getRectaDestino())){
			if(boby.getInvincibilityFrames() == 0)
				boby.perderVida();
		}
	}

	//veo si el jugador se cayo
	if(boby.getPosY()>600){
		boby.setPosY(40);		
		boby.perderVida();
		
	}

	//refresco el tiempo de invincibilidad
	boby.refreshIFrames();

	//actualiza el shootTimer del jugador (para que no tire 500 tiros por segundo)
	boby.refreshBullets();

	//veo si las balas le pegan al enemigo
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		for(unsigned i = 0;i < bullets.size();i++){
			if(! darthBob->derrotado() && collision(bullets[i]->getRectaDestino(),darthBob->getRectaDestino())){
				darthBob->perderVida();
				bullets.erase(bullets.begin() + i);
			}

		}
	}

	registro.registrar (LogEventos::info, "Se termina de actualizar juego");
}

void juego::dibujar ()
{
	registro.registrar (LogEventos::info, "Se comienza a dibujar juego");
	SDL_SetTextureBlendMode (textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget (renderer, textura_objetivo);
	// Copio el fondo1
	fondo1.dibujarFondo(renderer);
	// Copio el fondo2
	fondo2.dibujarFondo(renderer);
	
	//Copio el fondo3
	SDL_RenderCopy (renderer, textura_fondo3, &rect_origen_fondo3, nullptr);
	SDL_SetRenderTarget (renderer, nullptr);
	// Copio el resultado
	SDL_RenderCopy (renderer, textura_objetivo, nullptr, nullptr);

	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		if(!darthBob->derrotado())
			darthBob->dibujar(renderer);
		else{
			darthBob->~Enemigo();
		}
	}

	// Copio a bob
	if((boby.getInvincibilityFrames()/2) %2 == 0)
		boby.dibujar(renderer);


	for(unsigned i = 0; i < bullets.size();i++){
		bullets[i]->dibujar(renderer);
	}
	registro.registrar (LogEventos::info, "Se termina de dibujar juego");
}

void juego::presentar ()
{
	registro.registrar (LogEventos::info, "Se comienza a presentar juego");
	SDL_RenderPresent (renderer);
	us += t_ciclo.microsegundos (true);
	cuadros++;
	if (t_fps.milisegundos () > 5000) {
		int ms = t_fps.milisegundos (true);
		float fps = 1000.0 * (float)cuadros / (float)ms;
		cuadros = 0;
		std::cout << "fps: " << std::fixed << std::setprecision (2) << fps << "\n";
	}
	registro.registrar (LogEventos::info, "Se termina de presentar juego");
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

bool juego::apretandoAgacharse(const Uint8* state){
	return state[SDL_SCANCODE_C];
}

//Cambia de nivel
bool juego::apretandoNivel(const Uint8* state){
	return state[SDL_SCANCODE_Q];
}

//sube al jugador
bool juego::apretandoFlotar(const Uint8* state){
	return state[SDL_SCANCODE_W];
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

	registro.registrar (LogEventos::info, "Se destruyo juego");
}
