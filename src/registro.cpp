#include <fstream>
#include <iostream> //Podemos utilizar la consola
#include <stdlib.h>
#include <ctime>
#include "registro.hpp"

using namespace std; // se usa esta linea para no anteponer en cada cout el std

#include <time.h>
struct tm* gmtime(const time_t* tPtr);

void LogEventos::registrar(LogEventos::TipoLog tipoEvento,const char* evento_ocurrido){
	std::lock_guard<std::mutex> lock(mutex);
	if(tipoLog >= tipoEvento){

		ofstream archivo("registro.txt",ios::app);
		string hora, nombre_evento;
		if(!archivo.is_open()){
				archivo.open("registro.txt",ios::app); //app de append

			}
		/*char *hora_actual;
    	time_t tAct = time(NULL);
    	hora_actual=asctime(localtime(&tAct));

		cout << "dd-mm-aaaa: " << pt1->tm_mday << "-" << pt1->tm_mon+1 << "-"
                          << pt1->tm_year+1900 << endl;

		*/

    	time_t tSac = time(NULL);
    	struct tm* pt1 = localtime(&tSac);
    	char * evento;

    	if(tipoEvento == 0){
    		evento = "ERROR";
    	}
    	else if(tipoEvento == 1){
    		evento = "INFO";
    	}
    	else if(tipoEvento==2){
    		evento = "DEBUG";
    	}
    	

    	archivo << pt1->tm_hour << ":" << pt1->tm_min << ":" << pt1->tm_sec << ": " << evento <<": " << evento_ocurrido << endl;
    

		archivo.close();


	}
	
}

void LogEventos::borrarEventos(){
	ofstream archivo("registro.txt",ios::trunc); 
	
}

void LogEventos::definirTipoLog(LogEventos::TipoLog tipo){
	tipoLog = tipo;
	
}

void LogEventos::mostrarEventos(){

	if (system("cat registro.txt") == -1){
		registrar(error,"No se pudo mostrar el registro en pantalla");
	}
}

void LogEventos::registrarNuevoJuego(){
	time_t tSac = time(NULL);
    struct tm* pt1 = localtime(&tSac);


    ofstream archivo("registro.txt",ios::app);
	
	if(!archivo.is_open()){
			archivo.open("registro.txt",ios::app); //app de append

	}

	archivo << "                         "<< endl;
    archivo << pt1->tm_hour << ":" << pt1->tm_min << ":" << pt1->tm_sec << ": " << " SE HA INICIADO UN NUEVO JUEGO " << endl;
    archivo << "                         "<< endl;


    archivo.close();
}

void LogEventos::registrarFinJuego(){
	time_t tSac = time(NULL);
    struct tm* pt1 = localtime(&tSac);


    ofstream archivo("registro.txt",ios::app);
	
	if(!archivo.is_open()){
			archivo.open("registro.txt",ios::app); //app de append

	}

	archivo << "                         "<< endl;
    archivo << pt1->tm_hour << ":" << pt1->tm_min << ":" << pt1->tm_sec << ": " << " SE HA CERRADO EL SERVIDOR" << endl;
    archivo << "                         "<< endl;


    archivo.close();
}