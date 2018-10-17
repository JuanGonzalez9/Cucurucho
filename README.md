# Cucurucho
Taller de Programacion 2-2018

Para jugar desde el cliente:

ejecutar el programa desde la consola con los parametros:
	
-d INFO servidor 8080 (o cualquier otro puerto)

y otra vez en otra consola en modo cliente:

-d INFO cliente 8080 (el puerto tiene que ser el mismo que el del servidor)

Para jugar pararse en la ventana chica que aparecec mas a la izquierda y apretar botones
Para cerrar, cerrar primero la ventana grande y despues apretar W



..................................COSAS DE ECLIPSE.............................................................................

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
