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
lowlevel

INCLUDES = -I${ODBCIDIR} -I../src

TESTS = $(check_PROGRAMS)
EXTRA_DIST = runtests


LDFLAGS = \
	-L${ODBCLDIR} -lodbc ../src/mem_functions.o

CLEANFILES=*.*~ .deps/*.P


