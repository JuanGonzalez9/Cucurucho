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
	d1 (2),
	d2 (3),
	d3 (5),
	nivel (1),
	cascada(0),
	num_jugadores(1),
	rect_origen_fondo3 {0, 0, 800, 600},
	ventana (nullptr),
	renderer (nullptr),
	textura_objetivo (nullptr),
	traductor(nullptr)
{
	atexit (SDL_Quit);

	if (SDL_Init (SDL_INIT_VIDEO) < 0) {
		logerror("No pudo inicializarse SDL");
		std::cerr << "No pudo inicializarse SDL: " << SDL_GetError () << '\n';
		return;
	}
	ventana = SDL_CreateWindow ("Titulo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	if (nullptr == ventana) {
		logerror("No pudo crease la ventana");
		std::cerr << "No pudo crease la ventana: " << SDL_GetError () << '\n';
		return;
	}
	renderer = SDL_CreateRenderer (ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (nullptr == renderer) {
		logerror("No pudo crease el renderer");
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
	boby2.obtenerTextura("//configuracion//personajes//heroe2//sprite", renderer);
	boby2.setRectOrigen(0,0,480,480);
	boby3.obtenerTextura("//configuracion//personajes//heroe3//sprite", renderer);
	boby3.setRectOrigen(0,0,480,480);
	boby4.obtenerTextura("//configuracion//personajes//heroe4//sprite", renderer);
	boby4.setRectOrigen(0,0,480,480);

	//enemigoNivel1->crearTextura("imagenes/enemigo.png",renderer);
	enemigoNivel1 = new Enemigo(600,150,5);
	enemigoNivel1->obtenerTextura("//configuracion//personajes//enemigo1//sprite", renderer);

	enemigoNivel2 = new Enemigo(260,80,5);
	enemigoNivel2->obtenerTextura("//configuracion//personajes//enemigo2//sprite", renderer);

	enemigoNivel3 = new Enemigo(550,150,5);

	enemigoNivel3->obtenerTextura("//configuracion//personajes//enemigo3//sprite", renderer);

	loginfo("Se crearon los enemigos");

	// Creamos textura para pegar las plataformas
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
	traductor = new traductorDelCliente();

	//evita que se estire el fondo si al principio voy para atras
	fondo1.avanzarOrigen(50);
	fondo2.avanzarOrigen(50);

	//activo los jugadores al principio
	boby.activar();
	if(num_jugadores>=2)
		boby2.activar();
	if(num_jugadores>=3)
		boby3.activar();
	if(num_jugadores>=4)
		boby4.activar();

	//inicializo la posicion de los bobys en el primer nivel
	boby.setPosX(50);
	boby.setPosY(200);
	boby.setCoordenadaX(0+50);
	boby.setCoordenadaY(200);

	boby2.setPosX(150);
	boby2.setPosY(200);
	boby2.setCoordenadaX(0+150);
	boby2.setCoordenadaY(200);

	boby3.setPosX(250);
	boby3.setPosY(200);
	boby3.setCoordenadaX(0+250);
	boby3.setCoordenadaY(200);

	boby4.setPosX(350);
	boby4.setPosY(200);
	boby4.setCoordenadaX(0+350);
	boby4.setCoordenadaY(200);


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

	bobyPosX= nivel == 2 ? boby2.getPosX() : boby2.obtenerCoordenadaX();
	bobyPosY= nivel == 2 ? boby2.obtenerCoordenadaY() : boby2.getPosY();
	velocityBoby = boby2.obtenerVelocidadY();

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,34,72,nivel)){
		boby2.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,34,72,velocityBoby,nivel);

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

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,34,72,nivel)){
		boby3.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,34,72,velocityBoby,nivel);

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

	if(plataformas.hayColisionSuperior(bobyPosX,bobyPosY,34,72,nivel)){
		boby4.aterrizar();
	}
	else if( velocityBoby > 1){
		bobyPosY = plataformas.aproximarPosicionAPlataforma(bobyPosX,bobyPosY,34,72,velocityBoby,nivel);

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
	if((!boby.esGrisado()&&boby.esActivo()&&(boby.getPosX()<0))||(!boby2.esGrisado()&&boby2.esActivo()&&(boby2.getPosX()<0))||(!boby3.esGrisado()&&boby3.esActivo()&&(boby3.getPosX()<0))||(!boby4.esGrisado()&&boby4.esActivo()&&(boby4.getPosX()<0))||(nivel==2)){
		scrolleando=false;
	}

	//si boby es activo, esta apretando derecha, y sobrepasa el ancho, boby puede causar un scroll.
	//Nota: si esta grisado o no es irrelevante aca porque tecnicamente no se pueden apretar las teclas (esta desconectado).
	if((boby.esActivo())&&(apretandoDerecha(state))&&(boby.getPosX() > (ancho / 2)))
		bobyPuede=true;

	//NOTA: por ahora uso esta tecla para los otros 3 pero hay que cambiarla al input correspondiente
	if((boby2.esActivo())&&(apretandoplayer2derecha(state))&&(boby2.getPosX() > (ancho / 2)))
		boby2Puede=true;

	if((boby3.esActivo())&&(apretandoplayer2derecha(state))&&(boby3.getPosX() > (ancho / 2)))
		boby3Puede=true;

	if((boby4.esActivo())&&(apretandoplayer2derecha(state))&&(boby4.getPosX() > (ancho / 2)))
		boby4Puede=true;

	//SI NINGUNO de los 4 puede scrollear, no hay scrolleo.
	if((!bobyPuede)&&(!boby2Puede)&&(!boby3Puede)&&(!boby4Puede))
		scrolleando=false;

	//Ahora ya tengo la condicion.
	if(scrolleando){
		//scrollea el fondo horizontalmente.
		fondo1.avanzarOrigen(d1);
		fondo2.avanzarOrigen(d2);			
		rect_origen_fondo3.x += d3;
		if (rect_origen_fondo3.x > mundo_w-ancho) {
			rect_origen_fondo3.x = mundo_h-ancho;
		}
		//si un jugador tiene su tecla derecha apretada, hace que se mueve en su lugar hacia adelante
		//el resto retrocede respecto la pantalla (se mantiene su animacion) 
		if (apretandoDerecha(state)){
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
		if (apretandoplayer2derecha(state)){
			boby2.hacerComoQueCamina();
			boby2.subirCoordenadaXEn(d3);
		}else{
			if (boby2.esGrisado()){
				boby2.subirCoordenadaXEn(d3);
			}else {
				boby2.decrementarPosX(d3);
			}
		}
		//(para 3 y 4 cambiar el input que corresponde)
		if (apretandoplayer2derecha(state)){
			boby3.hacerComoQueCamina();
			boby3.subirCoordenadaXEn(d3);
		}else{
			if (boby3.esGrisado()){
				boby3.subirCoordenadaXEn(d3);
			}else {
				boby3.decrementarPosX(d3);
			}
		}
		if (apretandoplayer2derecha(state)){
			boby4.hacerComoQueCamina();
			boby4.subirCoordenadaXEn(d3);
		}else{
			if (boby4.esGrisado()){
				boby4.subirCoordenadaXEn(d3);
			}else {
				boby4.decrementarPosX(d3);
			}
		}

	}else{
		//esto es lo que pasa si el fondo no scrollea.
		if ((apretandoDerecha(state))&&(boby.getPosX() < 760)){
			//SOLO PARA PLAYER1 (mueve los fondos en nivel 1 y 3)
			if ((num_jugadores==1)&&(nivel!=2)){
				fondo1.avanzarOrigen(d1);
				fondo2.avanzarOrigen(d2);
			}
			boby.avanzar();
			boby.subirCoordenadaXEn(d3);
		}
		if ((apretandoplayer2derecha(state))&&(boby2.getPosX() < 760)){
			boby2.avanzar();
			boby2.subirCoordenadaXEn(d3);
		}
		if ((apretandoplayer2derecha(state))&&(boby3.getPosX() < 760)){
			boby3.avanzar();
			boby3.subirCoordenadaXEn(d3);
		}
		if ((apretandoplayer2derecha(state))&&(boby4.getPosX() < 760)){
			boby4.avanzar();
			boby4.subirCoordenadaXEn(d3);
		}
	}

	//ahora el movimiento hacia la izquierda
	if((apretandoIzquierda(state))&&(boby.getPosX() > 0 )){
		boby.retroceder();
		boby.subirCoordenadaXEn(-d3);
		//solo para player 1- scrolleo el fondo si hay un jugador		
		if((nivel != 2)&&(num_jugadores==1)){
			fondo1.avanzarOrigen(-d1);
			fondo2.avanzarOrigen(-d2);
		}
	}
	//muevo los otros jugadores (usar el input apropiado)
	if((apretandoplayer2izquierda(state))&&(boby2.getPosX() > 0 )){
		boby2.retroceder();
		boby2.subirCoordenadaXEn(-d3);		
	}
	if((apretandoplayer2izquierda(state))&&(boby3.getPosX() > 0 )){
		boby3.retroceder();
		boby3.subirCoordenadaXEn(-d3);		
	}
	if((apretandoplayer2izquierda(state))&&(boby4.getPosX() > 0 )){
		boby4.retroceder();
		boby4.subirCoordenadaXEn(-d3);		
	}	

///////////////////FIN DE SCROLL HORIZONTAL, ALELUYA///////////////////////////////////

	//probando traductor
	/*if(apretandoSalto(state) && apretandoAbajo(state) && !boby.estaSaltando()){
		boby.bajar();		
	}
	else{
		if(apretandoSalto(state))
				boby.saltar();
	}*/
	
	if(traductor->quiereSaltar()){
		boby.saltar();	
	}
	

	if(apretandoplayer2salto(state) && apretandoAbajo(state) && !boby2.estaSaltando()){
		boby2.bajar();		
	}
	else{
		if(apretandoplayer2salto(state))
				boby2.saltar();
	}
	if(apretandoplayer2salto(state) && apretandoAbajo(state) && !boby3.estaSaltando()){
		boby3.bajar();		
	}
	else{
		if(apretandoplayer2salto(state))
				boby3.saltar();
	}
	if(apretandoplayer2salto(state) && apretandoAbajo(state) && !boby4.estaSaltando()){
		boby4.bajar();		
	}
	else{
		if(apretandoplayer2salto(state))
				boby4.saltar();
	}



	if(apretandoAgacharse(state) && !(apretandoDerecha(state) || apretandoIzquierda(state))){
		boby.agacharse();
	}

	if(! apretandoAgacharse(state)){
		boby.pararse();
	}

	if(apretandoNivel2(state)){
		if(nivel == 1){
			cambioNivel=true;
		}		
	}

	if(apretandoNivel3(state)){
		if(nivel == 2){
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

			nuevaBala = new Bullet(boby.getPosX(),boby.getPosY(),posBala,direccionDeBala*10,boby.getEstado());

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

	loginfo("Se termina de manejar eventos de juego");
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
	if((boby.esActivo()&&(boby.getPosY()>580))||(boby2.esActivo()&&(boby2.getPosY()>580))||(boby3.esActivo()&&(boby3.getPosY()>580))||(boby4.esActivo()&&(boby4.getPosY()>580))||(nivel!=2)){
		scrolleando=false;
	}

	//si boby es activo, supera los 200px, se mueve para arriba y no toca el techo, boby puede causar un scroll.
	if((boby.esActivo())&&(boby.getPosY() <=(200))&&(boby.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby.obtenerVelocidadY() > 0)){
		bobyPuede=true;
		if(boby.obtenerVelocidadY()<maxVel)
			maxVel=boby.obtenerVelocidadY();
	}
	if((boby2.esActivo())&&(boby2.getPosY() <=(200))&&(boby2.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby2.obtenerVelocidadY() > 0)){
		boby2Puede=true;
		if(boby2.obtenerVelocidadY()<maxVel)
			maxVel=boby2.obtenerVelocidadY();
	}
	if((boby3.esActivo())&&(boby3.getPosY() <=(200))&&(boby3.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby3.obtenerVelocidadY() > 0)){
		boby3Puede=true;
		if(boby3.obtenerVelocidadY()<maxVel)
			maxVel=boby3.obtenerVelocidadY();
	}
	if((boby4.esActivo())&&(boby4.getPosY() <=(200))&&(boby4.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby4.obtenerVelocidadY() > 0)){
		boby4Puede=true;
		if(boby4.obtenerVelocidadY()<maxVel)
			maxVel=boby4.obtenerVelocidadY();
	}

	//SI NINGUNO de los 4 puede scrollear, no hay scrolleo.
	if((!bobyPuede)&&(!boby2Puede)&&(!boby3Puede)&&(!boby4Puede))
		scrolleando=false;

	//Ya determine si puedo scrollear, ¡Y! la velocidad maxima.

	if(scrolleando){
		//scrolleo verticalmente.
		boby.decrementarPosY(maxVel);
		boby2.decrementarPosY(maxVel);
		boby3.decrementarPosY(maxVel);
		boby4.decrementarPosY(maxVel);

		fondo1.avanzarOrigenY(maxVel/3);
		fondo2.avanzarOrigenY(maxVel/2);
		rect_origen_fondo3.y += maxVel;
	}
	else{
		//si no se puede scrollear, evito que los jugadores que pueden moverse por arriba del borde se muevan.
		if((boby.esActivo())&&(boby.getPosY()<=(0))&&(boby.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby.obtenerVelocidadY() > 0)){
			boby.decrementarPosY(boby.obtenerVelocidadY());
			boby.subirCoordenadaYEn(-boby.obtenerVelocidadY());
			}
		if((boby2.esActivo())&&(boby2.getPosY()<=(0))&&(boby2.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby2.obtenerVelocidadY() > 0)){
			boby2.decrementarPosY(boby2.obtenerVelocidadY());
			boby2.subirCoordenadaYEn(-boby2.obtenerVelocidadY());
			}
		if((boby3.esActivo())&&(boby3.getPosY()<=(0))&&(boby3.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby3.obtenerVelocidadY() > 0)){
			boby3.decrementarPosY(boby3.obtenerVelocidadY());
			boby3.subirCoordenadaYEn(-boby3.obtenerVelocidadY());
			}
		if((boby4.esActivo())&&(boby4.getPosY()<=(0))&&(boby4.obtenerVelocidadY()<0)&&(rect_origen_fondo3.y + boby4.obtenerVelocidadY() > 0)){
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
			fondo1.avanzarOrigenY(34);
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

	if((nivel==1)&& (boby.llegoAlFinalDelNivel1() || cambioNivel==true))
	{	
		cambioNivel=false;
		nivel=2;
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

	if((nivel==2)&& (boby.llegoAlFinalDelNivel2() || cambioNivel== true))
	{	
		
		cambioNivel=false;
		nivel=3;		
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

	if((nivel ==3) && boby.llegoAlFinalDelNivel3()){
		termino = true;
	}

	//Actualizo balas
	for(int i = 0; i < bullets.size(); i++){
		bullets[i]->move();
	}

	//borro las balas que exceden su rango para que no sigan hasta el infinito
	for(unsigned i = 0; i < bullets.size(); i++){
		if(bullets[i]->getDuracion() == 0){
			bullets.erase(bullets.begin() + i);
		}
	}

	//veo si el jugador toca al enemigo
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		if(! enemigoNivel1->derrotado() && collision(boby.getRectaDestino(),enemigoNivel1->getRectaDestino())){
			if(boby.getInvincibilityFrames() == 0)
				boby.perderVida();
		}
	}

	//veo si el jugador se cayo
	if(boby.getPosY()>600){
		if (nivel ==2){
			boby.setCoordenadaY(boby.obtenerCoordenadaY()+40 - boby.getPosY());
		}
		boby.setPosY(40);		
		boby.perderVida();
	}
	if(boby2.getPosY()>600){
		if (nivel ==2){
			boby2.setCoordenadaY(boby2.obtenerCoordenadaY()+40 - boby2.getPosY());
		}
		boby2.setPosY(40);		
		boby2.perderVida();
	}
	if(boby3.getPosY()>600){
		if (nivel ==2){
			boby3.setCoordenadaY(boby3.obtenerCoordenadaY()+40 - boby3.getPosY());
		}
		boby3.setPosY(40);		
		boby3.perderVida();
	}
	if(boby4.getPosY()>600){
		if (nivel ==2){
			boby4.setCoordenadaY(boby4.obtenerCoordenadaY()+40 - boby4.getPosY());
		}
		boby4.setPosY(40);		
		boby4.perderVida();
	}

	//refresco el tiempo de invincibilidad
	boby.refreshIFrames();
	boby2.refreshIFrames();
	boby3.refreshIFrames();
	boby4.refreshIFrames();

	//actualiza el shootTimer del jugador (para que no tire 500 tiros por segundo)
	boby.refreshBullets();

	//veo si las balas le pegan al enemigo
	if(nivel == 1 && boby.estaCercaDelFinalDelNivel1()){
		for(unsigned i = 0;i < bullets.size();i++){
			if(! enemigoNivel1->derrotado() && collision(bullets[i]->getRectaDestino(),enemigoNivel1->getRectaDestino())){
				enemigoNivel1->perderVida();
				bullets.erase(bullets.begin() + i);
			}

		}
	}

	loginfo("Se termina de actualizar juego");
	
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
	}

	// Copio a los jugadores (LO HAGO EN ORDEN INVERSO ASI SI SE SUPERPONEN APARECEN P1 arriba del 2, etc.)
	if((num_jugadores>=4)&&((boby4.getInvincibilityFrames()/2) %2 == 0))
		boby4.dibujar(renderer);
	if((num_jugadores>=3)&&((boby3.getInvincibilityFrames()/2) %2 == 0))
		boby3.dibujar(renderer);
	if((num_jugadores>=2)&&((boby2.getInvincibilityFrames()/2) %2 == 0))
		boby2.dibujar(renderer);
	if((boby.getInvincibilityFrames()/2) %2 == 0)
		boby.dibujar(renderer);


	for(unsigned i = 0; i < bullets.size();i++){
		bullets[i]->dibujar(renderer);
	}
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

void juego::setAcciones(char* msj){
	traductor->setMensajeATraducir(msj);
}

juego::~juego ()
{
	SDL_DestroyTexture (textura_fondo3);
	SDL_DestroyTexture (textura_objetivo);
	SDL_DestroyRenderer (renderer);
	SDL_DestroyWindow (ventana);
	SDL_DestroyTexture(textura_bala);
	if(! enemigoNivel1->derrotado()){
		enemigoNivel1->~Enemigo();
	}

	for(unsigned i = 0;i < bullets.size();i++){
		bullets[i]->~Bullet();
	}

	traductor->~traductorDelCliente();

	loginfo("Se destruyo juego");
	
}
