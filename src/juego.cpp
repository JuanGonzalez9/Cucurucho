#include <iostream>
#include <iomanip>
#include "juego.hpp"
#include "alien.hpp"
#include "DatosPersonaje.h"


static const int periodo=1000000/60; // TODO averiguar
static const int ancho=800;
static const int alto=800;

juego::juego (ventana &v, int cantidadJugadores, puntajes &pts):
	contenedor_principal (v),
	pts (pts),
	termino (false),
	cambioNivel(false),
	us (periodo),
	cuadros (0),	
	d1 (2),
	d2 (3),
	d3 (5),
	nivel (1),
	cascada(0),
	coordenada(800),
	num_jugadores(cantidadJugadores),
	rect_origen_fondo3 {0, 0, 800, 600},
	renderer (nullptr),
	textura_objetivo (nullptr),
	cliente(nullptr),
	cliente2(nullptr),
	cliente3(nullptr),
	cliente4(nullptr),
	armador(nullptr)
{
	renderer = v.renderer ();

	//cosas de balas
	direccionDeBala = 0;
	textura_bala = cfg.obtener_textura ("//configuracion//items//bala//sprite", renderer);
	textura_bala2 = cfg.obtener_textura ("//configuracion//items//bala2//sprite", renderer);
	textura_bala3 = cfg.obtener_textura ("//configuracion//items//bala3//sprite", renderer);
	textura_bala4 = cfg.obtener_textura ("//configuracion//items//bala4//sprite", renderer);

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
	boby2.obtenerTextura("//configuracion//personajes//heroe2//sprite", renderer);
	boby2.setRectOrigen(0,0,480,480);
	boby3.obtenerTextura("//configuracion//personajes//heroe3//sprite", renderer);
	boby3.setRectOrigen(0,0,480,480);
	boby4.obtenerTextura("//configuracion//personajes//heroe4//sprite", renderer);
	boby4.setRectOrigen(0,0,480,480);

	boby.agregarGris("//configuracion//personajes//heroe//sprite", renderer);
	boby2.agregarGris("//configuracion//personajes//heroe2//sprite", renderer);
	boby3.agregarGris("//configuracion//personajes//heroe3//sprite", renderer);
	boby4.agregarGris("//configuracion//personajes//heroe4//sprite", renderer);

	Enemigo* nuevoEnemigo;
	nuevoEnemigo = new alien(this, 100, 245, 1, 0, 1);
	vectorEnemigos.push_back(nuevoEnemigo);

	//todo lo que dice NOTA MARTIN tiene que ver con los vectores de enemigos, items y balas

	//cargo vector de enemigos
	//NOTA MARTIN /////////////////////////////////////////////////////
	//para hacer un enemigo nuevo:
	//nuevoenemigo = new [subclase](x,y,vidas,coordenada, nivel)
	//x e y son las posiciones de pantalla donde quiero que aparezca
	//coordenada dice en que coordenada del nivel aparecen, nivel en que nivel aparecen
	//luego asigno textura correspondiente
	/*
	Enemigo* nuevoEnemigo;
	nuevoEnemigo = new Ovni(800,80,1,1920,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Marcianito(800,240,1,1400,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Marcianito(640,240,1,640,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	//FALTA PARSER PARA QUE NO ROMPA
	nuevoEnemigo = new Turret(800,400,1,2000,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//turret//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	//FALTA PARSER PARA QUE NO ROMPA
	nuevoEnemigo = new Paracaidas(600,-86,1,2000,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//parachute//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Ovni(800,50,1,2200,2);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Marcianito(750,-80,1,2700,2);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
	vectorEnemigos.push_back(nuevoEnemigo);
	*/

	this->cargarEnemigosNivel1(8);
	this->cargarEnemigosNivel2(8);
	this->cargarEnemigosNivel3(8);
	///////////////////////////////////////////////////////////

	//CARGO LOS ICONOS DE VIDAS///////////////////////////
	vida11.setRectOrigen(0,0,40,40);
	vida11.setRectDestino(5,5,40,40);
	vida11.obtenerTextura("//configuracion//items//vida1//sprite", renderer);
	vida12.setRectOrigen(0,0,40,40);
	vida12.setRectDestino(45,5,40,40);
	vida12.obtenerTextura("//configuracion//items//vida1//sprite", renderer);
	vida13.setRectOrigen(0,0,40,40);
	vida13.setRectDestino(85,5,40,40);
	vida13.obtenerTextura("//configuracion//items//vida1//sprite", renderer);

	vida21.setRectOrigen(0,0,40,40);
	vida21.setRectDestino(755,5,40,40);
	vida21.obtenerTextura("//configuracion//items//vida2//sprite", renderer);
	vida22.setRectOrigen(0,0,40,40);
	vida22.setRectDestino(715,5,40,40);
	vida22.obtenerTextura("//configuracion//items//vida2//sprite", renderer);
	vida23.setRectOrigen(0,0,40,40);
	vida23.setRectDestino(675,5,40,40);
	vida23.obtenerTextura("//configuracion//items//vida2//sprite", renderer);

	vida31.setRectOrigen(0,0,40,40);
	vida31.setRectDestino(5,555,40,40);
	vida31.obtenerTextura("//configuracion//items//vida3//sprite", renderer);
	vida32.setRectOrigen(0,0,40,40);
	vida32.setRectDestino(45,555,40,40);
	vida32.obtenerTextura("//configuracion//items//vida3//sprite", renderer);
	vida33.setRectOrigen(0,0,40,40);
	vida33.setRectDestino(85,555,40,40);
	vida33.obtenerTextura("//configuracion//items//vida3//sprite", renderer);

	vida41.setRectOrigen(0,0,40,40);
	vida41.setRectDestino(755,555,40,40);
	vida41.obtenerTextura("//configuracion//items//vida4//sprite", renderer);
	vida42.setRectOrigen(0,0,40,40);
	vida42.setRectDestino(715,555,40,40);
	vida42.obtenerTextura("//configuracion//items//vida4//sprite", renderer);
	vida43.setRectOrigen(0,0,40,40);
	vida43.setRectDestino(675,555,40,40);
	vida43.obtenerTextura("//configuracion//items//vida4//sprite", renderer);

	gameover1.setRectOrigen(0,0,120,40);
	gameover1.setRectDestino(5,5,120,40);
	gameover1.obtenerTextura("//configuracion//items//gameoverhud//sprite", renderer);

	gameover2.setRectOrigen(0,0,120,40);
	gameover2.setRectDestino(675,5,120,40);
	gameover2.obtenerTextura("//configuracion//items//gameoverhud//sprite", renderer);

	gameover3.setRectOrigen(0,0,120,40);
	gameover3.setRectDestino(5,555,120,40);
	gameover3.obtenerTextura("//configuracion//items//gameoverhud//sprite", renderer);

	gameover4.setRectOrigen(0,0,120,40);
	gameover4.setRectDestino(675,555,120,40);
	gameover4.obtenerTextura("//configuracion//items//gameoverhud//sprite", renderer);
	/////////////////////////////////////////////////




	//jefes default

	enemigoNivel1 = new MonstruoFinalNivel1(this,8000-252,150,5);
	enemigoNivel1->obtenerTextura("//configuracion//personajes//enemigo1//sprite", renderer);
	//((MonstruoFinalNivel1*) enemigoNivel1)->posicionar();
	vectorEnemigos.push_back(enemigoNivel1);

	enemigoNivel2 = new MonstruoFinalNivel2(this,230,80,7);
	enemigoNivel2->obtenerTextura("//configuracion//personajes//enemigo2//sprite", renderer);
	vectorEnemigos.push_back(enemigoNivel2);


	enemigoNivel3 = new MonstruoFinalNivel3(this,8000-280,150,10);
	enemigoNivel3->obtenerTextura("//configuracion//personajes//enemigo3//sprite", renderer);
	vectorEnemigos.push_back(enemigoNivel3);


	/*enemigoNivel2 = new Enemigo(260,80,5);
	enemigoNivel2->obtenerTextura("//configuracion//personajes//enemigo2//sprite", renderer);

	enemigoNivel3 = new Enemigo(550,150,5);

	enemigoNivel3->obtenerTextura("//configuracion//personajes//enemigo3//sprite", renderer);*/

	loginfo("Se crearon los enemigos");

	// Creamos textura para pegar las plataformas

	loginfo("Se llama a plataformas para inicializar");
	plataformas.inicializar(renderer);

	loginfo("Se llama para obtener la textura para el fondo de las plataformas");
	SDL_Texture* textura_fondo3_temp = cfg.obtener_textura ("//configuracion//escenarios//nivel1//fondo3", renderer);
	SDL_QueryTexture (textura_fondo3_temp, nullptr, nullptr, &mundo_w, &mundo_h);

	loginfo("Se llama para crear la textura para el fondo de las plataformas");
	textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,mundo_w,mundo_h);
	SDL_DestroyTexture (textura_fondo3_temp);
	
	
	loginfo("Se llama para cargar los valores de las plataformas en el nivel 1");
	plataformas.cargarValoresFijos(textura_fondo3,renderer,1);
	
	loginfo("Se construyo juego");

	
	//Traductor
	cliente = new traductorDelCliente();
	cliente2 = new traductorDelCliente();
	cliente3 = new traductorDelCliente();
	cliente4 = new traductorDelCliente();
	
	



	armador = new ArmadorDeRespuesta();

	//evita que se estire el fondo si al principio voy para atras
	fondo1.avanzarOrigen(50);
	fondo2.avanzarOrigen(50);

	//activo los jugadores al principio
	//inicializo la posicion de los bobys en el primer nivel
	boby.activar();
	boby.setPosX(50);
	boby.setPosY(200);
	boby.setCoordenadaX(0+50);
	boby.setCoordenadaY(200);

	

		

	if(num_jugadores>=2){
		
		boby2.activar();
		boby2.setPosX(150);
		boby2.setPosY(200);
		boby2.setCoordenadaX(0+150);
		boby2.setCoordenadaY(200);
	}

	if(num_jugadores>=3){
		
		boby3.activar();
		boby3.setPosX(250);
		boby3.setPosY(200);
		boby3.setCoordenadaX(0+250);
		boby3.setCoordenadaY(200);
	}	

	if(num_jugadores>=4){
		
		boby4.activar();
		boby4.setPosX(350);
		boby4.setPosY(200);
		boby4.setCoordenadaX(0+350);
		boby4.setCoordenadaY(200);
	}
	
}

