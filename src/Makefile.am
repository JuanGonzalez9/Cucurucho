bin_PROGRAMS = contra
contra_SOURCES = registro.cpp cfg.cpp Juego.cpp CucuruchoLocal.cpp
contra_CPPFLAGS = -Wall $(XML_CPPFLAGS)
contra_LDADD= $(XML_LIBS) -lSDL2 -lSDL2_image
contradir = $(prefix)/bin

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(bindir)/imagenes && \
	cp ../../imagenes/* $(DESTDIR)$(bindir)/imagenes

