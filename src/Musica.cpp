/*
 * Musica.cpp
 *
 *  Created on: 8 nov. 2018
 *      Author: gisse
 */

#include "Musica.h"

// ---- CONSTRUCTOR ----
Musica::Musica() {
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	Mix_AllocateChannels(3);
	setVolumenMusica(100);
	volumenSonidos = 100;
	musicaActual = mSinMusica;
	tocandoMusica = false;
	// Carga de la musica
	vMusica.push_back(cargarMusica("intro"));
	vMusica.push_back(cargarMusica("musica1"));
	vMusica.push_back(cargarMusica("Aggressor")); // jefe final 1
	vMusica.push_back(cargarMusica("modoInmortal"));

	//--- Cargar chunks
	// armas
	vSonidos.push_back(cargarSonido("cg1"));
	vSonidos.push_back(cargarSonido("pistol"));
	vSonidos.push_back(cargarSonido("rifle"));
	vSonidos.push_back(cargarSonido("shotgun"));
	vSonidos.push_back(cargarSonido("cañon"));

	//perderVida/ morir
	vSonidos.push_back(cargarSonido("pierdeVidaJugador"));
	vSonidos.push_back(cargarSonido("muereJugador"));
	vSonidos.push_back(cargarSonido("muereEnemigo"));

	// acciones
	vSonidos.push_back(cargarSonido("salto"));
	vSonidos.push_back(cargarSonido("agarrarItem"));
}

// ------- GETTER SETTER -----
void Musica::setVolumenMusica(int iVolumen){
	volumenMusica = iVolumen;
}
int Musica::getVolumenMusica(){
	return volumenMusica;
}

void Musica::setVolumenChunk(int iVolumen){
	volumenSonidos = iVolumen;
}
int Musica::getVolumenChunk(){
	return volumenSonidos;
}
// ------- METODOS PUBLICOS -------
void Musica::tocarMusica(){
	if (musicaActual != mSinMusica){
		Mix_VolumeMusic(getVolumenMusica());
		if (Mix_PlayingMusic() == 0){
			Mix_PlayMusic(vMusica[musicaActual -1],-1);
		} else {
			pausarMusica();
		}
	}
}
void Musica::tocarMusica(eMusica iDMusica){
	if (iDMusica != mSinMusica){
		Mix_VolumeMusic(getVolumenMusica());
		if (Mix_PlayingMusic() == 0){
				Mix_PlayMusic(vMusica[iDMusica -1],-1);
				musicaActual = iDMusica;
			} else {
				pausarMusica();
			}
	}

}

void Musica::pararMusica(){
	Mix_HaltMusic();

}
void Musica::pausarMusica(){
	if (Mix_PausedMusic() == 1){
			Mix_ResumeMusic();
	} else {
			Mix_PauseMusic();
	}
}

// cambia musica secuencialmente
void Musica::cambiarMusica(){
	Musica::eMusica eNuevaCancion = musicaActual;
	pararMusica();
	// aca podria ser dependiendo del lvl
	switch (musicaActual){
	case mSinMusica:
		eNuevaCancion = mEsperandoJugadores;
		break;
	case mEsperandoJugadores:
		eNuevaCancion = mMusicaNivel1;
		break;
	case mMusicaNivel1:
		eNuevaCancion = mBossNivel1;
		break;
	default:
		eNuevaCancion = mSinMusica;
		break;
	}
	musicaActual = eNuevaCancion;
	tocarMusica();

}

void Musica::cambiarMusica(int nivel, bool jefeFinal){
	Musica::eMusica eNuevaCancion = musicaActual;
	pararMusica();
	// aca podria ser dependiendo del lvl
	switch (nivel){
	case 1:
		if(jefeFinal){
			eNuevaCancion = mBossNivel1;
		} else eNuevaCancion = mMusicaNivel1;
		break;
	case 2:
		if(jefeFinal){
			eNuevaCancion = mBossNivel2;
		} else eNuevaCancion = mMusicaNivel2;
		break;
	case 3:
		if(jefeFinal){
			eNuevaCancion = mBossNivel3;
		} else eNuevaCancion = mMusicaNivel3;
		break;
	default:
		eNuevaCancion = mSinMusica;
		break;
	}
	musicaActual = eNuevaCancion;
	tocarMusica();

}
// ------------- VOLUMEN MUSICA ---------------
void Musica::subirVolumenMusica(){
	volumenMusica += 10;
	if (volumenMusica<198){
		Mix_VolumeMusic(volumenMusica);
	} else {
		volumenMusica = 198;
	}
}
void Musica::bajarVolumenMusica(){
	volumenMusica -= 10;
	if (volumenMusica > -1){
		Mix_VolumeMusic(volumenMusica);
	} else {
		volumenMusica = 0;
	}
}
// -------------- Chunks / Sonidos ------------
void Musica::escucharSonido(eSonido iDSonido){
	Mix_VolumeChunk(vSonidos[iDSonido], volumenSonidos);
	Mix_PlayChannel(-1,vSonidos[iDSonido],0);
}

void Musica::escucharSonido(eSonido iDSonido, int channel){
	Mix_VolumeChunk(vSonidos[iDSonido], volumenSonidos);
	Mix_PlayChannel(channel,vSonidos[iDSonido],0);
}
void Musica::subirVolumenSonidos(){
	volumenSonidos+=10;
	if ( volumenSonidos > 198){
		volumenSonidos = 198;
	}
}
void Musica::bajarVolumenSonidos(){
	volumenSonidos-=10;
	if ( volumenSonidos < 0)
		volumenSonidos = 0;
}

void Musica::volumenSonido(eSonido idSonido, int vol){
	Mix_VolumeChunk(vSonidos[idSonido], vol);
}

// ------- METODOS PRIVADOS --------
Mix_Music* Musica::cargarMusica(string nombreMusica){
	string musicaPathWAV = RUTA_SONIDOS + nombreMusica + ".wav";

	Mix_Music* gMusic = Mix_LoadMUS(musicaPathWAV.c_str());
	if( gMusic == NULL ){
		string musicaPathMP3 = RUTA_SONIDOS + nombreMusica + ".mp3";
		gMusic = Mix_LoadMUS(musicaPathMP3.c_str());
		if( gMusic == NULL ){
		cout << "ERROR cargando cancion: "<< nombreMusica<< " SDL_mixer Error: "<<Mix_GetError() << endl;
		}
	}
	return gMusic;
}

Mix_Chunk* Musica::cargarSonido(string nombreSonido){
	string sonidoPath = RUTA_SONIDOS + nombreSonido + ".wav";
	Mix_Chunk* gSonido = Mix_LoadWAV(sonidoPath.c_str());
	if (gSonido == NULL){
		cout << "ERROR cargando sonido: "<< nombreSonido<< " SDL_mixer Error: "<<Mix_GetError() << endl;
	}
	return gSonido;
}
// ---- DESTRUCTOR ----
Musica::~Musica() {
	for (unsigned int i = 0; i<vMusica.size(); i++){
		Mix_FreeMusic(vMusica[i]);
	}
	vMusica.clear();
	musicaActual = mSinMusica;

	for (unsigned int i = 0; i < vSonidos.size() ; i++){
		Mix_FreeChunk(vSonidos[i]);
	}
	vSonidos.clear();
}

