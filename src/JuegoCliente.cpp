#include "JuegoCliente.h"
#include "alien.hpp"

extern "C"
{
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_keycode.h>
}

JuegoCliente::JuegoCliente(ventana &v, int cantidadJugadores,int numeroDeJugador, puntajes &pts):
	juego (v, cantidadJugadores, pts),
	etiqueta_puntos (5, 2, 41)
{
	this->numeroDeJugador = numeroDeJugador;
	nivel = 1;
	cantJugadores = cantidadJugadores;
	bala_rectOrigen = {0,0,32,32};
	bala_rectDestino = {0,0,8,8};

	//Texturas Enemigos
	Enemigo* nuevoEnemigo;
	nuevoEnemigo = new Marcianito(750,-340,1,2700,2);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//marciano//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Ovni(800,80,1,1920,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//ovni//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new MonstruoFinalNivel1(this,800,80,5);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//enemigo1//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new MonstruoFinalNivel2(this,800,80,7);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//enemigo2//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new MonstruoFinalNivel3(this,800,80,10);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//enemigo3//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Turret(0,0,1,2000,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//turret//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new Paracaidas(600,-86,1,2000,1);
	nuevoEnemigo->obtenerTextura("//configuracion//personajes//parachute//sprite", renderer);
	vEnemigos.push_back(nuevoEnemigo);

	nuevoEnemigo = new alien(this, 400, 245, 1, 0, 1);
	//((alien*)nuevoEnemigo)->limitar(300, 500);
	vEnemigos.push_back(nuevoEnemigo);

	//Texturas Items
	Item* nuevoItem = new Item(0,0,1,1920,1,1);
	nuevoItem->obtenerTextura("//configuracion//items//vidaitem//sprite", renderer);
	vItems.push_back(nuevoItem);

	nuevoItem = new Item(0,0,1,1920,2,1);
	nuevoItem->obtenerTextura("//configuracion//items//ametralladora//sprite", renderer);
	vItems.push_back(nuevoItem);

	nuevoItem = new Item(0,0,1,1920,3,1);
	nuevoItem->obtenerTextura("//configuracion//items//escopeta//sprite", renderer);
	vItems.push_back(nuevoItem);

	nuevoItem = new Item(0,0,1,1920,4,1);
	nuevoItem->obtenerTextura("//configuracion//items//bazooka//sprite", renderer);
	vItems.push_back(nuevoItem);

	//Texturas Bala
	Bullet* nuevaBala = new Bullet(0,0,0,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala2//sprite", renderer);
	vBalaEnemiga.push_back(nuevaBala);

	nuevaBala = new Bullet(0,0,0,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego//sprite", renderer);
	nuevaBala->actualizarRectOrigenOriginal();
	vBalaEnemiga.push_back(nuevaBala);

	nuevaBala = new Bullet(0,0,0,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_fuego_2//sprite", renderer);
	nuevaBala->actualizarRectOrigenOriginal();
	vBalaEnemiga.push_back(nuevaBala);

	nuevaBala = new Bullet(0,0,0,0,2,100);
	nuevaBala->obtenerTextura("//configuracion//items//bala_misil//sprite", renderer);
	nuevaBala->actualizarRectOrigenOriginal();
	vBalaEnemiga.push_back(nuevaBala);
	
	SDL_Color color = {55, 149, 242, 255};
	SDL_Color color_borde = {255, 255, 255, 255};
	etiqueta_puntos.anclado_x = control::medio;
	etiqueta_puntos.texto ("00000");
	etiqueta_puntos.color (color);
	etiqueta_puntos.borde (2, color_borde);
	agregar (&etiqueta_puntos, false);

	// musica
	laMusica = new Musica();
}

void JuegoCliente::establecerPuntaje(int puntaje)
{
	std::string s = std::to_string(puntaje);
	if (s.length() < 5) {
		s.insert(0, 5-s.length(), '0');
	}
	etiqueta_puntos.texto (s.c_str());
}

int JuegoCliente::jugador() const
{
	return numeroDeJugador;
}

void JuegoCliente::setMensajeDelServidor(string msj){
	p.parsear(msj,cantJugadores,numeroDeJugador);
	if(nivel == 1 && p.getNivel() == 3){
		actualizarNivel2();
		nivel = 3;
	}
	else{
		if(nivel == 1 && p.getNivel() == 2){
			actualizarNivel1();
			nivel = 2;
		}
		else if(nivel == 2 && p.getNivel() == 3){
			actualizarNivel2();
			nivel = 3;	
		}
	}
}

void JuegoCliente::actualizarFondos(){
	int nivel = p.getNivel();

	if(nivel == 1 || nivel == 3){
		fondo1.setRectOrigenX(p.getPosFondo1());
		fondo2.setRectOrigenX(p.getPosFondo2());
		rect_origen_fondo3.x = p.getPosFondo3();
	}
	else{
		fondo1.setRectOrigenY(p.getPosFondo1());
		fondo2.setRectOrigenY(p.getPosFondo2());
		rect_origen_fondo3.y = p.getPosFondo3();	
	}
}

Personaje* JuegoCliente::dameAlBobyNumero(int numeroDeBoby){
	Personaje* unBoby;
	switch(numeroDeBoby){
		case (1):
			unBoby = &boby;
			break;
		case (2):
			unBoby = &boby2;
			break;
		case (3):
			unBoby = &boby3;
			break;
		case (4):
			unBoby = &boby4;
			break;
		default:
			break;
	}

	return unBoby;
}

void JuegoCliente::actualizarPosicionDeMiPersonaje(int numeroPersonaje){
	Personaje* unBoby = dameAlBobyNumero(numeroPersonaje);

	unBoby->setPosX(p.getPosPersonajeX(numeroPersonaje));
	unBoby->setPosY(p.getPosPersonajeY(numeroPersonaje));
}

void JuegoCliente::actualizarEstadoDeMiPersonaje(int numeroP){
	Personaje* unBoby = dameAlBobyNumero(numeroP);

	unBoby->setEstados(p.estaSaltando(numeroP),p.estaDisparando(numeroP),p.estaMirandoALaDerecha(numeroP),p.estaActivo(numeroP),p.estaGrisado(numeroP));
	unBoby->setEstadosEnumerados(p.getEstado(numeroP),p.getDireccionDisparo(numeroP));
}

void JuegoCliente::ajustarTamanioBala(int tipoDeBala){
	if(tipoDeBala == 3){
		bala_rectDestino.h = 32;
		bala_rectDestino.w = 32;
	}
	else{
		bala_rectDestino.h = 8;
		bala_rectDestino.w = 8;
	}
}

void JuegoCliente::dibujarBalas(vector< pair< int,pair<int,int> > > balas){
	int balasDibujadas = 0;
	vector<int> cantBalas = p.getCantBalas();
	for(unsigned i = 0; i < cantBalas.size();i++){
		int balasDeEsteJugador = cantBalas[i];
		if(p.getVidaPersonaje(i+1) > 0){
			SDL_Texture* texturaActual;
	
			switch(i){
				case (0):
					texturaActual = textura_bala;
					break;
				case (1):
					texturaActual = textura_bala2;
					break;
				case (2):
					texturaActual = textura_bala3;
					break;
				case (3):
					texturaActual = textura_bala4;
					break;
			}

			for(int j = 0; j < balasDeEsteJugador;j++){
				int tipoDeBala = balas[balasDibujadas].first;
				ajustarTamanioBala(tipoDeBala);
				bala_rectDestino.x = balas[balasDibujadas].second.first;
				bala_rectDestino.y = balas[balasDibujadas].second.second;
				SDL_RenderCopy(renderer, texturaActual, & bala_rectOrigen , & bala_rectDestino);
				balasDibujadas++;
			}
		}
		else balasDibujadas += balasDeEsteJugador;
	}
}

void JuegoCliente::manejarEventos (){
	SDL_Event e;
	while (SDL_PollEvent (&e) != 0) { switch (e.type){
		case SDL_QUIT:
			termino = true;
			break;
		default:
			break;
	} }
}

void JuegoCliente::dibujarEnemigoFinal(){
	switch(nivel){
		case (1):
			enemigoNivel1->dibujar(renderer);
			break;
		case (2):
			enemigoNivel2->dibujar(renderer);
			break;
		case (3):
			enemigoNivel3->dibujar(renderer);
			break;
		default:
			break;
	}
}

void JuegoCliente::dibujarEnemigos(){
	vDatosEnemigo.clear();
	vDatosEnemigo = p.getEnemigos();

	for(unsigned i = 0; i < vDatosEnemigo.size();i++){
		Constantes::TipoEnemigo tipo = vDatosEnemigo[i]->getTipoEnemigo();
		int x = vDatosEnemigo[i]->getPosX();
		int y = vDatosEnemigo[i]->getPosY();
		int marcaPasos = vDatosEnemigo[i]->getMarcaPasos();
		//cout<<"(x,y) "<<x<<" , "<<y<<endl;
		if(tipo == Constantes::turret || tipo == Constantes::marcianito){
			vEnemigos[tipo-1]->setMarcaPasos(marcaPasos);
		}
		if(tipo == Constantes::alien){
			((alien*)vEnemigos[tipo-1])->actualizar();
		}
		vEnemigos[tipo-1]->setPos(x,y);
		vEnemigos[tipo-1]->dibujar(renderer);
	}
}

void JuegoCliente::dibujarItems(){
	vDatosItem.clear();
	vDatosItem = p.getItems();

	for(unsigned i = 0; i < vDatosItem.size();i++){
		Constantes::TipoItem tipo = vDatosItem[i]->getTipoItem();
		int x = vDatosItem[i]->getPosX();
		int y = vDatosItem[i]->getPosY();
		switch(tipo){
			case (Constantes::vidaExtra):
				vItems[0]->setPos(x,y);
				vItems[0]->dibujar(renderer);
				break;
			case (Constantes::ametralladora):
				vItems[1]->setPos(x,y);
				vItems[1]->dibujar(renderer);
				break;
			case (Constantes::escopeta):
				vItems[2]->setPos(x,y);
				vItems[2]->dibujar(renderer);
				break;
			case (Constantes::bazooka):
				vItems[3]->setPos(x,y);
				vItems[3]->dibujar(renderer);
				break;
			default:
				cout<<"No conozco ese tipo de item"<<endl;
				break;
		}
	}	
}

void JuegoCliente::dibujarBalasEnemigas(){
	vDatosBalaEnemiga.clear();
	vDatosBalaEnemiga = p.getBalasEnemigas();

	for(unsigned i = 0; i < vDatosBalaEnemiga.size();i++){
		Constantes::TipoArma tipo = vDatosBalaEnemiga[i]->getTipoArma();
		int x = vDatosBalaEnemiga[i]->getPosX();
		int y = vDatosBalaEnemiga[i]->getPosY();
		//cout<<"(x,y) = "<<x<<" "<<y<<" tipo = "<<tipo<<endl;
		vBalaEnemiga[tipo - 1]->setPos(x,y);
		vBalaEnemiga[tipo - 1]->dibujar(renderer);
		/*switch(tipo){
			case (Constantes::normal):
				vBalaEnemiga[0]->setPos(x,y);
				vBalaEnemiga[0]->dibujar(renderer);
				break;
			default:
				cout<<"No conozco ese tipo de arma"<<endl;
				break;
		}*/
	}
}


void JuegoCliente::dibujarJugadores(){
	p.refreshIframes();
	Personaje* unBoby;
	for(int i = 0; i < num_jugadores; i++){
		unBoby = dameAlBobyNumero(i + 1);
		if(i != numeroDeJugador && unBoby->esActivo() && p.getVidaPersonaje(i + 1) > 0){
			actualizarPosicionDeMiPersonaje(i+1);
			actualizarEstadoDeMiPersonaje(i+1);
			if((p.getIframes(i+1)/2) %2 ==0)
				unBoby->dibujar(renderer);
		}
	}
	
	unBoby = dameAlBobyNumero(numeroDeJugador + 1);
	if(p.getVidaPersonaje(numeroDeJugador + 1) > 0){
		actualizarPosicionDeMiPersonaje(numeroDeJugador + 1);
		actualizarEstadoDeMiPersonaje(numeroDeJugador + 1);
		if((p.getIframes(numeroDeJugador+1)/2) %2 == 0)		
			unBoby->dibujar(renderer);
	}
}

void JuegoCliente::dibujameLasVidas(){

	boby.setVidas(p.getVidaPersonaje(1));
	boby2.setVidas(p.getVidaPersonaje(2));
	boby3.setVidas(p.getVidaPersonaje(3));
	boby4.setVidas(p.getVidaPersonaje(4));

	dibujarVidas();
}

void JuegoCliente::dibujar(){
	SDL_SetTextureBlendMode (textura_objetivo, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget (renderer, textura_objetivo);

	actualizarFondos();

	fondo1.dibujarFondo(renderer);
	fondo2.dibujarFondo(renderer);
	SDL_RenderCopy (renderer, textura_fondo3, &rect_origen_fondo3, nullptr);

	SDL_SetRenderTarget (renderer, nullptr);
	SDL_RenderCopy (renderer, textura_objetivo, nullptr, nullptr);

	dibujarEnemigos();
	dibujarItems();
	dibujarJugadores();
	dibujarBalasEnemigas();

	vector< pair<int,pair<int,int>> > balas = p.getBalas();
	dibujarBalas(balas);

	dibujameLasVidas();

	//if(p.estaElEnemigo()) dibujarEnemigoFinal();
	
	establecerPuntaje(p.getPuntaje());
	contenedor_principal::dibujar ();
}

void JuegoCliente::reproducirSonidos(){
	string sonidos = p.getChunks();

	for(int i = 0;i < sonidos.length();i++){
		Constantes::TipoChunk aReproducir = (Constantes::TipoChunk) (sonidos[i] - '0');
		
		switch(aReproducir){
			case (Constantes::sinChunk):
				break;
			case (Constantes::chunkSalto):
				laMusica->escucharSonido(Musica::cAccionSalto,1);
				break;
			case (Constantes::disparaPistola):
				laMusica->escucharSonido(Musica::cPistol,2);
				break;
			case (Constantes::disparaAmetralladora):
				laMusica->escucharSonido(Musica::cAmetralladora,2);
				break;
			case (Constantes::disparaBazooka):
				laMusica->escucharSonido(Musica::cBazooka,2);
				break;
			case (Constantes::disparaEscopeta):
				laMusica->escucharSonido(Musica::cShotgun,2);
				break;
			case (Constantes::modoInmortal):
				laMusica->escucharSonido(Musica::cModoInmortal,0);
				break;
			default:
				cout<<"sonido desconocido"<<endl;
				break;
		
		
		}

		if(p.alguienPerdioVida()) laMusica->escucharSonido(Musica::cPierdeVidaJugador,3);
		if(p.alguienMurio()) laMusica->escucharSonido(Musica::cMuereJugador,3);
	}
}

JuegoCliente::~JuegoCliente() {
}