int juego::obtenerCantidadDeEnemigos(int nivel)
{
	int r = 0;
	if (nivel >= 1 && nivel <=3) {
		std::string s("//configuracion//cantidad_enemigos//nivel_");
		s += std::to_string(nivel);
		std::cout << cfg.obtener_i (s.c_str(), [](int n, bool omision) {return n > 0 && n < 1000;}) << "\n";
	}
	return r;
}

int juego::obtenerPuntaje(int jugador, bool resetear)
{
	Personaje *p;
	switch (jugador) {
		case 0:
			p = &boby;
			break;
		case 1:
			p = &boby2;
			break;
		case 2:
			p = &boby3;
			break;
		case 3:
			p = &boby4;
			break;
		default:
			return -1;
	};
	int puntos = p->obtenerPuntaje();
	if (resetear) {
		p->resetearPuntaje();
	}
	return puntos;
}

void juego::serializar_puntaje (std::stringstream &ss)
{
	pts.serializar (ss);
}

bool juego::jugando ()
{
	return !termino;
}

void juego::manejar_eventos ()
{
	loginfo("Se comienza a manejar eventos de juego");
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	///////////////////GRAVEDAD/////////////////////////

	int bobyPosX= nivel == 2 ? boby.getPosX() : boby.obtenerCoordenadaX();
	int bobyPosY= nivel == 2 ? boby.obtenerCoordenadaY() : boby.getPosY();
	int velocityBoby = boby.obtenerVelocidadY();

	if(!boby.estaMirandoALaDerecha()){
		bobyPosX += 12;
	}

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,25,72,nivel)){
		boby.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,25,72,velocityBoby,nivel);

		if(bobyPosY != -1){
			boby.actualizarPos(bobyPosY, nivel);
			boby.aterrizar();
		}
	}
	else {
		boby.caer();
	}

	bobyPosX= nivel == 2 ? boby2.getPosX() : boby2.obtenerCoordenadaX();
	bobyPosY= nivel == 2 ? boby2.obtenerCoordenadaY() : boby2.getPosY();
	velocityBoby = boby2.obtenerVelocidadY();

	if(!boby2.estaMirandoALaDerecha()){
		bobyPosX += 12;
	}

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,25,72,nivel)){
		boby2.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,25,72,velocityBoby,nivel);

		if(bobyPosY != -1){
			boby2.actualizarPos(bobyPosY, nivel);
			boby2.aterrizar();
		}
	}
	else {
		boby2.caer();
	}

	bobyPosX= nivel == 2 ? boby3.getPosX() : boby3.obtenerCoordenadaX();
	bobyPosY= nivel == 2 ? boby3.obtenerCoordenadaY() : boby3.getPosY();
	velocityBoby = boby3.obtenerVelocidadY();

	if(!boby3.estaMirandoALaDerecha()){
		bobyPosX += 12;
	}

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,25,72,nivel)){
		boby3.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,25,72,velocityBoby,nivel);

		if(bobyPosY != -1){
			boby3.actualizarPos(bobyPosY, nivel);
			boby3.aterrizar();
		}
	}
	else {
		boby3.caer();
	}
	bobyPosX= nivel == 2 ? boby4.getPosX() : boby4.obtenerCoordenadaX();
	bobyPosY= nivel == 2 ? boby4.obtenerCoordenadaY() : boby4.getPosY();
	velocityBoby = boby4.obtenerVelocidadY();

	if(!boby4.estaMirandoALaDerecha()){
		bobyPosX += 12;
	}

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,25,72,nivel)){
		boby4.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,25,72,velocityBoby,nivel);

		if(bobyPosY != -1){
			boby4.actualizarPos(bobyPosY, nivel);
			boby4.aterrizar();
		}
	}
	else {
		boby4.caer();
	}




///////////////////SCROLL HORIZONTAL///////////////////////////////////////

	//lo siguiente se encarga de chequear si se mueve el fondo y hace lo que corresponde (en nivel no 2)
	//el fondo solo se mueve si todos los jugadores activos tienen posX mayor a 0,
	//y si alguno de ellos esta por delante de la mitad  y tiene la tecla de movimiento apretada
	bool scrolleando=true;
	bool bobyPuede=false;
	bool boby2Puede=false;
	bool boby3Puede=false;
	bool boby4Puede=false;
	//el nivel no scrollea si el nivel es 2 o si la posicion de uno de los jugadores activos NO GRISADOS es 0.
	if((boby.enJuego()&&(boby.getPosX()<0))||(boby2.enJuego()&&(boby2.getPosX()<0))||(boby3.enJuego()&&(boby3.getPosX()<0))||(boby4.enJuego()&&(boby4.getPosX()<0))||(nivel==2)){
		scrolleando=false;
	}

	//si boby es activo, esta apretando derecha, y sobrepasa el ancho, boby puede causar un scroll.
	//Nota: si esta grisado o no es irrelevante aca porque tecnicamente no se pueden apretar las teclas (esta desconectado).
	if((boby.esActivo()&&!boby.muerto())&&(cliente->quiereAccion(Constantes::derecha))&&(boby.getPosX() > (ancho / 2)))
		bobyPuede=true;

	//NOTA: por ahora uso esta tecla para los otros 3 pero hay que cambiarla al input correspondiente
	if((boby2.esActivo()&&!boby2.muerto())&&(cliente2->quiereAccion(Constantes::derecha))&&(boby2.getPosX() > (ancho / 2)))
		boby2Puede=true;

	if((boby3.esActivo()&&!boby3.muerto())&&(cliente3->quiereAccion(Constantes::derecha))&&(boby3.getPosX() > (ancho / 2)))
		boby3Puede=true;

	if((boby4.esActivo()&&!boby4.muerto())&&(cliente4->quiereAccion(Constantes::derecha))&&(boby4.getPosX() > (ancho / 2)))
		boby4Puede=true;

	//SI NINGUNO de los 4 puede scrollear, no hay scrolleo.
	if((!bobyPuede)&&(!boby2Puede)&&(!boby3Puede)&&(!boby4Puede))
		scrolleando=false;

	if (rect_origen_fondo3.x >= mundo_w-ancho) {
		scrolleando=false;

	}
	//Ahora ya tengo la condicion.
	if(scrolleando){
		//scrollea el fondo horizontalmente.
		fondo1.avanzarOrigen(d1);
		fondo2.avanzarOrigen(d2);			
		rect_origen_fondo3.x += d3;

		if (rect_origen_fondo3.x > mundo_w-ancho) {
			rect_origen_fondo3.x = mundo_w-ancho;

		}

		
		
		//si un jugador tiene su tecla derecha apretada, hace que se mueve en su lugar hacia adelante
		//el resto retrocede respecto la pantalla (se mantiene su animacion) 
		if (cliente->quiereAccion(Constantes::derecha)){
			boby.hacerComoQueCamina();
			boby.subirCoordenadaXEn(d3);
		}else{
			//si un personaje esta grisado muevo su coordenada para adelante y dejo la posicion como esta
			if (boby.esGrisado()){
				boby.subirCoordenadaXEn(d3);
			}else {
				boby.decrementarPosX(d3);
			}
		}
		//era apretandoplayer2derecha(state)
		if (cliente2->quiereAccion(Constantes::derecha)){
			boby2.hacerComoQueCamina();
			boby2.subirCoordenadaXEn(d3);
		}else{
			if (boby2.esGrisado()){
				boby2.subirCoordenadaXEn(d3);
			}else {
				boby2.decrementarPosX(d3);
			}
		}
		if (cliente3->quiereAccion(Constantes::derecha)){
			boby3.hacerComoQueCamina();
			boby3.subirCoordenadaXEn(d3);
		}else{
			if (boby3.esGrisado()){
				boby3.subirCoordenadaXEn(d3);
			}else {
				boby3.decrementarPosX(d3);
			}
		}
		if (cliente4->quiereAccion(Constantes::derecha)){
			boby4.hacerComoQueCamina();
			boby4.subirCoordenadaXEn(d3);
		}else{
			if (boby4.esGrisado()){
				boby4.subirCoordenadaXEn(d3);
			}else {
				boby4.decrementarPosX(d3);
			}
		}
		//empujo enemigos para atras
		//NOTA MARTIN  //////////////////////////////////////
		for(unsigned i = 0; i < vectorEnemigos.size(); i++){
			vectorEnemigos[i]->empujarAtras(d3, nivel);
		}
		for(unsigned i = 0; i < vectorItems.size(); i++){
			vectorItems[i]->empujarAtras(d3, nivel);
		}
		for(unsigned i = 0; i < balasEnemigas.size(); i++){
			balasEnemigas[i]->empujarAtras(d3, nivel);
		}
		/////////////////////////////////////////////////////
		coordenada +=d3;

	}else{
		//esto es lo que pasa si el fondo no scrollea.
		if ((cliente->quiereAccion(Constantes::derecha))&&(boby.getPosX() < 760)){
			//SOLO PARA PLAYER1 (mueve los fondos en nivel 1 y 3)
			if ((num_jugadores==1)&&(nivel!=2)){
				fondo1.avanzarOrigen(d1);
				fondo2.avanzarOrigen(d2);
			}
			boby.avanzar();
			boby.subirCoordenadaXEn(d3);
		}
		if ((cliente2->quiereAccion(Constantes::derecha))&&(boby2.getPosX() < 760)){
			boby2.avanzar();
			boby2.subirCoordenadaXEn(d3);
		}
		if ((cliente3->quiereAccion(Constantes::derecha))&&(boby3.getPosX() < 760)){
			boby3.avanzar();
			boby3.subirCoordenadaXEn(d3);
		}
		if ((cliente4->quiereAccion(Constantes::derecha))&&(boby4.getPosX() < 760)){
			boby4.avanzar();
			boby4.subirCoordenadaXEn(d3);
		}
	}

	//ahora el movimiento hacia la izquierda
	if(!boby.esGrisado()&&(cliente->quiereAccion(Constantes::izquierda))&&(boby.getPosX() > 0 )){
		boby.retroceder();
		boby.subirCoordenadaXEn(-d3);
		//solo para player 1- scrolleo el fondo si hay un jugador		
		if((nivel != 2)&&(num_jugadores==1)){
			fondo1.avanzarOrigen(-d1);
			fondo2.avanzarOrigen(-d2);
		}
	}
	//muevo los otros jugadores (usar el input apropiado)
	if(!boby2.esGrisado()&&(cliente2->quiereAccion(Constantes::izquierda))&&(boby2.getPosX() > 0 )){
		boby2.retroceder();
		boby2.subirCoordenadaXEn(-d3);		
	}
	if(!boby3.esGrisado()&&(cliente3->quiereAccion(Constantes::izquierda))&&(boby3.getPosX() > 0 )){
		boby3.retroceder();
		boby3.subirCoordenadaXEn(-d3);		
	}
	if(!boby4.esGrisado()&&(cliente4->quiereAccion(Constantes::izquierda))&&(boby4.getPosX() > 0 )){
		boby4.retroceder();
		boby4.subirCoordenadaXEn(-d3);		
	}	

