dnl Process this file with autoconf to produce a configure script.
AC_INIT([oraodbc], [0.5.1])

AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR(src/SQLAllocHandle.c)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AM_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl Checks for libraries.
AC_CHECK_LIB(pthread, pthread_mutex_lock)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/types.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl set up libtool
AM_PROG_LIBTOOL

dnl set up lib version
LIB_VERSION="1:0:0"
AC_SUBST(LIB_VERSION)

dnl Check for UnixODBC headers and libraries
AC_CHECK_HEADERS(odbcinst.h,,AC_MSG_ERROR(unixODBC: odbcinst.h not found))
AC_CHECK_HEADERS(sqlext.h,,AC_MSG_ERROR(unixODBC: sqlext.h not found))
AC_CHECK_HEADERS(sql.h,,AC_MSG_ERROR(unixODBC: sql.h not found))
AC_CHECK_HEADERS(sqltypes.h,,AC_MSG_ERROR(unixODBC: sqltypes.h not found))
AC_CHECK_LIB([odbc],SQLAllocHandle, [], [AC_MSG_ERROR(could not find unixODBC (-lodbc))])
AC_CHECK_LIB([odbcinst],SQLGetPrivateProfileString, [], [AC_MSG_ERROR(could not find unixODBC (-lodbcinst))])
LD_ODBCINSTLIB="-lodbc -lodbcinst"
AC_SUBST(LD_ODBCINSTLIB)

dnl Look for ORACLE_HOME shell variable first
AC_MSG_CHECKING(for ORACLE_HOME)
if test "x$ORACLE_HOME" != "x"
then
	oraclehome="$ORACLE_HOME"
fi

dnl Allow user to override  ORACLE_HOME
AC_ARG_WITH([oraclehome],
	    AC_HELP_STRING([--with-oraclehome[=DIR]],[Look in DIR for the ORACLE]),[ oraclehome="$withval"]
	    )


dnl Check that we set up oraclehome already
if test "x$oraclehome" = "x"
then
	AC_MSG_ERROR(Cannot find ORACLE_HOME. Try --with-oraclehome=PATH)
fi
AC_MSG_RESULT(found and set to $oraclehome)

dnl Check for necessary Oracle headers
orahdir=""
AC_MSG_CHECKING(for oci.h)
for d in $includedir $oraclehome/include $oraclehome/rdbms/demo $oraclehome/rdbms/public $oraclehome/oci/include
do
	if test -f $d/oci.h
	then
		AC_MSG_RESULT(found oci.h in $d)
		orahdir=$d
		break
	fi
done

if test "x$orahdir" = "x"
then
	AC_MSG_ERROR( oci.h not found )
else
	ORAIDIR=${orahdir}
	AC_SUBST(ORAIDIR)
fi

dnl Check for necessary Oracle libraries
oraldir=""
AC_MSG_CHECKING(for libclntsh )
for d in $libdir $oraclehome/lib
do
	if test -f $d/libclntsh.so || test -f $d/libclntsh.a
	then
		AC_MSG_RESULT(found libclntsh.so or libclntsh.a in $d)
		oraldir=$d
		ORALDIR=${oraldir}
		AC_SUBST(ORALDIR)
		break
	fi
done

if test x$oraldir = x
then
	AC_MSG_ERROR( libclntsh not found )
fi

AC_CHECK_LIB([clntsh], [OCIEnvCreate], AC_DEFINE([LIBCLNTSH8], [], [Enable libclntsh.so version switching]),, "-L$oraldir")

ORALIB="-lclntsh"
AC_SUBST(ORALIB)

dnl Compile time options.

dnl Undefined symbol problems (slpmprodstab) with libclntsh.so. If linking
dnl against it with a function that it should contain whatever version it
dnl is fails, then there is a good chance that the symbol hack is required.
AC_CHECK_LIB([clntsh], [OCIInitialize], [symbolcheck="no"], [symbolcheck="yes"], "-L$oraldir")
AC_MSG_CHECKING(whether to enable libclntsh.so undefined symbol hack)
AC_ARG_ENABLE([symbol-hack],
	      AC_HELP_STRING([--enable-symbol-hack[=yes|no]], [Enable libclntsh.so undefined symbol workaround [default=no]]),
	      [symbolval=$enableval], [ symbolval="default"]
	      )
if test x$symbolval = xyes
then
	AC_DEFINE([EXPORT_SLPMPRODSTAB], [], [Enable symbol hack])
	AC_MSG_RESULT(yes (autodetection overridden))
else
	if test x$symbolval = xdefault
	then
		if test x$symbolcheck = xyes
		then
			AC_MSG_RESULT(yes (autodetected))
			AC_DEFINE(EXPORT_SLPMPRODSTAB)
		else
			AC_MSG_RESULT(no (autodetected))
		fi
	else
		AC_MSG_RESULT(no (autodetection overridden))
	fi
fi

AC_ARG_ENABLE([binary_float],
	      AC_HELP_STRING([--enable-binary-float[=yes|no]],[support Oracle 10 IEEE 754 floating point numbers [default=no]]),
	      [
	       if test x$enableval = xno
	       then
		       AC_MSG_RESULT(disabling BINARY_FLOAT support)
	       else
		       AC_DEFINE([IEEE_754_FLT], [], [Oracle 10 IEEE_754 floating point support])
		       AC_MSG_RESULT(enabling BINARY_FLOAT support)
	       fi
	      ]
	     )

AC_MSG_CHECKING(whether to enable ODBC tracing)
AC_ARG_ENABLE([trace],
	      AC_HELP_STRING([--enable-trace[=yes|no]],[Include ODBC tracing (slow) [default=no]]),
	      [traceval=$enableval], [traceval="no"]
	      )

if test x$traceval = xyes
then
	AC_DEFINE([ENABLE_TRACE], [], [Tracing enabled])
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_MSG_CHECKING(whether to use just user or all catalog)
AC_ARG_ENABLE([user-catalog],
	      AC_HELP_STRING([--enable-user-catalog[=yes|no]], [User user catalog for catalog functions]),
	      [catalog=$enableval], [catalog="no"]
	      )
if test x$catalog = xno || test x$catalog = xall
then
	AC_MSG_RESULT(all catalog)
else
	AC_DEFINE([ENABLE_USER_CATALOG], [], [Enable user catalog])
	AC_MSG_RESULT(user catalog)
fi

AC_OUTPUT( Makefile test/Makefile src/Makefile  )
