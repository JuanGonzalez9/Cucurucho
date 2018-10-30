/*
 * Socket.h
 *
 *  Created on: Oct 10, 2018
 *      Author: juan
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stddef.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class Socket {

public:
	Socket();
	Socket(int accept);
	int bindAndListen(unsigned short port);
	int conectar(const char* host_name, unsigned short port);
	int aceptar();
	int enviar(int idSock,const char* buffer, int length);
	int recibir(int idSock,char* buffer, int length);
	virtual ~Socket();

	//----SET y GET--------------------
	int getSocketId();
	int getAcceptedSocket();
	void setSocketId(int valor);
	void setConexion(bool conectado);
	bool estaConectado();

private:
	int idSocket;
	int accepted_socket;
	int cantMaximaDeJugadores;
	bool conectado;
};

#endif /* SOCKET_H_ */
