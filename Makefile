# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/products/devel/dbox/odbc/install
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/oracle
pkglibdir = $(libdir)/oracle
pkgincludedir = $(includedir)/oracle

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = i686-pc-linux-gnu
build_triplet = i686-pc-linux-gnu
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
target_alias = i686-pc-linux-gnu
target_triplet = i686-pc-linux-gnu
AS = @AS@
CC = gcc
CPP = gcc -E
DLLTOOL = @DLLTOOL@
LD_ODBCINSTLIB = -L/afs/fnal.gov/files/code/e875/general/external/Linux2-GCC_2_95/lib -lodbcinst
LEX = flex
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIB_VERSION = 1:0:0
LN_S = ln -s
MAKEINFO = makeinfo
OBJDUMP = @OBJDUMP@
ODBCIDIR = /afs/fnal.gov/files/code/e875/general/external/Linux2-GCC_2_95/include
ODBCINSTLIB = libodbcinst.so
ODBCLDIR = /afs/fnal.gov/files/code/e875/general/external/Linux2-GCC_2_95/lib
ODBCLIB = libodbc.so
ORAIDIR = /usr/products/Linux/oracle_client/v8_1_6/rdbms/demo
ORALDIR = /usr/products/Linux/oracle_client/v8_1_6/lib
ORALIB = -lclntsh
ORANDIR = /usr/products/Linux/oracle_client/v8_1_6/network/public
ORAPDIR = /usr/products/Linux/oracle_client/v8_1_6/rdbms/public
PACKAGE = oracle
RANLIB = ranlib
VERSION = 0.4.0pre1

check_PROGRAMS =  connect1 connect2 insert1 insert2 direct_exec1 read1 malloctest 


INCLUDES = -I${ODBCIDIR} -I../src

EXTRA_DIST = 

LDFLAGS =  	-L${ODBCLDIR} -lodbc ../src/mem_functions.o

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 

DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS = 
LIBS = -lpthread 
connect1_SOURCES = connect1.c
connect1_OBJECTS =  connect1.o
connect1_LDADD = $(LDADD)
connect1_DEPENDENCIES = 
connect1_LDFLAGS = 
connect2_SOURCES = connect2.c
connect2_OBJECTS =  connect2.o
connect2_LDADD = $(LDADD)
connect2_DEPENDENCIES = 
connect2_LDFLAGS = 
insert1_SOURCES = insert1.c
insert1_OBJECTS =  insert1.o
insert1_LDADD = $(LDADD)
insert1_DEPENDENCIES = 
insert1_LDFLAGS = 
insert2_SOURCES = insert2.c
insert2_OBJECTS =  insert2.o
insert2_LDADD = $(LDADD)
insert2_DEPENDENCIES = 
insert2_LDFLAGS = 
direct_exec1_SOURCES = direct_exec1.c
direct_exec1_OBJECTS =  direct_exec1.o
direct_exec1_LDADD = $(LDADD)
direct_exec1_DEPENDENCIES = 
direct_exec1_LDFLAGS = 
read1_SOURCES = read1.c
read1_OBJECTS =  read1.o
read1_LDADD = $(LDADD)
read1_DEPENDENCIES = 
read1_LDFLAGS = 
malloctest_SOURCES = malloctest.c
malloctest_OBJECTS =  malloctest.o
malloctest_LDADD = $(LDADD)
malloctest_DEPENDENCIES = 
malloctest_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
DEP_FILES =  .deps/connect1.P .deps/connect2.P .deps/direct_exec1.P \
.deps/insert1.P .deps/insert2.P .deps/malloctest.P .deps/read1.P
SOURCES = connect1.c connect2.c insert1.c insert2.c direct_exec1.c read1.c malloctest.c
OBJECTS = connect1.o connect2.o insert1.o insert2.o direct_exec1.o read1.o malloctest.o

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .lo .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu test/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-checkPROGRAMS:

clean-checkPROGRAMS:
	-test -z "$(check_PROGRAMS)" || rm -f $(check_PROGRAMS)

distclean-checkPROGRAMS:

maintainer-clean-checkPROGRAMS:

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

connect1: $(connect1_OBJECTS) $(connect1_DEPENDENCIES)
	@rm -f connect1
	$(LINK) $(connect1_LDFLAGS) $(connect1_OBJECTS) $(connect1_LDADD) $(LIBS)

connect2: $(connect2_OBJECTS) $(connect2_DEPENDENCIES)
	@rm -f connect2
	$(LINK) $(connect2_LDFLAGS) $(connect2_OBJECTS) $(connect2_LDADD) $(LIBS)

insert1: $(insert1_OBJECTS) $(insert1_DEPENDENCIES)
	@rm -f insert1
	$(LINK) $(insert1_LDFLAGS) $(insert1_OBJECTS) $(insert1_LDADD) $(LIBS)

insert2: $(insert2_OBJECTS) $(insert2_DEPENDENCIES)
	@rm -f insert2
	$(LINK) $(insert2_LDFLAGS) $(insert2_OBJECTS) $(insert2_LDADD) $(LIBS)

direct_exec1: $(direct_exec1_OBJECTS) $(direct_exec1_DEPENDENCIES)
	@rm -f direct_exec1
	$(LINK) $(direct_exec1_LDFLAGS) $(direct_exec1_OBJECTS) $(direct_exec1_LDADD) $(LIBS)

read1: $(read1_OBJECTS) $(read1_DEPENDENCIES)
	@rm -f read1
	$(LINK) $(read1_LDFLAGS) $(read1_OBJECTS) $(read1_LDADD) $(LIBS)

malloctest: $(malloctest_OBJECTS) $(malloctest_DEPENDENCIES)
	@rm -f malloctest
	$(LINK) $(malloctest_LDFLAGS) $(malloctest_OBJECTS) $(malloctest_LDADD) $(LIBS)

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = test

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu test/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-checkPROGRAMS mostlyclean-compile \
		mostlyclean-libtool mostlyclean-tags mostlyclean-depend \
		mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-checkPROGRAMS clean-compile clean-libtool clean-tags \
		clean-depend clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-checkPROGRAMS distclean-compile \
		distclean-libtool distclean-tags distclean-depend \
		distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-checkPROGRAMS \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-tags maintainer-clean-depend \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-checkPROGRAMS distclean-checkPROGRAMS \
clean-checkPROGRAMS maintainer-clean-checkPROGRAMS mostlyclean-compile \
distclean-compile clean-compile maintainer-clean-compile \
mostlyclean-libtool distclean-libtool clean-libtool \
maintainer-clean-libtool tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir mostlyclean-depend \
distclean-depend clean-depend maintainer-clean-depend info-am info \
dvi-am dvi check check-am installcheck-am installcheck install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
