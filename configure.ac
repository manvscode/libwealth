AC_PREREQ(2.67)
AC_INIT([libwealth], [1.0], [manvscode@gmail.com], [libwealth], [http://www.manvscode.com/])
AM_INIT_AUTOMAKE([1.13 foreign silent-rules -Wall -Werror])

AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([src/libwealth-config.h])

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CXX
AC_C_INLINE
AC_HEADER_STDC


AH_TOP([
#ifndef _LIBWEALTH_H_
#define _LIBWEALTH_H_
])
AH_BOTTOM([
#endif /* _LIBWEALTH_H_ */
])


DFLAGS="$DFLAGS"

CFLAGS="-std=c99 -Wall -pedantic -g -ggdb -O0 $DFLAGS -I/usr/local/include"
#CFLAGS="$CFLAGS "
LDFLAGS="$LDFLAGS -lcollections"

AM_PROG_AR
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
LT_INIT([shared static])

AC_ARG_ENABLE([tests],
	[AS_HELP_STRING([--disable-tests], [Enable test programs.])],
	[:],
	[enable_tests=no])

AM_CONDITIONAL([ENABLE_TESTS], [test "$enable_tests" = "yes"])



AC_PROG_INSTALL

echo " ------------------------------ "
echo ""
echo "  CFLAGS = $CFLAGS"
echo " LDFLAGS = $LDFLAGS"
echo ""
echo " ------------------------------ "

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	tests/Makefile
])

AC_OUTPUT