///////////////////FIN DE SCROLL HORIZONTAL, ALELUYA///////////////////////////////////


	//probando traductor	
	if(cliente->quiereAccion(Constantes::salto) && cliente->quiereAccion(Constantes::abajo) && !boby.estaSaltando()){
		boby.bajar();	
	}
	else{
		if(cliente->quiereAccion(Constantes::salto)){
			boby.saltar();	
		}
	}

	if(cliente2->quiereAccion(Constantes::salto) && cliente2->quiereAccion(Constantes::abajo) && !boby2.estaSaltando()){
		boby2.bajar();		
	}
	else{
		if(cliente2->quiereAccion(Constantes::salto))
				boby2.saltar();
	}
	if(cliente3->quiereAccion(Constantes::salto) && cliente3->quiereAccion(Constantes::abajo) && !boby3.estaSaltando()){
		boby3.bajar();		
	}
	else{
		if(cliente3->quiereAccion(Constantes::salto))
				boby3.saltar();
	}
	if(cliente4->quiereAccion(Constantes::salto) && cliente4->quiereAccion(Constantes::abajo) && !boby4.estaSaltando()){
		boby4.bajar();		
	}
	else{
		if(cliente4->quiereAccion(Constantes::salto))
				boby4.saltar();
	}



	if(cliente->quiereAccion(Constantes::agacharse) && !(cliente->quiereAccion(Constantes::derecha) || cliente->quiereAccion(Constantes::izquierda))){
		boby.agacharse();
	}

	if(! cliente->quiereAccion(Constantes::agacharse)){
		boby.pararse();
	}

	if(cliente2->quiereAccion(Constantes::agacharse) && !(cliente2->quiereAccion(Constantes::derecha) || cliente2->quiereAccion(Constantes::izquierda))){
	 boby2.agacharse();
	}
	if(! cliente2->quiereAccion(Constantes::agacharse)){
	 boby2.pararse();
	}
	if(cliente3->quiereAccion(Constantes::agacharse) && !(cliente3->quiereAccion(Constantes::derecha) || cliente3->quiereAccion(Constantes::izquierda))) boby3.agacharse();
	if(! cliente3->quiereAccion(Constantes::agacharse)) boby3.pararse();

	if(cliente4->quiereAccion(Constantes::agacharse) && !(cliente3->quiereAccion(Constantes::derecha) || cliente3->quiereAccion(Constantes::izquierda))) boby4.agacharse();
	if(! cliente4->quiereAccion(Constantes::agacharse)) boby4.pararse();

	if(cliente->quiereAccion(Constantes::nivel2)){
		if(nivel == 1){
			cambioNivel=true;
		}		
	}

	/*if(cliente->quiereAccion(Constantes::nivel3)){
		if(nivel == 2){
			cambioNivel=true;
		}
	}*/

	//DISPAROS
	if(!boby.esGrisado()&&boby.esActivo()&&cliente->quiereAccion(Constantes::disparo)){
		boby.pelarElChumbo();
		if(boby.puedeDisparar()){
			direccionDeBala = 0;
			if(cliente->quiereAccion(Constantes::arriba))
				direccionDeBala--;
			if(cliente->quiereAccion(Constantes::abajo))
				direccionDeBala++;

			int posBala = 10;
			if(!boby.estaMirandoALaDerecha())
				posBala = -10;
			boby.disparar(posBala,direccionDeBala*10,textura_bala);
		}
	}

	if(!boby2.esGrisado()&&boby2.esActivo()&&cliente2->quiereAccion(Constantes::disparo)){
		boby2.pelarElChumbo();
		if(boby2.puedeDisparar()){
			direccionDeBala = 0;
			if(cliente2->quiereAccion(Constantes::arriba))
				direccionDeBala--;
			if(cliente2->quiereAccion(Constantes::abajo))
				direccionDeBala++;

			int posBala = 10;
			if(!boby2.estaMirandoALaDerecha())
				posBala = -10;
			boby2.disparar(posBala,direccionDeBala*10,textura_bala2);
		}
	}

	if(!boby3.esGrisado()&&boby3.esActivo()&&cliente3->quiereAccion(Constantes::disparo)){
		boby3.pelarElChumbo();
		if(boby3.puedeDisparar()){
			direccionDeBala = 0;
			if(cliente3->quiereAccion(Constantes::arriba))
				direccionDeBala--;
			if(cliente3->quiereAccion(Constantes::abajo))
				direccionDeBala++;

			int posBala = 10;
			if(!boby3.estaMirandoALaDerecha())
				posBala = -10;
			boby3.disparar(posBala,direccionDeBala*10,textura_bala3);
		}
	}

	if(!boby4.esGrisado()&&boby4.esActivo()&&cliente4->quiereAccion(Constantes::disparo)){
		boby4.pelarElChumbo();
		if(boby4.puedeDisparar()){
			direccionDeBala = 0;
			if(cliente4->quiereAccion(Constantes::arriba))
				direccionDeBala--;
			if(cliente4->quiereAccion(Constantes::abajo))
				direccionDeBala++;

			int posBala = 10;
			if(!boby4.estaMirandoALaDerecha())
				posBala = -10;
			boby4.disparar(posBala,direccionDeBala*10,textura_bala4);
		}
	}

	if(! cliente->quiereAccion(Constantes::disparo)){
		boby.dejarDeDisparar();
	}

	if(! cliente2->quiereAccion(Constantes::disparo)){
		boby2.dejarDeDisparar();
	}


	if(! cliente3->quiereAccion(Constantes::disparo)){
		boby3.dejarDeDisparar();
	}

	if(! cliente4->quiereAccion(Constantes::disparo)){
		boby4.dejarDeDisparar();
	}

	if(cliente->quiereAccion(Constantes::arriba)){
		boby.apuntarArriba();
	}
	
	if(cliente->quiereAccion(Constantes::abajo)){
		boby.apuntarAbajo();
	}

	if(cliente2->quiereAccion(Constantes::arriba)){
		boby2.apuntarArriba();
	}
	if(cliente2->quiereAccion(Constantes::abajo)){
		boby2.apuntarAbajo();
	}
 	
 	if(cliente3->quiereAccion(Constantes::arriba)){
		boby3.apuntarArriba();
	}
	if(cliente3->quiereAccion(Constantes::abajo)){
		boby3.apuntarAbajo();
	}

	if(cliente4->quiereAccion(Constantes::arriba)){
		boby4.apuntarArriba();
	}
	if(cliente4->quiereAccion(Constantes::abajo)){
		boby4.apuntarAbajo();
	}


	//activo multijugador
	if((num_jugadores==1)&&(apretandoMultijugador2P(state))){
		num_jugadores=2;
		boby2.activar();
	}

	//activo multijugador4P
	if(apretandoMultijugador4P(state)){
		num_jugadores=4;
		boby2.activar();
		boby3.activar();
		boby4.activar();
	}

	if(apretandoGrisP2(state)){
		boby2.grisar();
	}
	if(apretandoDesGrisP2(state)){
		boby2.desgrisar(plataformas,nivel);
		
	}

	if(alguienQuiereGodMode()){

		/*if(boby.esGOD()){
			boby.desactivarGodMode();
			boby2.desactivarGodMode();
			printf("DESACTIVANDO GOD MODE\n");
			SDL_Delay(50);
			
		}
		else{*/
			printf("ACTIVANDO GOD MODE\n");
			godModeParaTodos();
			SDL_Delay(50);

		//}
		
	}


	if(! cliente->quiereAccion(Constantes::arriba) && ! cliente->quiereAccion(Constantes::abajo)){
		boby.dejarDeApuntar();
	}

	if(! cliente2->quiereAccion(Constantes::arriba) && ! cliente2->quiereAccion(Constantes::abajo)){
		boby2.dejarDeApuntar();
	}

	if(! cliente3->quiereAccion(Constantes::arriba) && ! cliente3->quiereAccion(Constantes::abajo)){
		boby3.dejarDeApuntar();
	}

	if(! cliente4->quiereAccion(Constantes::arriba) && ! cliente4->quiereAccion(Constantes::abajo)){
		boby4.dejarDeApuntar();
	}

	SDL_Event e;
	while (SDL_PollEvent (&e) != 0) {
		switch(e.type){

			case (SDL_QUIT):
				termino = true;
				break;

			default:
				break;
		}
	}

	loginfo("Se termina de manejar eventos de juego");
}

