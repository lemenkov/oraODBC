
# $Id: Makefile.am,v 1.14 2002/09/27 17:01:25 dbox Exp $
check_PROGRAMS=\
connect1 \
connect2 \
direct_exec1 \
direct_exec2 \
insert1 \
insert3 \
read1 \
read2 \
read3 \
malloctest \
rollback1 \
commit1 \
insert2 \
insert4 \
lowlevel \
getinfo1 \
insert5 \
insert6 \
insert7


INCLUDES = -I${ODBCIDIR} -I../src \
 -I${ODBCIDIR} -I${ORAIDIR} -I${ORANDIR} -I${ORAPDIR}

EXTRA_DIST = check_oracle_environment runtests 
TESTS = check_oracle_environment $(check_PROGRAMS)

LDFLAGS =     -L${ODBCLDIR} -lodbc -L../src/.libs -loraodbc ../src/mem_functions.o ../src/common.o ../src/common_functions.o

CLEANFILES=*.*~ .deps/*.P


