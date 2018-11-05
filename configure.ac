AC_INIT([contra], [1.0])
AC_SUBST([CONVERT])
AC_PATH_PROG([CONVERT], [convert])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PATH_XML2(2.4.0)
PKG_CHECK_MODULES(SDL2, sdl2 SDL2_image SDL2_ttf)
AC_PROG_CXX
AC_PROG_MKDIR_P 
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT

    