void juego::actualizarNivel1(){
	for (int i = 0; i < num_jugadores; i++) {
		pts.puntos (i, nivel-1, obtenerPuntaje(i, true));
	}
	
	cambioNivel=false;
	nivel=2;
	coordenada=3000;
	fondo1.obtenerTextura("//configuracion//escenarios//nivel2//fondo1", renderer);
	fondo2.obtenerTextura("//configuracion//escenarios//nivel2//fondo2", renderer);

	SDL_Texture* textura_fondo3_temp = cfg.obtener_textura ("//configuracion//escenarios//nivel2//fondo3", renderer);
	SDL_QueryTexture (textura_fondo3_temp, nullptr, nullptr, &mundo_w, &mundo_h);
	textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,mundo_w,mundo_h);
	SDL_DestroyTexture (textura_fondo3_temp);
	plataformas.cargarValoresFijos(textura_fondo3,renderer,2);

	fondo1.setRectOrigen(0,3600-600,800,600);
	fondo2.setRectOrigen(0,3600-600,800,600);
	rect_origen_fondo3={0,3600-600,800,600};
	boby.setPosX(50);
	boby.setPosY(400);
	boby.setCoordenadaX(0+50);
	boby.setCoordenadaY(3600-(600-400));

	boby2.setPosX(250);
	boby2.setPosY(400);
	boby2.setCoordenadaX(0+250);
	boby2.setCoordenadaY(3600-(600-400));

	boby3.setPosX(450);
	boby3.setPosY(400);
	boby3.setCoordenadaX(0+450);
	boby3.setCoordenadaY(3600-(600-400));

	boby4.setPosX(650);
	boby4.setPosY(400);
	boby4.setCoordenadaX(0+650);
	boby4.setCoordenadaY(3600-(600-400));
	
}

void juego::actualizarNivel2(){
	for (int i = 0; i < num_jugadores; i++) {
		pts.puntos (i, nivel-1, obtenerPuntaje(i, true));
	}

	cambioNivel=false;
	nivel=3;
	coordenada=800;
	fondo1.obtenerTextura("//configuracion//escenarios//nivel3//fondo1", renderer);
	fondo2.obtenerTextura("//configuracion//escenarios//nivel3//fondo2", renderer);

	SDL_Texture* textura_fondo3_temp = cfg.obtener_textura ("//configuracion//escenarios//nivel3//fondo3", renderer);
	SDL_QueryTexture (textura_fondo3_temp, nullptr, nullptr, &mundo_w, &mundo_h);
	textura_fondo3 =plataformas.crearTexturaParaElFondo(textura_fondo3_temp,renderer,mundo_w,mundo_h);
	SDL_DestroyTexture (textura_fondo3_temp);
	plataformas.cargarValoresFijos(textura_fondo3,renderer,3);

	fondo1.setRectOrigen(0,0,800,600);
	fondo2.setRectOrigen(0,0,800,600);
	rect_origen_fondo3={0,0,800,600};

	boby.setPosX(50);
	boby.setPosY(280);	
	boby.setCoordenadaX(0+50);
	boby.setCoordenadaY(280);

	boby2.setPosX(150);
	boby2.setPosY(280);
	boby2.setCoordenadaX(0+150);
	boby2.setCoordenadaY(280);

	boby3.setPosX(250);
	boby3.setPosY(280);
	boby3.setCoordenadaX(0+250);
	boby3.setCoordenadaY(280);

	boby4.setPosX(350);
	boby4.setPosY(280);
	boby4.setCoordenadaX(0+350);
	boby4.setCoordenadaY(280);

	fondo1.avanzarOrigen(50);
	fondo2.avanzarOrigen(50);
}

void juego::actualizarNivel3(){
	for (int i = 0; i < num_jugadores; i++) {
		pts.puntos (i, nivel-1, obtenerPuntaje(i, true));
	}
	termino = true;
}

void juego::grisarJugador(int numeroDeJugador){
	switch(numeroDeJugador){
		case (1):
			boby.grisar();
			break;
		case (2):
			boby2.grisar();
			break;
		case (3):
			boby3.grisar();
			break;
		case (4):
			boby4.grisar();
			break;
		default:
			break;
	}
}
	
void juego::desgrisarJugador(int numeroDeJugador){
	switch(numeroDeJugador){
		case (1):
			boby.desgrisar(plataformas,nivel);
			break;
		case (2):
			boby2.desgrisar(plataformas,nivel);
			break;
		case (3):
			boby3.desgrisar(plataformas,nivel);
			break;
		case (4):
			boby4.desgrisar(plataformas,nivel);
			break;
		default:
			break;
	}
}
	

