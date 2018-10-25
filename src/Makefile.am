bin_PROGRAMS = contra
contra_SOURCES = DatosPersonaje.cpp JuegoCliente.cpp Parser.cpp ArmadorDeRespuesta.cpp traductorDelCliente.cpp Socket.cpp EscuchadorDeAcciones.cpp Bullet.cpp Enemigo.cpp Personaje.cpp Sprite.cpp EntidadDibujable.cpp plataformas.cpp iocontra.cpp temporizador.cpp juego.cpp registro.cpp cfg.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS) -std=c++11
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image -ldl
contradir = $(prefix)/bin

imagenes=	personaje.png \
         	personaje2.png \
         	personaje3.png \
         	personaje4.png \
		fondo1.png \
		fondo2.png \
		fondo3.png \
		fondo1vert.png \
		fondo2vert.png \
		fondo3vert.png \
		bullet.png \
		enemigo.png \
		piedra.bmp \
		metal.bmp \
		hielo.png \
		hielo1.png \
		nieve.png \
		boss.png \
		cascroca1.png \
		cascroca2.png \
		cascroca3.png \
		cascroca4.png \
		piedras.png \
		piedram.png \
		piedral.png \
		piedraxl.png \
		puente.bmp \
		puentesoga.png \
		nieve_arboles.png\
		fondo1lv1.png\
		fondo2lv1.png\
		fondo1lv3.png\
		fondo2lv3.png\
		monstruolevel2.bmp \
		monstruolevel3.bmp \
		puente2.png
		
imagenes_o=	imagen-personaje.o \
		imagen-personaje2.o \
		imagen-personaje3.o \
		imagen-personaje4.o \
		imagen-fondo1.o \
		imagen-fondo2.o \
		imagen-fondo3.o \
		imagen-fondo1vert.o \
		imagen-fondo2vert.o \
		imagen-fondo3vert.o \
		imagen-bullet.o \
		imagen-enemigo.o \
		imagen-piedra.o \
		imagen-metal.o \
		imagen-hielo.o \
		imagen-hielo1.o \
		imagen-nieve.o \
		imagen-boss.o \
		imagen-cascroca1.o \
		imagen-cascroca2.o \
		imagen-cascroca3.o \
		imagen-cascroca4.o \
		imagen-piedras.o \
		imagen-piedram.o \
		imagen-piedral.o \
		imagen-piedraxl.o \
		imagen-puente.o\
		imagen-puentesoga.o\
		imagen-nieve_arboles.o\
		imagen-fondo1lv1.o\
		imagen-fondo2lv1.o\
		imagen-fondo1lv3.o\
		imagen-fondo2lv3.o\
		imagen-monstruolevel2.o \
		imagen-monstruolevel3.o \
		imagen-puente2.o
		
contra_LDFLAGS= $(imagenes_o) -rdynamic
nodist_contra_SOURCES = $(imagenes_o)
BUILT_SOURCES = $(imagenes)
CLEANFILES = $(imagenes_o)

$(imagenes): Makefile
	FILE=`echo "$$(basename $@)"`; \
	EXT=$${FILE##*.}; \
	NAME=`echo "$$(basename $@ .$$EXT)"`; \
	if [ ! -f "imagen-$$NAME.o" ] || [ "../../imagenes/$@" -nt "imagen-$$NAME.o" ]; then \
		echo creando $@; \
		rm -f $(bin_PROGRAMS); \
		$(MKDIR_P) ../imagenes && \
		cp "../../imagenes/$@" ../imagenes ; \
		IMAGEN_DE_RESPALDO="imagen-gris-imagenes-$$NAME.$$EXT"; \
		$(CONVERT) ../imagenes/$$FILE -modulate 100,1 $$IMAGEN_DE_RESPALDO; \
		objcopy -I binary -O elf64-x86-64 -B i386 $$IMAGEN_DE_RESPALDO imagen-$$NAME.o; \
	fi;

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.png $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.bmp $(DESTDIR)$(bindir)/imagenes

	

	

	

	

	

	
