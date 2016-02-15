#	$NetBSD: Makefile,v 1.11 2013/01/24 10:41:28 roy Exp $
#	@(#)Makefile	8.1 (Berkeley) 6/6/93

PROG=	tput
SRCS=tput.c
DPADD=	${LIBTERMINFO}
LDADD=	-lterminfo
MLINKS=	tput.1 clear.1
SCRIPTS=clear.sh

#CPPFLAGS+=	-I${.CURDIR}/../../lib/libterminfo

CPPFLAGS+= -I.. -I. -I../libterminfo
CFLAGS+=-Werror-implicit-function-declaration

OBJS = $(SRCS:.c=.o)

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG) $(CLEANFILES)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -L ../libterminfo $(LDADD) -static

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_MAIN) $(INC) $(PIC) -c -o $@ $<