void juego::actualizar ()
{
	loginfo("Se comienza actualizar juego");

////////////////////////////SCROLL VERTICAL/////////////////////////////////

	//parecido al scroll horizontal. solo se puede scrollear SI:
	//1) es el nivel 2, y ninguno de los jugadores activos esta en el borde inferior.
	//2) para al menos uno de los jugadores activos, supera los 200 pixeles, se esta moviendo para arriba y no toca el techo.
	//notese que tambien debo obtener la maxima velocidad Y para determinar por cuanto debo hacer el scroll.
	bool scrolleando=true;
	bool bobyPuede=false;
	bool boby2Puede=false;
	bool boby3Puede=false;
	bool boby4Puede=false;
	int maxVel=0;

	//vemos si pasa 1)
	if((boby.enJuego()&&(boby.getPosY()>500))||(boby2.enJuego()&&(boby2.getPosY()>500))||(boby3.enJuego()&&(boby3.getPosY()>500))||(boby4.enJuego()&&(boby4.getPosY()>500))||(nivel!=2)){
		scrolleando=false;
	}

	//si boby es activo, supera los 200px, se mueve para arriba y no toca el techo, boby puede causar un scroll.
	if(boby.esActivo()&&!boby.muerto()&&(boby.getPosY() <=(200))&&(boby.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby.obtenerVelocidadY() > 0)){
		bobyPuede=true;
		if(boby.obtenerVelocidadY()<maxVel)
			maxVel=boby.obtenerVelocidadY();
	}
	if(boby2.esActivo()&&!boby2.muerto()&&(boby2.getPosY() <=(200))&&(boby2.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby2.obtenerVelocidadY() > 0)){
		boby2Puede=true;
		if(boby2.obtenerVelocidadY()<maxVel)
			maxVel=boby2.obtenerVelocidadY();
	}
	if(boby3.esActivo()&&!boby3.muerto()&&(boby3.getPosY() <=(200))&&(boby3.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby3.obtenerVelocidadY() > 0)){
		boby3Puede=true;
		if(boby3.obtenerVelocidadY()<maxVel)
			maxVel=boby3.obtenerVelocidadY();
	}
	if(boby4.esActivo()&&!boby4.muerto()&&(boby4.getPosY() <=(200))&&(boby4.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby4.obtenerVelocidadY() > 0)){
		boby4Puede=true;
		if(boby4.obtenerVelocidadY()<maxVel)
			maxVel=boby4.obtenerVelocidadY();
	}

	//SI NINGUNO de los 4 puede scrollear, no hay scrolleo.
	if((!bobyPuede)&&(!boby2Puede)&&(!boby3Puede)&&(!boby4Puede))
		scrolleando=false;

	//Ya determine si puedo scrollear, Y! la velocidad maxima.

	if(scrolleando){
		//scrolleo verticalmente.

		if(boby.esGrisado()){
			boby.subirCoordenadaYEn(maxVel);
		}else{
			boby.decrementarPosY(maxVel);
		}
		if(boby2.esGrisado()){
			boby2.subirCoordenadaYEn(maxVel);
		}else{
			boby2.decrementarPosY(maxVel);
		}
		if(boby3.esGrisado()){
			boby3.subirCoordenadaYEn(maxVel);
		}else{
			boby3.decrementarPosY(maxVel);
		}
		if(boby4.esGrisado()){
			boby4.subirCoordenadaYEn(maxVel);
		}else{
			boby4.decrementarPosY(maxVel);
		}
		//empujo enemigos para abajo
		//NOTA MARTIN //////////////////////////////////
		for(unsigned i = 0; i < vectorEnemigos.size(); i++){
			vectorEnemigos[i]->empujarAtras(maxVel, nivel);
		}

		for(unsigned i = 0; i < vectorItems.size(); i++){
			vectorItems[i]->empujarAtras(maxVel, nivel);
		}
		for(unsigned i = 0; i < balasEnemigas.size(); i++){
			balasEnemigas[i]->empujarAtras(maxVel, nivel);
		}
		////////////////////////////////////////////////
		coordenada -=maxVel;

		fondo1.avanzarOrigenY(maxVel/3);
		fondo2.avanzarOrigenY(maxVel/2);
		rect_origen_fondo3.y += maxVel;
	}
	else{
		//si no se puede scrollear, evito que los jugadores que pueden moverse por arriba del borde se muevan.
		if(boby.esActivo()&&(boby.getPosY()<=(0))&&(boby.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby.obtenerVelocidadY() > 0)){
			boby.decrementarPosY(boby.obtenerVelocidadY());
			boby.subirCoordenadaYEn(-boby.obtenerVelocidadY());
			}
		if(boby2.esActivo()&&(boby2.getPosY()<=(0))&&(boby2.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby2.obtenerVelocidadY() > 0)){
			boby2.decrementarPosY(boby2.obtenerVelocidadY());
			boby2.subirCoordenadaYEn(-boby2.obtenerVelocidadY());
			}
		if(boby3.esActivo()&&(boby3.getPosY()<=(0))&&(boby3.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby3.obtenerVelocidadY() > 0)){
			boby3.decrementarPosY(boby3.obtenerVelocidadY());
			boby3.subirCoordenadaYEn(-boby3.obtenerVelocidadY());
			}
		if(boby4.esActivo()&&(boby4.getPosY()<=(0))&&(boby4.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby4.obtenerVelocidadY() > 0)){
			boby4.decrementarPosY(boby4.obtenerVelocidadY());
			boby4.subirCoordenadaYEn(-boby4.obtenerVelocidadY());
			}
	}

/////////////////////////FIN DE SCROLL VERTICAL/////////////////////////////



	//efecto cascada
	if(nivel==2)
	{
		cascada+=2;
		fondo1.avanzarOrigenY(-2);		
		if(cascada==32){
			fondo1.avanzarOrigenY(32);
			cascada=0;
		}
	}

	boby.actualizar();
	boby2.actualizar();
	boby3.actualizar();
	boby4.actualizar();

	while (us >= periodo*0.9 ) {
		us -= periodo;
		
	}

	/*if((nivel==1)&& (((!boby.esGrisado() && boby.llegoAlFinalDelNivel1()) || (!boby2.esGrisado() && boby2.llegoAlFinalDelNivel1()) || (!boby3.esGrisado() && boby3.llegoAlFinalDelNivel1()) || (!boby4.esGrisado() && boby4.llegoAlFinalDelNivel1())) || cambioNivel== true)) actualizarNivel1();
	if((nivel==2)&& (((!boby.esGrisado() && boby.llegoAlFinalDelNivel2()) || (!boby2.esGrisado() && boby2.llegoAlFinalDelNivel2()) || (!boby3.esGrisado() && boby3.llegoAlFinalDelNivel2()) || (!boby4.esGrisado() && boby4.llegoAlFinalDelNivel2())) || cambioNivel== true)) actualizarNivel2();
	if((nivel ==3) && ((!boby.esGrisado() && boby.llegoAlFinalDelNivel3()) || (!boby2.esGrisado() && boby2.llegoAlFinalDelNivel3()) || (!boby3.esGrisado() && boby3.llegoAlFinalDelNivel3()) || (!boby4.esGrisado() && boby4.llegoAlFinalDelNivel3()))) actualizarNivel3();
	*/

	if((nivel == 1) && cambioNivel)actualizarNivel1();
	if((nivel == 2) && cambioNivel)actualizarNivel2();
	if((nivel == 3) && cambioNivel)actualizarNivel3();

	//actualiza el shootTimer del jugador (para que no tire 500 tiros por segundo)
	//tambien el movimiento de las balas y borro las que exceden su rango
	boby.refreshBullets();
	boby2.refreshBullets();
	boby3.refreshBullets();
	boby4.refreshBullets();


	//veo si el jugador toca al enemigo
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		if(! enemigoNivel1->derrotado() && collision(boby.getRectaDestino(),enemigoNivel1->getRectaDestino())){
			if(boby.getInvincibilityFrames() == 0)
				boby.perderVida();
		}
	}

	//veo COLISIONES

	//NOTA MARTIN ///////////////////////////////////
	int i=0;
	int pickup=0;

	if(boby.enJuego()&&(boby.getInvincibilityFrames() == 0)){

		//veo si toca algun enemigo
		for(int i = 0; i < vectorEnemigos.size(); i++){
			if(vectorEnemigos[i]->esActivo() && !vectorEnemigos[i]->derrotado() && collision(boby.getRectaDestino(),vectorEnemigos[i]->getRectaDestino())){
				boby.perderVida();
			}
		}
		//veo si toca las balas de los enemigos
		i=0;
		while (i<balasEnemigas.size()){
			if (collision(boby.getRectaDestino(),balasEnemigas[i]->getRectaDestino())){
				balasEnemigas.erase(balasEnemigas.begin() + i);
				boby.perderVida();
			}else i++;
		}
	}

	//veo si toca algun item
	if(boby.enJuego()){
		i=0;
		while (i<vectorItems.size()){
			if (!vectorItems[i]->derrotado() && collision(boby.getRectaDestino(),vectorItems[i]->getRectaDestino())){
				vectorItems[i]->perderVida();
				pickup= vectorItems[i]->darContenido();
				boby.cambiarArma(pickup);
				vectorItems.erase(vectorItems.begin() + i);
			}else i++;
		}
	}

	if(boby2.enJuego()&&(boby2.getInvincibilityFrames() == 0)){

		//veo si toca algun enemigo
		for(int i = 0; i < vectorEnemigos.size(); i++){
			if(!vectorEnemigos[i]->derrotado() && collision(boby2.getRectaDestino(),vectorEnemigos[i]->getRectaDestino())){
				boby2.perderVida();
			}
		}


		//veo si toca las balas de los enemigos
		i=0;
		while (i<balasEnemigas.size()){
			if (collision(boby2.getRectaDestino(),balasEnemigas[i]->getRectaDestino())){
				balasEnemigas.erase(balasEnemigas.begin() + i);
				boby2.perderVida();
			}else i++;
		}
	}
	//veo si toca algun item
	if(boby2.enJuego()){
		i=0;
		while (i<vectorItems.size()){
			if (!vectorItems[i]->derrotado() && collision(boby2.getRectaDestino(),vectorItems[i]->getRectaDestino())){
				vectorItems[i]->perderVida();
				pickup= vectorItems[i]->darContenido();
				boby2.cambiarArma(pickup);
				vectorItems.erase(vectorItems.begin() + i);
			}else i++;
		}
	}

	if(boby3.enJuego()&&(boby3.getInvincibilityFrames() == 0)){

		//veo si toca algun enemigo
		for(int i = 0; i < vectorEnemigos.size(); i++){
			if(!vectorEnemigos[i]->derrotado() && collision(boby3.getRectaDestino(),vectorEnemigos[i]->getRectaDestino())){
				boby3.perderVida();
			}
		}

		//veo si toca las balas de los enemigos
		i=0;
		while (i<balasEnemigas.size()){
			if (collision(boby3.getRectaDestino(),balasEnemigas[i]->getRectaDestino())){
				balasEnemigas.erase(balasEnemigas.begin() + i);
				boby3.perderVida();
			}else i++;
		}
	}
	//veo si toca algun item
	if(boby3.enJuego()){
		i=0;
		while (i<vectorItems.size()){
			if (!vectorItems[i]->derrotado() && collision(boby3.getRectaDestino(),vectorItems[i]->getRectaDestino())){
				vectorItems[i]->perderVida();
				pickup= vectorItems[i]->darContenido();
				boby3.cambiarArma(pickup);
				vectorItems.erase(vectorItems.begin() + i);
			}else i++;
		}
	}

	if(boby4.enJuego()&&(boby4.getInvincibilityFrames() == 0)){

		//veo si toca algun enemigo
		for(int i = 0; i < vectorEnemigos.size(); i++){
			if(!vectorEnemigos[i]->derrotado() && collision(boby4.getRectaDestino(),vectorEnemigos[i]->getRectaDestino())){
				boby4.perderVida();
			}
		}

		//veo si toca las balas de los enemigos
		i=0;
		while (i<balasEnemigas.size()){
			if (collision(boby4.getRectaDestino(),balasEnemigas[i]->getRectaDestino())){
				balasEnemigas.erase(balasEnemigas.begin() + i);
				boby4.perderVida();
			}else i++;
		}
	}
	//veo si toca algun item
	if(boby4.enJuego()){
		i=0;
		while (i<vectorItems.size()){
			if (!vectorItems[i]->derrotado() && collision(boby4.getRectaDestino(),vectorItems[i]->getRectaDestino())){
				vectorItems[i]->perderVida();
				pickup= vectorItems[i]->darContenido();
				boby4.cambiarArma(pickup);
				vectorItems.erase(vectorItems.begin() + i);
			}else i++;
		}
	}
	////////////////////////////////////////////////////


	//veo si el jugador se cayo
	if(boby.getPosY()>600){
		boby.perderVida();
		boby.resetearPosicion(plataformas, nivel);	
		boby.cambiarArma(0);
	}
	if(boby2.getPosY()>600){
		boby2.perderVida();
		boby2.resetearPosicion(plataformas, nivel);	
		boby2.cambiarArma(0);
	}
	if(boby3.getPosY()>600){
		boby3.perderVida();
		boby3.resetearPosicion(plataformas, nivel);	
		boby3.cambiarArma(0);
	}
	if(boby4.getPosY()>600){
		boby4.perderVida();
		boby4.resetearPosicion(plataformas, nivel);	
		boby4.cambiarArma(0);
	}

	//refresco el tiempo de invincibilidad
	boby.refreshIFrames();
	boby2.refreshIFrames();
	boby3.refreshIFrames();
	boby4.refreshIFrames();

	

	//veo si las balas le pegan al enemigo
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		boby.verSiBalasPegan(enemigoNivel1);
	}
	//veo si las balas le pegan al enemigo
	//NOTA MARTIN /////////////////////////////////////////
	i=0;

	int dropItem=0;
	int coordXItem=0;
	int coordYItem=0;
	int posYItem=0;
	int posXItem=0;
	while (i<vectorEnemigos.size()){
		if(vectorEnemigos[i]->esActivo()){
			boby.verSiBalasPegan(vectorEnemigos[i]);
			boby2.verSiBalasPegan(vectorEnemigos[i]);
			boby3.verSiBalasPegan(vectorEnemigos[i]);
			boby4.verSiBalasPegan(vectorEnemigos[i]);

			if (vectorEnemigos[i]->derrotado()){
				//pruebo suerte!
				//POSTA: va de 2 a 10
				//dropItem= rand() % 10 + 2;
				//DEBUG: va de 1 a 4
				dropItem= rand() % 4 + 2;
				if(dropItem<=4){
					//siiii! suelta un item!
					coordXItem= vectorEnemigos[i]->coordenadaXParaItem();
					coordYItem= vectorEnemigos[i]->coordenadaYParaItem();
					posXItem= vectorEnemigos[i]->posicionXParaItem();
					posYItem= vectorEnemigos[i]->posicionYParaItem();

					Item* nuevoItem;
					nuevoItem = new Item(posXItem,posYItem,coordXItem,coordYItem,dropItem,nivel);
					//nuevoItem = new Item(400,200,400,400,dropItem,nivel);
					if(dropItem==1) nuevoItem->obtenerTextura("//configuracion//items//vidaitem//sprite", renderer);
					if(dropItem==2) nuevoItem->obtenerTextura("//configuracion//items//ametralladora//sprite", renderer);
					if(dropItem==3) nuevoItem->obtenerTextura("//configuracion//items//escopeta//sprite", renderer);
					if(dropItem==4) nuevoItem->obtenerTextura("//configuracion//items//bazooka//sprite", renderer);
					vectorItems.push_back(nuevoItem);
				}
				vectorEnemigos.erase(vectorEnemigos.begin() + i);
			}
			else i++;
		} else i++;
	}
	///////////////////////////////////////////////////////

	//actualizo el enemigo

	//NOTA MARTIN /////////////////////////////////////////
	Bullet* nuevaBala;
	int maxCoor=this->maximaCoordenadaJugadores();
	int maxPos=this->maximaPosicionJugadores();

	for(i = 0; i < vectorEnemigos.size(); i++){
		if(!vectorEnemigos[i]->esActivo())
			vectorEnemigos[i]->activar(nivel, maxCoor, maxPos);
			
		if(vectorEnemigos[i]->esActivo())
			vectorEnemigos[i]->actualizar(nivel, coordenada);
		//le agrego el tengoQueEnviarEnemigo para q no disparen los que estan afuera de la pantalla
		if(vectorEnemigos[i]->quiereDisparar() && tengoQueEnviarEnemigo(i)){
			nuevaBala = new Bullet(vectorEnemigos[i]->posicionXParaItem(),vectorEnemigos[i]->posicionYParaItem(),vectorEnemigos[i]->disparoXVel(),vectorEnemigos[i]->disparoYVel(),2,100);
			nuevaBala->obtenerTextura("//configuracion//items//bala2//sprite", renderer);
			vectorEnemigos[i]->disparo();
			balasEnemigas.push_back(nuevaBala);
		}
	}
	
	//borro los que se pasaron del borde o estan derrotados
	i=0;
	while (i<vectorEnemigos.size()){
		if ((vectorEnemigos[i]->pasaBorde(nivel))||(nivel>(vectorEnemigos[i]->obtenerNivelActivo()))||(vectorEnemigos[i]->derrotado()))
			vectorEnemigos.erase(vectorEnemigos.begin() + i);
		else i++;
	}

	//actualizo los items

	for(i = 0; i < vectorItems.size(); i++){
		if(vectorItems[i]->esActivo())
			vectorItems[i]->actualizarItem();
	}

	//borro los que se pasaron del borde o estan derrotados
	i=0;
	while (i<vectorItems.size()){
		if (/*(vectorItems[i]->pasaBorde(nivel))||*/(nivel>(vectorItems[i]->obtenerNivelActivo()))||(vectorItems[i]->derrotado()))
			vectorItems.erase(vectorItems.begin() + i);
		else i++;
	}

	//Actualizo balas enemigas

	for(int i = 0; i < balasEnemigas.size(); i++){
		balasEnemigas[i]->move();
	}

	//borro las balas que exceden su rango para que no sigan hasta el infinito
	i=0;
	while (i<balasEnemigas.size()){
		if (balasEnemigas[i]->getDuracion() ==0)
			balasEnemigas.erase(balasEnemigas.begin() + i);
		else i++;
	}
	/////////////////////////////////////////////////////

	loginfo("Se termina de actualizar juego");
	
}

