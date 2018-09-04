#include <fstream>
#include <iostream> //Podemos utilizar la consola
#include <stdlib.h>
#include <ctime>
#include "registro.hpp"

using namespace std; // se usa esta linea para no anteponer en cada cout el std



void LogEventos::registrar(LogEventos::TipoLog tipoEvento,const char* evento_ocurrido){
	if(tipoLog >= tipoEvento){

		ofstream archivo("registro.txt",ios::app);
		string hora, nombre_evento;
		if(!archivo.is_open()){
				archivo.open("registro.txt",ios::app); //app de append

			}
		char *hora_actual;
    	time_t tAct = time(NULL);
    	hora_actual=asctime(localtime(&tAct));	

    	archivo << "Fecha&Hora: " << hora_actual;
		archivo << "Nombre evento: " <<  evento_ocurrido << endl;

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

