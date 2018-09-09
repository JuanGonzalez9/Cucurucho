#include <fstream>
#include <iostream> //Podemos utilizar la consola
#include <stdlib.h>
#include <ctime>

using namespace std; // se usa esta linea para no anteponer en cada cout el std

class LogEventos {
public:
	const char* tipoLog;
	void registrar(const char* tipoEvento, const char* evento_ocurrido);
	void borrarEventos();
	void definirTipoLog(const char* tipo);
	void mostrarEventos();
};

void LogEventos::registrar(const char* tipoEvento,const char* evento_ocurrido){
	if(tipoLog == tipoEvento){

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
	return;
}

void LogEventos::borrarEventos(){
	ofstream archivo("registro.txt",ios::trunc); 
	return;
}

void LogEventos::definirTipoLog(const char* tipo){
	tipoLog = tipo;
	return;
}

void mostrarEventos(){
	system("cat registro.txt");
}


int main(){
	LogEventos registro;
	registro.definirTipoLog("error");
	
	registro.registrar("error", "sucedio que tal cosa...");
	mostrarEventos();

	registro.borrarEventos();
	
	mostrarEventos();


	return 0;
}