int juego::maximaCoordenadaJugadores(){
	int max=0;
	if(nivel==2){
		max=3600;
		if(boby.enJuego()&&(boby.obtenerCoordenadaY()<max))
			max=boby.obtenerCoordenadaY();
		if(boby2.enJuego()&&(boby2.obtenerCoordenadaY()<max))
			max=boby2.obtenerCoordenadaY();
		if(boby3.enJuego()&&(boby3.obtenerCoordenadaY()<max))
			max=boby3.obtenerCoordenadaY();
		if(boby4.enJuego()&&(boby4.obtenerCoordenadaY()<max))
			max=boby4.obtenerCoordenadaY();
	} else{
		if(boby.enJuego()&&(boby.obtenerCoordenadaX()>max))
			max=boby.obtenerCoordenadaX();
		if(boby2.enJuego()&&(boby2.obtenerCoordenadaX()>max))
			max=boby2.obtenerCoordenadaX();
		if(boby3.enJuego()&&(boby3.obtenerCoordenadaX()>max))
			max=boby3.obtenerCoordenadaX();
		if(boby4.enJuego()&&(boby4.obtenerCoordenadaX()>max))
			max=boby4.obtenerCoordenadaX();
	}
	return max;
}

int juego::maximaPosicionJugadores(){
	int max=0;
	if(nivel==2){
		max=600;
		if(boby.enJuego()&&(boby.getPosY()<max))
			max=boby.getPosY();
		if(boby2.enJuego()&&(boby2.getPosY()<max))
			max=boby2.getPosY();
		if(boby3.enJuego()&&(boby3.getPosY()<max))
			max=boby3.getPosY();
		if(boby4.enJuego()&&(boby4.getPosY()<max))
			max=boby4.getPosY();
	} else{
		if(boby.enJuego()&&(boby.getPosY()>max))
			max=boby.getPosY();
		if(boby2.enJuego()&&(boby2.getPosY()>max))
			max=boby2.getPosY();
		if(boby3.enJuego()&&(boby3.getPosY()>max))
			max=boby3.getPosY();
		if(boby4.enJuego()&&(boby4.getPosY()>max))
			max=boby4.getPosY();
	}
	return max;
}

