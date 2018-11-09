/*
 * Socket.cpp
 *
 *  Created on: Oct 10, 2018
 *      Author: juan
 */

#include "Socket.h"
using namespace std;

Socket::Socket() {
	conectado = false;
	cantMaximaDeJugadores = 4;
	idSocket = socket(PF_INET,SOCK_STREAM,0);

	if(idSocket == -1){
		cout<<"No se pudo crear el socket"<<endl;
		return;
	}
	else{
		conectado = true;	
	}
}

Socket::Socket(int accept){
	cantMaximaDeJugadores = 4;
	// ATENCION: ahora asigno accept a accepted_socket directamente.
	accepted_socket = idSocket = accept;

	if(idSocket < 0){
		cout<<"Error en el accept"<<endl;
		return;
	}
}

int Socket::bindAndListen(unsigned short port){
	struct sockaddr_in adress;
	adress.sin_family = AF_INET;
	adress.sin_port = htons(port);
	adress.sin_addr.s_addr = INADDR_ANY;
	memset(adress.sin_zero,0,sizeof(adress.sin_zero));

	int error = bind(idSocket, (struct sockaddr*) &adress,sizeof(struct sockaddr_in));
	if(error < 0){
		//despues va en el log
		cout<<"error en el bind"<<endl;
		return -1;
	}
	//permito que entren hasta 4 conexiones a la cola
	if( listen(idSocket,cantMaximaDeJugadores) < 0){
		cout<<"error en el listen"<<endl;
		return -1;
	}
	return 0;
}

int Socket::conectar(const char* host_name, unsigned short port){
	//en host_name va el server adress ej: 127.0.0.1

	struct sockaddr_in adress;
	adress.sin_family = AF_INET;
	adress.sin_port = htons(port);
	adress.sin_addr.s_addr = inet_addr(host_name);
	memset(adress.sin_zero,0,sizeof(adress.sin_zero));

	int is_connected = connect(idSocket,(struct sockaddr*) &adress,sizeof(struct sockaddr_in));
	if(is_connected < 0){
		cout<<"error en la conexion"<<endl;
		return -1;
	}
	return 0;
}

int Socket::aceptar(){
	Socket* accepted_socket = new Socket(accept(idSocket,0,0));
	this->accepted_socket = accepted_socket->getSocketId();
	if(accepted_socket->getSocketId() == -1){
		cout<<"error en el accept"<<endl;
		return -1;
	}
	return 0;
}

int Socket::enviar(int idSock,const char* buffer, int length){
	//guardo la cantidad de bytes que ya envie
	int sent = 0;
	//std::cout << "Enviar: " << length << "\n";
	while(sent < length && sent != -1){
		sent += send(idSock,buffer + sent,length - sent,MSG_NOSIGNAL);
		//std::cout << "Enviados: " << sent << "\n";
	}
	//std::cout << "Enviados: total: " << sent << "\n";
	return sent;
	#if 0
	int escritos = 0, r;
	std::cout << "Enviar: " << length << " en " << idSock << "\n";
	while (escritos < length && (r = send (idSock, buffer+escritos, length-escritos, 0)) != -1) {
		escritos += r;
		std::cout << "Enviados: " << escritos << "\n";
	}
	std::cout << "Enviados: total: " << escritos << "\n";
	return escritos;
	#endif
}

int Socket::recibir(int idSock,char* buffer, int length){
	int recieved = 0;
	int cantDeCeros = 0;
	buffer[0] = '\0';
	//std::cout << "Recibir: " << length << "\n";
	while(recieved < length && recieved != -1){
		recieved += recv(idSock,buffer + recieved,length - recieved,0);
		//std::cout << "Recibidos: " << recieved << "\n";
		#if 0
		if(recieved == 0){
			std::cout << "recv cero\n";
			cantDeCeros++;
			if(cantDeCeros == 10) recieved = -1;
		}
		#endif
	}
	//std::cout << "Recibidos: total: " << recieved << "\n";
	return recieved;
	#if 0
	int leidos = 0, r;
	std::cout << "Recibir: " << length << " en " << idSock << "\n";
	while (leidos < length && (r = recv (idSock, buffer+leidos, length-leidos, 0)) > 0) {
		leidos += r;
		std::cout << "Recibidos: " << leidos << "\n";
	}
	std::cout << "Recibidos: total: " << leidos << "\n";
	#endif
}

//--------SET y GET--------------------------

int Socket::getSocketId(){
	return idSocket;
}

int Socket::getAcceptedSocket(){
	return accepted_socket;
}

void Socket::setSocketId(int valor){
	//idSocket = valor;
	// ATENCION: ahora asigno accept a accepted_socket directamente.
	accepted_socket = idSocket = valor;
}

void Socket::setConexion(bool conectado){
	this->conectado = conectado;
}

bool Socket::estaConectado(){
	return conectado;
}

Socket::~Socket() {
	shutdown(idSocket,0);
	close(idSocket);
}

