bin_PROGRAMS = contra
contra_SOURCES = registro.cpp cfg.cpp Juego.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS)
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image -ldl
contradir = $(prefix)/bin

imagenes=	foo.png \
		nivel1.png \
		personaje.png
imagenes_o=	imagen-foo.o \
		imagen-nivel1.o \
		imagen-personaje.o
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
	cp ../../imagenes/*.png $(DESTDIR)$(bindir)/imagenes