void juego::cargarEnemigosNivel1(int cantEnemigos){
	Enemigo* nuevoEnemigo;
	int coor=0;
	for (int i = 0; i < cantEnemigos; i++) {
		//cargo una coordenada aleatoria entre 800 (borde derecho de primera pantalla) y 6000 (antes del jefe)
		coor= rand() % 7000 + 800;
		//decido cual enemigo cargo, esto es aleatorio de 1 a 4
		//saco el resto de dividir la coordenada por 4 para ahorrarme hacer otro rand ;)
		switch(coor%4){
			case (0):
				//marciano
				nuevoEnemigo = new Marcianito(800,240,1,coor,1);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (1):
				//ovni
				nuevoEnemigo = new Ovni(800,80,1,coor,1);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (2):
				//turret
				nuevoEnemigo = new Turret(800,400,1,coor,1);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//turret//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (3):
				//paracaidas
				nuevoEnemigo = new Paracaidas(600,-100,1,coor,1);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//parachute//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
		}
	}
}

void juego::cargarEnemigosNivel2(int cantEnemigos){
	Enemigo* nuevoEnemigo;
	int coor=0;
	for (int i = 0; i < cantEnemigos; i++) {
		//cargo una coordenada aleatoria entre 3000 (borde superior de primera pantalla) y 600 (antes del jefe)
		coor= rand() % 3000 + 600;
		//decido cual enemigo cargo, esto es aleatorio de 1 a 4
		//saco el resto de dividir la coordenada por 4 para ahorrarme hacer otro rand ;)
		switch(coor%4){
			case (0):
				//marciano
				nuevoEnemigo = new Marcianito(200,-100,1,coor,2);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (1):
				//ovni
				nuevoEnemigo = new Ovni(800,100,1,coor,2);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (2):
				//turret
				nuevoEnemigo = new Turret(300,-80,1,coor,2);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//turret//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (3):
				//paracaidas
				nuevoEnemigo = new Paracaidas(600,-100,1,coor,2);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//parachute//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
		}
	}
}

void juego::cargarEnemigosNivel3(int cantEnemigos){
	Enemigo* nuevoEnemigo;
	int coor=0;
	for (int i = 0; i < cantEnemigos; i++) {
		//cargo una coordenada aleatoria entre 800 (borde derecho de primera pantalla) y 6000 (antes del jefe)
		coor= rand() % 7000 + 800;
		//decido cual enemigo cargo, esto es aleatorio de 1 a 4
		//saco el resto de dividir la coordenada por 4 para ahorrarme hacer otro rand ;)
		switch(coor%4){
			case (0):
				//marciano
				nuevoEnemigo = new Marcianito(800,240,1,coor,3);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (1):
				//ovni
				nuevoEnemigo = new Ovni(800,80,1,coor,3);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (2):
				//turret
				nuevoEnemigo = new Turret(800,400,1,coor,3);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//turret//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
			case (3):
				//paracaidas
				nuevoEnemigo = new Paracaidas(600,-100,1,coor,3);
				nuevoEnemigo->obtenerTextura("//configuracion//personajes//parachute//sprite", renderer);
				vectorEnemigos.push_back(nuevoEnemigo);
				break;
		}
	}
}

void juego::setDatosEnemigoFinal(){
	bool hayEnemigo = false;
	bool cerca = (boby.estaCercaDelFinalDelNivel1() || boby2.estaCercaDelFinalDelNivel1() || boby3.estaCercaDelFinalDelNivel1() || boby4.estaCercaDelFinalDelNivel1());

	bool cerca2 = (boby.estaCercaDelFinalDelNivel2() || boby2.estaCercaDelFinalDelNivel2() || boby3.estaCercaDelFinalDelNivel2() || boby4.estaCercaDelFinalDelNivel2());

	bool cerca3 = (boby.estaCercaDelFinalDelNivel3() || boby2.estaCercaDelFinalDelNivel3() || boby3.estaCercaDelFinalDelNivel3() || boby4.estaCercaDelFinalDelNivel3());

	switch(nivel){
		case (1):
			if(cerca && !enemigoNivel1->derrotado()){
				hayEnemigo = true;
			}
			break;
		case (2):
			if(cerca2 && !enemigoNivel2->derrotado()){
				hayEnemigo = true;
			}
			break;
		case (3):
			if(cerca3 && !enemigoNivel3->derrotado()){
				hayEnemigo = true;
			}
			break;
			
	}

	armador->setEnemigoFinal(hayEnemigo);
}

bool juego::tengoQueEnviarEnemigo(int i){
	return (!vectorEnemigos[i]->derrotado() && vectorEnemigos[i]->esActivo() && vectorEnemigos[i]->estoyEnEscena(rect_origen_fondo3.x,rect_origen_fondo3.y,rect_origen_fondo3.w,rect_origen_fondo3.h));
}

void juego::setDatosEnemigos(){
	string mensajeEnemigos = "";
	for(unsigned i = 0; i < vectorEnemigos.size(); i++){
		if(tengoQueEnviarEnemigo(i))
			mensajeEnemigos += vectorEnemigos[i]->serializar();
	}

	armador->setMensajeEnemigos(mensajeEnemigos);
}

void juego::setDatosItems(){
	string mensajeItems = "";
	for(unsigned i = 0; i < vectorItems.size(); i++){
		if(vectorItems[i]->puedoDibujar())
			mensajeItems += vectorItems[i]->serializar();
	}

	armador->setMensajeItems(mensajeItems);
}

void juego::setDatosBalasEnemigas(){
	string mensajeBalasEnemigas = "";
	for(unsigned i = 0; i < balasEnemigas.size(); i++){
		mensajeBalasEnemigas += balasEnemigas[i]->serializar();
	}

	armador->setMensajeBalasEnemigas(mensajeBalasEnemigas);
}

string juego::armarRespuesta(){

	//esto armo siempre
	armador->setNivel(nivel);
	armador->setFondo1(fondo1.getRectaOrigen().x,fondo1.getRectaOrigen().y);
	armador->setFondo2(fondo2.getRectaOrigen().x,fondo2.getRectaOrigen().y);
	armador->setFondo3(rect_origen_fondo3.x,rect_origen_fondo3.y);

	//Cargo los datos segun cuantos jugadores alla.

	if (num_jugadores >= 1){

		//printf("posicion y en armador de respuesta: %i\n",boby.getPosY() );
		datosBoby.setPosX(boby.getPosX());
		datosBoby.setPosY(boby.getPosY());


		datosBoby.setSaltando(boby.estaSaltando());
		datosBoby.setDisparando(boby.estaDisparando());
		datosBoby.setMirandoALaDerecha(boby.estaMirandoALaDerecha());
		datosBoby.setActivo(boby.esActivo());
		datosBoby.setGrisado(boby.esGrisado());
		datosBoby.setEstado( (Constantes::Estado) boby.getEstado());
		datosBoby.setDireccionDisparo( (Constantes::DireccionDisparo) boby.getDireccionDisparo());

		setCantidadDeBalas();
		armador->sumarBalas(boby.getBalas(),boby.getArma());

	}

	if(num_jugadores >= 2){
		datosBoby2.setPosX(boby2.getPosX());
		datosBoby2.setPosY(boby2.getPosY());
		datosBoby2.setSaltando(boby2.estaSaltando());
		datosBoby2.setDisparando(boby2.estaDisparando());
		datosBoby2.setMirandoALaDerecha(boby2.estaMirandoALaDerecha());
		datosBoby2.setActivo(boby2.esActivo());
		datosBoby2.setGrisado(boby2.esGrisado());
		datosBoby2.setEstado( (Constantes::Estado) boby2.getEstado());
		datosBoby2.setDireccionDisparo( (Constantes::DireccionDisparo) boby2.getDireccionDisparo());

		armador->sumarBalas(boby2.getBalas(),boby2.getArma());
	}

	if (num_jugadores >= 3){
		datosBoby3.setPosX(boby3.getPosX());
		datosBoby3.setPosY(boby3.getPosY());
		datosBoby3.setSaltando(boby3.estaSaltando());
		datosBoby3.setDisparando(boby3.estaDisparando());
		datosBoby3.setMirandoALaDerecha(boby3.estaMirandoALaDerecha());
		datosBoby3.setActivo(boby3.esActivo());
		datosBoby3.setGrisado(boby3.esGrisado());
		datosBoby3.setEstado( (Constantes::Estado) boby3.getEstado());
		datosBoby3.setDireccionDisparo( (Constantes::DireccionDisparo) boby3.getDireccionDisparo());

		armador->sumarBalas(boby3.getBalas(),boby3.getArma());
	}

	if (num_jugadores == 4){
		datosBoby4.setPosX(boby4.getPosX());
		datosBoby4.setPosY(boby4.getPosY());
		datosBoby4.setSaltando(boby4.estaSaltando());
		datosBoby4.setDisparando(boby4.estaDisparando());
		datosBoby4.setMirandoALaDerecha(boby4.estaMirandoALaDerecha());
		datosBoby4.setActivo(boby4.esActivo());
		datosBoby4.setGrisado(boby4.esGrisado());
		datosBoby4.setEstado( (Constantes::Estado) boby4.getEstado());
		datosBoby4.setDireccionDisparo( (Constantes::DireccionDisparo) boby4.getDireccionDisparo());

		armador->sumarBalas(boby4.getBalas(),boby4.getArma());
	}

	//este es el enemigo final
	setDatosEnemigoFinal();
	
	string vidas = "";
	vidas += to_string(boby.obtenerVidas());
	vidas += to_string(boby2.obtenerVidas());
	vidas += to_string(boby3.obtenerVidas());
	vidas += to_string(boby4.obtenerVidas());
	armador->setMensajeVidas(vidas);

	return armador->dameLaRespuestaPara(num_jugadores, &datosBoby, &datosBoby2, &datosBoby3, &datosBoby4);
	

}

