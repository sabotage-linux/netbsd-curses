#	$NetBSD: Makefile,v 1.22 2012/03/21 05:37:44 matt Exp $

USE_SHLIBDIR=	yes

LIB=		terminfo
WARNS?=		5

CPPFLAGS+=	-I.
CPPFLAGS+=	-I..

SRCS=		term.c ti.c setupterm.c curterm.c tparm.c tputs.c
SRCS+=		compile.c hash.c
SRCS+=		cdbr.c
SRCS+=		mi_vector_hash.c

INCS=		term.h
INCSDIR=	/usr/include

COPTS.tparm.c = -Wno-format-nonliteral

MAN=		terminfo.3 terminfo.5
MLINKS=		terminfo.3 setupterm.3 \
		terminfo.3 set_curterm.3 terminfo.3 del_curterm.3 \
		terminfo.3 termname.3 terminfo.3 longname.3 \
		terminfo.3 tigetnum.3 terminfo.3 tigetflag.3 \
		terminfo.3 tigetstr.3 terminfo.3 tparm.3 terminfo.3 tputs.3 \
		terminfo.3 putp.3 \
		terminfo.3 ti_setupterm.3 terminfo.3 ti_getflag.3 \
		terminfo.3 ti_getnum.3 terminfo.3 ti_getstr.3 \
		terminfo.3 tiparm.3 terminfo.3 ti_tiparm.3 \
		terminfo.3 ti_puts.3 terminfo.3 ti_putp.3

# Build in termcap emulation
SRCS+=		termcap.c
INCS+=		termcap.h
MAN+=		termcap.3
MLINKS+=	termcap.3 tgetent.3 termcap.3 tgetflag.3 termcap.3 tgetnum.3 \
		termcap.3 tgetstr.3 termcap.3 tgoto.3

#CPPFLAGS+=	-I${.OBJDIR}

#	$NetBSD: Makefile.hash,v 1.7 2013/01/24 14:17:40 roy Exp $

TERMINFODIR=../terminfo
SCRIPT_ENV=	\
	TERMINFODIR=$(TERMINFODIR) \
	TOOL_AWK=awk \
	TOOL_NBPERF=../nbperf/nbperf \
	TOOL_SED=sed \
	TOOL_SORT=sort \
	TOOL_TIC=../tic/tic

HOST_SH?=/bin/sh

all: libterminfo.a

$(TOOL_NBPERF):
	$(MAKE) -C ../nbperf nbperf

$(TOOL_TIC):
	$(MAKE) -C ../tic tic

# Generate our string and hash tables
hash.c: genhash term.h $(TOOL_NBPERF)
		@echo "Generating terminfo hash"
		$(SCRIPT_ENV) $(HOST_SH) $^ > $@

termcap_hash.c: genthash termcap_map.c $(TOOL_NBPERF)
		@echo "Generating termcap hash"
		${SCRIPT_ENV} ${HOST_SH} $^ > $@

# Allow terminfo descriptions to be compiled into libterminfo
compiled_terms.c: genterms term.h ${TERMINFODIR}/terminfo $(TOOL_TIC)
		@echo "Generating compiled terminfo descriptions"
		${SCRIPT_ENV} ${HOST_SH} $^ > $@

ifeq ($(USETOOLS),yes)
compiled_terms.c: ${TOOL_TIC}
endif

DPSRCS+=	hash.c termcap_hash.c compiled_terms.c

CLEANFILES+=	hash.c termcap_hash.c compiled_terms.c

clean:
	rm -f $(OBJS) libterminfo.a $(CLEANFILES)

# Generate our man pages
terminfo.5: genman terminfo.5.in term.h termcap_map.c
		@echo "Generating terminfo man pages"
		${SCRIPT_ENV} ${HOST_SH} $^ > $@

CLEANFILES+=	terminfo.5
CFLAGS+=-Werror-implicit-function-declaration

OBJS = $(SRCS:.c=.o)

man: terminfo.5

term.c: compiled_terms.c
termcap.c: termcap_hash.c

libterminfo.a: $(OBJS)
	ar cru $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(CFLAGS_MAIN) $(INC) $(PIC) -c -o $@ $<

