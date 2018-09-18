bin_PROGRAMS = contra
contra_SOURCES = Personaje.cpp EntidadDibujable.cpp plataformas.cpp iocontra.cpp temporizador.cpp juego.cpp registro.cpp cfg.cpp Juego.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS)
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image -ldl
contradir = $(prefix)/bin

imagenes=	foo.png \
		nivel.png \
		personaje.png \
		_fondo_1_.png \
		_fondo_2_.png \
		bob.png \
		piedra1.bmp \
		ContraPersonaje.gif
imagenes_o=	imagen-foo.o \
		imagen-nivel.o \
		imagen-personaje.o \
		imagen-_fondo_1_.o \
		imagen-_fondo_2_.o \
		imagen-bob.o \
		imagen-piedra1.o \
		imagen-ContraPersonaje.o
contra_LDFLAGS= $(imagenes_o) -rdynamic
nodist_contra_SOURCES = $(imagenes_o)
BUILT_SOURCES = $(imagenes)
CLEANFILES = $(imagenes_o)

$(imagenes): Makefile
	echo creando $@; \
	$(MKDIR_P) ../imagenes && \
	cp "../../imagenes/$@" ../imagenes ; \
	FILE=`echo "$$(basename $@)"`; \
	EXT=$${FILE##*.}; \
	NAME=`echo "$$(basename $@ .$$EXT)"`; \
	objcopy -I binary -O elf64-x86-64 -B i386 ../imagenes/$$FILE imagen-$$NAME.o;

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.png $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.bmp $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/*.gif $(DESTDIR)$(bindir)/imagenes
