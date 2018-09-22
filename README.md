# Cucurucho
Taller de Programacion 2-2018

Para configurar el Eclipse :

Agregar una variable de enterno asi:
Run -> Run Configurations... -> Enviroment -> New Enviroment Variable. 
	Nombre de la variable: LD_LIBRARY_PATH
	Value: /usr/local/lib

Agregar librerias asi:
	Projects -> Properties -> C/C++ Build -> Settings -> GCC C++ Linker -> Libraries -> add -> SDL2 / SDL2_image / xml2 / dl
	Y en library search path --> /usr/lib
	                                                  en GCC C++ Linker -> Miscellaneous -> linker flags -lxml2


En GCC C Compiler --> Miscellaneous --> other flags, agregar: -I/usr/include/libxml2
    

En GCC C++ Compiler --> Miscellaneous --> other flags, agregar: -std=c++11
y en 	   Compiler --> Includes --> include paths, agregar /usr/include/libxml2
