bin_PROGRAMS = contra
contra_SOURCES = Bullet.cpp Enemigo.cpp Personaje.cpp Sprite.cpp EntidadDibujable.cpp plataformas.cpp iocontra.cpp temporizador.cpp juego.cpp registro.cpp cfg.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS)
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image -ldl
contradir = $(prefix)/bin

imagenes=	personaje.png \
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
		hielo.bmp \
		puente.bmp
		
imagenes_o=	imagen-personaje.o \
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
		imagen-puente.o
		
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

	

	

	

	

	

	