void juego::dibujarVidas(){
	//////////dibujo vidas///////////////////////////////
	int nrovidas;

	nrovidas=boby.obtenerVidas();
	if(nrovidas>0){
		vida11.dibujar(renderer);
		if(nrovidas>1)
			vida12.dibujar(renderer);
		if(nrovidas>2)
			vida13.dibujar(renderer);
	} else{
		gameover1.dibujar(renderer);
	}

	if(num_jugadores>=2){
		nrovidas=boby2.obtenerVidas();
			if(nrovidas>0){
				vida21.dibujar(renderer);
			if(nrovidas>1)
				vida22.dibujar(renderer);
			if(nrovidas>2)
				vida23.dibujar(renderer);
		} else{
			gameover2.dibujar(renderer);
		}
	}

	if(num_jugadores>=3){
		nrovidas=boby3.obtenerVidas();
			if(nrovidas>0){
				vida31.dibujar(renderer);
			if(nrovidas>1)
				vida32.dibujar(renderer);
			if(nrovidas>2)
				vida33.dibujar(renderer);
		} else{
			gameover3.dibujar(renderer);
		}
	}



	if(num_jugadores>=4){
		nrovidas=boby4.obtenerVidas();
			if(nrovidas>0){
				vida41.dibujar(renderer);
			if(nrovidas>1)
				vida42.dibujar(renderer);
			if(nrovidas>2)
				vida43.dibujar(renderer);
		} else{
			gameover4.dibujar(renderer);
		}
	}

	/////////////////////////////////////////
}

void juego::dibujar ()
{
	loginfo("Se comienza a dibujar juego");
	
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
		if(!enemigoNivel1->derrotado())
			enemigoNivel1->dibujar(renderer);
		else{
			enemigoNivel1->~Enemigo(); // Esto no libera memoria
		}
	}

	//dibujar enemigos
	//NOTA MARTIN ////////////////////////////////////////////////
	for(unsigned i = 0; i < vectorEnemigos.size(); i++){
		if(!vectorEnemigos[i]->derrotado()&&vectorEnemigos[i]->esActivo())
			if(vectorEnemigos[i]->visible){
				vectorEnemigos[i]->dibujar(renderer);
			}
	}

	//dibujo items
	for(unsigned i = 0; i < vectorItems.size(); i++){
		if(vectorItems[i]->puedoDibujar())
			vectorItems[i]->dibujar(renderer);
	}

	//dibujo balas
	for(unsigned i = 0; i < balasEnemigas.size(); i++){
		balasEnemigas[i]->dibujar(renderer);
	}

	//////////////////////////////////////////////////////////////

	/*
	if(nivel == 2 && boby.estaCercaDelFinalDelNivel2()){
		if(!enemigoNivel2->derrotado())
			enemigoNivel2->dibujar(renderer);
		else{
			delete(enemigoNivel2);
		}
	}

	if(nivel == 3 && boby.estaCercaDelFinalDelNivel3()){
		if(!enemigoNivel3->derrotado())
			enemigoNivel3->dibujar(renderer);
		else{
			delete(enemigoNivel3);
		}
	}*/

	// Copio a los jugadores (LO HAGO EN ORDEN INVERSO ASI SI SE SUPERPONEN APARECEN P1 arriba del 2, etc.)
	if((num_jugadores>=4)&&((boby4.getInvincibilityFrames()/2) %2 == 0))
		boby4.dibujar(renderer);
	if((num_jugadores>=3)&&((boby3.getInvincibilityFrames()/2) %2 == 0))
		boby3.dibujar(renderer);
	if((num_jugadores>=2)&&((boby2.getInvincibilityFrames()/2) %2 == 0))
		boby2.dibujar(renderer);
	if((boby.getInvincibilityFrames()/2) %2 == 0)
		boby.dibujar(renderer);


	boby.dibujarBalas(renderer);
	boby2.dibujarBalas(renderer);
	boby3.dibujarBalas(renderer);
	boby4.dibujarBalas(renderer);

	//Lo paso a este metodo para reusarlo desde juegoCliente
	dibujarVidas();

	loginfo("Se termina de dibujar juego");
	
}

void juego::presentar ()
{
	loginfo("Se comienza a presentar juego");
	
	SDL_RenderPresent (renderer);
	us += t_ciclo.microsegundos (true);
	cuadros++;
	if (t_fps.milisegundos () > 5000) {
		int ms = t_fps.milisegundos (true);
		float fps = 1000.0 * (float)cuadros / (float)ms;
		cuadros = 0;
		std::cout << "fps: " << std::fixed << std::setprecision (2) << fps << "\n";
	}
	loginfo("Se termina de presentar juego");
	
}

bool juego::apretandoGodMode(const Uint8* state){
	return state[SDL_SCANCODE_F4];
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
bool juego::apretandoNivel2(const Uint8* state){
	return state[SDL_SCANCODE_Q];
}

//sube al jugador
bool juego::apretandoNivel3(const Uint8* state){
	return state[SDL_SCANCODE_W];
}

////////////////////////TECLAS MULTIPLAYER/////////////////////////////

//activa multijugador (2 players)
bool juego::apretandoMultijugador2P(const Uint8* state){
	return state[SDL_SCANCODE_N];
}
//activa multijugador (4 players)
bool juego::apretandoMultijugador4P(const Uint8* state){
	return state[SDL_SCANCODE_M];
}

bool juego::apretandoplayer2izquierda(const Uint8* state){
	return state[SDL_SCANCODE_A];
}
bool juego::apretandoplayer2salto(const Uint8* state){
	return state[SDL_SCANCODE_S];
}
bool juego::apretandoplayer2derecha(const Uint8* state){
	return state[SDL_SCANCODE_D];
}
bool juego::apretandoGrisP2(const Uint8* state){
	return state[SDL_SCANCODE_P];
}
bool juego::apretandoDesGrisP2(const Uint8* state){
	return state[SDL_SCANCODE_O];
}


////////////////////////FIN TECLAS MULTIPLAYER/////////////////////////

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

//Si alguno de los clientes aprieta la tecla W
bool juego::alguienQuiereGodMode(){
	return (cliente->quiereAccion(Constantes::nivel3) || cliente2->quiereAccion(Constantes::nivel3) || cliente3->quiereAccion(Constantes::nivel3) || cliente4->quiereAccion(Constantes::nivel3));
}

void juego::godModeParaTodos(){
	boby.activarGodMode();
	boby2.activarGodMode();
	boby3.activarGodMode();
	boby4.activarGodMode();
}

void juego::setAcciones(char* msj, int numeroCliente){
	if(numeroCliente == 1){
		cliente->setMensajeATraducir(msj);
	}
	else if (numeroCliente == 2){
		cliente2->setMensajeATraducir(msj);
	}
	else if (numeroCliente == 3){
		cliente3->setMensajeATraducir(msj);
	}
	else if (numeroCliente == 4){
		cliente4->setMensajeATraducir(msj);
	}
}

void juego::setCantidadDeBalas(){
	armador->setCantidadDeBalas(boby.getCantidadDeBalas(),1);
	armador->setCantidadDeBalas(boby2.getCantidadDeBalas(),2);
	armador->setCantidadDeBalas(boby3.getCantidadDeBalas(),3);
	armador->setCantidadDeBalas(boby4.getCantidadDeBalas(),4);
}

void juego::agregarBalaEnemigo(Bullet* nuevaBala){


	balasEnemigas.push_back(nuevaBala);
}

SDL_Renderer* juego::dameElRender(){
	return renderer;
}

juego::~juego ()
{
	SDL_DestroyTexture (textura_fondo3);
	SDL_DestroyTexture (textura_objetivo);
	SDL_DestroyTexture(textura_bala);
	SDL_DestroyTexture(textura_bala2);
	SDL_DestroyTexture(textura_bala3);
	SDL_DestroyTexture(textura_bala4);

	if(! enemigoNivel1->derrotado()){
		enemigoNivel1->~Enemigo();
	}

	//NOTA MARTIN //////////////////////////////
	unsigned size= vectorEnemigos.size();
	for(unsigned i = 0;i < size;i++){
		vectorEnemigos[i]->~Enemigo();
	}

	size= vectorItems.size();
	for(unsigned i = 0;i < size;i++){
		vectorItems[i]->~Item();
	}

	size= balasEnemigas.size();
	for(unsigned i = 0;i < size;i++){
		balasEnemigas[i]->~Bullet();
	}
	////////////////////////////////////////////

	cliente->~traductorDelCliente();
	armador->~ArmadorDeRespuesta();

	loginfo("Se destruyo juego");
	
}
