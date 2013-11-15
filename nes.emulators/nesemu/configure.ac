#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nesemu], [0.6.0], [silverhawk.rogue@gmail.com])
###FIX LT_INIT
AC_PROG_LIBTOOL
###FIX AM_PROG_LIBTOOL


AC_CONFIG_SRCDIR([src/nesemu.c])
AM_INIT_AUTOMAKE


###AM_INIT_AUTOMAKE(pondyparser, 0.6.0)

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
AC_CHECK_HEADERS([SDL.h])
AC_CHECK_LIB(SDL, SDL_Init, [], [
	echo "Not building SDL GUI."])

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([memset])

###AC_CONFIG_FILES([Makefile])
AC_OUTPUT(Makefile src/Makefile)
###AC_OUTPUT
