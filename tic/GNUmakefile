#	$NetBSD: Makefile,v 1.2 2012/05/31 20:10:06 joerg Exp $
PROG=		tic
WARNS=		4
SRCS=tic.c
SRCS+=cdbw.c mi_vector_hash.c compile.c hash.c
CLEANFILES+=hash.c

CPPFLAGS+=	-I../libterminfo
CPPFLAGS+= -I.. -I.
CFLAGS+=-Werror-implicit-function-declaration
ifndef HOSTPROG
LDADD+=		-lterminfo -lutil
DPADD+=		${LIBTERMINFO} ${LIBUTIL}
endif

OBJS = $(SRCS:.c=.o)

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG) $(CLEANFILES)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_MAIN) $(INC) $(PIC) -c -o $@ $<

hash.c: ../libterminfo/hash.c
	cp ../libterminfo/hash.c hash.c

../libterminfo/hash.c:
	$(MAKE) -C ../libterminfo hash.c

