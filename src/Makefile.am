bin_PROGRAMS = contra
contra_SOURCES = Bullet.cpp Enemigo.cpp Personaje.cpp Sprite.cpp EntidadDibujable.cpp plataformas.cpp iocontra.cpp temporizador.cpp juego.cpp registro.cpp cfg.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS)
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image -ldl
contradir = $(prefix)/bin

imagenes=	foo.png \
		nivel.png \
		personaje.png \
		_fondo_1_.png \
		_fondo_2_.png \
		bob.png \
		bullet.png \
		enemigo.png \
		piedra1.bmp \
		metal.bmp \
		hielo.bmp \
		puente.bmp \
		ContraPersonaje.gif
imagenes_o=	imagen-foo.o \
		imagen-nivel.o \
		imagen-personaje.o \
		imagen-_fondo_1_.o \
		imagen-_fondo_2_.o \
		imagen-bob.o \
		imagen-bullet.o \
		imagen-enemigo.o \
		imagen-piedra1.o \
		imagen-metal.o \
		imagen-hielo.o \
		imagen-puente.o \
		imagen-ContraPersonaje.o
		
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
		TMP="imagen-gris-imagenes-$$NAME.png"; \
		$(CONVERT) ../imagenes/$$FILE -modulate 100,10 $$TMP; \
		objcopy -I binary -O elf64-x86-64 -B i386 $$TMP imagen-$$NAME.o; \
		echo name: $$TMP; \
	fi;

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.png $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.bmp $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.gif $(DESTDIR)$(bindir)/imagenes
