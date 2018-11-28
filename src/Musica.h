/*
 * Musica.h
 *
 *  Created on: 8 nov. 2018
 *      Author: Dalma
 */

#ifndef MUSICA_H_
#define MUSICA_H_

// constantes
#define RUTA_SONIDOS "sonidos/"

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Musica {
public:
	Musica();
	virtual ~Musica();

	enum eMusica {
		mSinMusica,
		mEsperandoJugadores,
		mMusicaNivel1,
		mBossNivel1,
		mMusicaNivel2,
		mBossNivel2,
		mMusicaNivel3,
		mBossNivel3,
		mDesconectado,
		mModoInmortal,
	};
	eMusica musicaActual;

	enum eSonido {
		cAmetralladora,
		cPistol,
		cBazooka,
		cShotgun,
		cCanion,
		cPierdeVidaJugador,
		cMuereJugador,
		cMuereEnemigo,
		cAccionSalto,
		cAgarrarItem,
		cModoInmortal
	};
	eSonido chunkActual;

	//getter setter
	void setVolumenMusica(int iVolumen);
	int getVolumenMusica();
	void setVolumenChunk(int iVolumen);
	int getVolumenChunk();

	//Metodos
	// toca la musica actual, pausa si ya estaba sonando
	void tocarMusica();
	// toca musica especifica
	void tocarMusica(eMusica idMusica);
	// Detiene musica actual
	void pararMusica();
	void pausarMusica();
	void cambiarMusica();
	void cambiarMusica(int nivel, bool jefeFinal);

	void escucharSonido(eSonido idSonido);
	void escucharSonido(eSonido idSonido,int channel);

	// Control de volumen
	void subirVolumenMusica();
	void bajarVolumenMusica();
	void subirVolumenSonidos();
	void bajarVolumenSonidos();
	void volumenSonido(eSonido idSonido, int vol);

private:
	vector<Mix_Music*> vMusica;
	vector<Mix_Chunk*> vSonidos;
	vector<Mix_Chunk*> vGritos; // puede ser q no lo necesite

	int volumenMusica;
	int volumenSonidos;
	bool tocandoMusica;

	// -------METODOS PRIVADOS----------
	Mix_Music* cargarMusica(string nombreMusica);
	Mix_Chunk* cargarSonido(string nombreSonido);
};

#endif /* MUSICA_H_ */
