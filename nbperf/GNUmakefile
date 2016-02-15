# $NetBSD: Makefile,v 1.1 2009/08/15 16:21:04 joerg Exp $

PROG=	nbperf
SRCS=	nbperf.c
SRCS+=	nbperf-bdz.c nbperf-chm.c nbperf-chm3.c
SRCS+=	graph2.c graph3.c
SRCS+=  mi_vector_hash.c

CPPFLAGS += -I..

OBJS = $(SRCS:.c=.o)

all: $(PROG)

clean:
	rm -f $(OBJS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_MAIN) $(INC) $(PIC) -c -o $@ $<

