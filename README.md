# Cucurucho
Taller de Programacion 2-2018

Para configurar el Eclipse :

Agregar una variable de enterno asi:
Run -> Run Configurations... -> Enviroment -> New Enviroment Variable. 
	Nombre de la variable: LD_LIBRARY_PATH
	Value: /usr/local/lib

Agregar librerias asi:
	Projects -> Properties -> C/C++ Build -> Settings -> GCC C++ Linker -> Libraries -> add -> SDL2 / SDL2_image / SDL2main
