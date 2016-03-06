HOSTCC ?= $(CC)
AWK ?= awk
AR ?= ar
RANLIB ?= ranlib
HOST_SH ?= /bin/sh
LN ?= ln
INSTALL ?= install
MKDIR ?= mkdir

SO_SUFFIX ?= .so

PIC = -fPIC

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include

-include config.mak

ifdef SMALL
CPPFLAGS+=-DSMALL
endif

ifneq ($(HOSTCC),$(CC))
CROSSCOMPILING=1
else
CROSSCOMPILING=0
endif

ifneq (,$(findstring $(PIC),$(CFLAGS)))
#CFLAGS contains -fPIC, so we can use the same obj files for dyn/static lib
ALLPIC=1
else
ALLPIC=0
endif

CFLAGS+=-Werror-implicit-function-declaration
CPPFLAGS+= -I. -I./libterminfo

TOOL_NBPERF=	nbperf/nbperf
NBPERF_SRCS=	nbperf/nbperf.c
NBPERF_SRCS+=	nbperf/nbperf-bdz.c nbperf/nbperf-chm.c nbperf/nbperf-chm3.c
NBPERF_SRCS+=	nbperf/graph2.c nbperf/graph3.c
NBPERF_SRCS+=   nbperf/mi_vector_hash.c
NBPERF_OBJS=    $(NBPERF_SRCS:.c=.o)

TOOL_HOSTTIC=tic/host_tic
TOOL_TTIC=tic/tic
TIC_SRCS=tic/tic.c
TIC_SRCS+=tic/cdbw.c tic/mi_vector_hash.c tic/compile.c tic/hash.c
CLEANFILES+=tic/hash.c
TIC_OBJS=$(TIC_SRCS:.c=.o)
TIC_HOBJS=$(TIC_SRCS:.c=.ho)
TOOL_TIC=$(TOOL_HOSTTIC)

TI_SRCS= libterminfo/term.c libterminfo/ti.c libterminfo/setupterm.c \
         libterminfo/curterm.c libterminfo/tparm.c libterminfo/tputs.c
TI_SRCS+=libterminfo/compile.c libterminfo/hash.c
TI_SRCS+=libterminfo/cdbr.c
TI_SRCS+=libterminfo/mi_vector_hash.c
# Build in termcap emulation
TI_SRCS+=libterminfo/termcap.c
TI_LIBA=libterminfo/libterminfo.a
TI_LIBSO=libterminfo/libterminfo$(SO_SUFFIX)
TI_OBJS=$(TI_SRCS:.c=.o)
TI_LOBJS=$(TI_SRCS:.c=.lo)

libterminfo/term.o: CPPFLAGS+=-DINSTALL_PREFIX=\"$(PREFIX)\"
libterminfo/term.lo: CPPFLAGS+=-DINSTALL_PREFIX=\"$(PREFIX)\"

CU_SRCS_=acs.c addbytes.c addch.c addchnstr.c addnstr.c attributes.c \
	background.c bell.c border.c box.c chgat.c clear.c clearok.c \
	clrtobot.c clrtoeol.c color.c copywin.c cr_put.c \
	ctrace.c cur_hash.c curs_set.c \
	curses.c delch.c deleteln.c delwin.c echochar.c erase.c fileio.c \
	flushok.c fullname.c getch.c getstr.c getyx.c id_subwins.c idlok.c \
	idcok.c inch.c inchstr.c initscr.c insch.c insdelln.c insertln.c \
	instr.c keypad.c keyname.c leaveok.c line.c meta.c move.c \
	mvwin.c newwin.c nodelay.c notimeout.c overlay.c overwrite.c pause.c \
	printw.c putchar.c refresh.c resize.c scanw.c screen.c scroll.c \
	scrollok.c setterm.c standout.c timeout.c toucholap.c touchwin.c \
	tstp.c tty.c unctrl.c underscore.c


ifndef DISABLE_WCHAR
CPPFLAGS+=-DHAVE_WCHAR
CU_SRCS_+= cchar.c add_wch.c add_wchstr.c addwstr.c echo_wchar.c ins_wch.c \
	insstr.c ins_wstr.c get_wch.c get_wstr.c in_wch.c in_wchstr.c \
	inwstr.c
else
CPPFLAGS+=-DDISABLE_WCHAR
endif

CU_SRCS=$(patsubst %,libcurses/%,$(CU_SRCS_))
CU_INCS=libcurses/curses.h libcurses/unctrl.h
CU_OBJS=$(CU_SRCS:.c=.o)
CU_LIBA=libcurses/libcurses.a
CU_LIBSO=libcurses/libcurses$(SO_SUFFIX)
CU_LOBJS=$(CU_SRCS:.c=.lo)

PA_SRCS_ = _deck.c above.c below.c bottom.c del.c getuser.c hidden.c
PA_SRCS_+= hide.c move.c new.c replace.c setuser.c show.c top.c
PA_SRCS_+= update.c window.c
PA_SRCS=$(patsubst %,libpanel/%,$(PA_SRCS_))
PA_INCS=libpanel/panel.h
PA_OBJS=$(PA_SRCS:.c=.o)
PA_LIBA=libpanel/libpanel.a
PA_LIBSO=libpanel/libpanel$(SO_SUFFIX)
PA_LOBJS=$(PA_SRCS:.c=.lo)
PA_MAN_=move_panel.3 new_panel.3 panel.3 panel_above.3 panel_hidden.3 \
	panel_userptr.3 update_panels.3

ME_SRCS_=menu.c item.c userptr.c internals.c driver.c post.c attributes.c
ME_MAN=	menu_attributes.3 menu_item_name.3 menu_items.3 menu_userptr.3 \
	menu_cursor.3 menu_item_new.3 menu_mark.3 menu_win.3 menu_driver.3 \
	menu_item_opts.3 menu_new.3 menus.3 menu_format.3 menu_item_userptr.3 \
	menu_opts.3 menu_hook.3 menu_item_value.3 menu_pattern.3 \
	menu_item_current.3 menu_item_visible.3 menu_post.3
ME_SRCS=$(patsubst %,libmenu/%,$(ME_SRCS_))
ME_INCS=libmenu/menu.h libmenu/eti.h
ME_OBJS=$(ME_SRCS:.c=.o)
ME_LIBA=libmenu/libmenu.a
ME_LIBSO=libmenu/libmenu$(SO_SUFFIX)
ME_LOBJS=$(ME_SRCS:.c=.lo)

FO_SRCS_=driver.c field_types.c internals.c field.c form.c post.c type_alnum.c \
        type_alpha.c type_integer.c type_numeric.c type_enum.c type_regex.c \
	type_ipv4.c type_ipv6.c
FO_MAN=	form_cursor.3 form_data.3 form_driver.3 form_field.3 \
	form_field_attributes.3 form_field_buffer.3 form_field_info.3 \
	form_field_just.3 form_field_new.3 form_field_opts.3 \
	form_field_userptr.3 form_field_validation.3 form_fieldtype.3 \
	form_hook.3 form_new.3 form_new_page.3 form_opts.3 form_page.3 \
	form_post.3 form_userptr.3 form_win.3 forms.3
FO_SRCS=$(patsubst %,libform/%,$(FO_SRCS_))
FO_INCS=libform/form.h
FO_OBJS=$(FO_SRCS:.c=.o)
FO_LIBA=libform/libform.a
FO_LIBSO=libform/libform$(SO_SUFFIX)
FO_LOBJS=$(FO_SRCS:.c=.lo)

TS_SRCS_=map.c misc.c set.c term.c tset.c
TS_SRCS=$(patsubst %,tset/%,$(TS_SRCS_))
TS_OBJS=$(TS_SRCS:.c=.o)

TP_SRCS=tput/tput.c
TP_OBJS=tput/tput.o

STALIBS=$(TI_LIBA) $(CU_LIBA) $(PA_LIBA) $(ME_LIBA) $(FO_LIBA)
DYNLIBS=$(TI_LIBSO) $(CU_LIBSO) $(PA_LIBSO) $(ME_LIBSO) $(FO_LIBSO)
PROGS=$(TOOL_TTIC) tset/tset tput/tput
TI_LINKLIB=$(TI_LIBA)

all: $(STALIBS) $(DYNLIBS) $(PROGS)

all-static: $(STALIBS) $(PROGS)

all-dynamic: TI_LINKLIB=$(TI_LIBSO)
all-dynamic: $(DYNLIBS) $(PROGS)

install-tic: $(TOOL_TTIC)
	$(INSTALL) -Dm 755 $(TOOL_TTIC) $(DESTDIR)$(BINDIR)/tic

install-tset: tset/tset
	$(INSTALL) -Dm 755 tset/tset $(DESTDIR)$(BINDIR)/tset
	$(LN) -sf $(notdir $<) $(DESTDIR)$(BINDIR)/reset

install-tput: tput/tput tput/clear.sh
	$(INSTALL) -Dm 755 tput/tput $(DESTDIR)$(BINDIR)/tput
	$(INSTALL) -Dm 755 tput/clear.sh $(DESTDIR)$(BINDIR)/clear

install-progs: install-tic install-tset install-tput

install-headers-curses: libcurses/curses.h libcurses/unctrl.h
	$(INSTALL) -Dm 644 libcurses/curses.h $(DESTDIR)$(INCDIR)/curses.h
	$(INSTALL) -Dm 644 libcurses/unctrl.h $(DESTDIR)$(INCDIR)/unctrl.h
	$(LN) -sf curses.h $(DESTDIR)$(INCDIR)/ncurses.h

install-headers-terminfo: libterminfo/term.h libterminfo/termcap.h
	$(INSTALL) -Dm 644 libterminfo/term.h $(DESTDIR)$(INCDIR)/term.h
	$(INSTALL) -Dm 644 libterminfo/termcap.h $(DESTDIR)$(INCDIR)/termcap.h

install-headers-panel: libpanel/panel.h
	$(INSTALL) -Dm 644 libpanel/panel.h $(DESTDIR)$(INCDIR)/panel.h

install-headers-menu: libmenu/menu.h libmenu/eti.h
	$(INSTALL) -Dm 644 libmenu/menu.h $(DESTDIR)$(INCDIR)/menu.h
	$(INSTALL) -Dm 644 libmenu/eti.h $(DESTDIR)$(INCDIR)/eti.h

install-headers-form: libform/form.h
	$(INSTALL) -Dm 644 libform/form.h $(DESTDIR)$(INCDIR)/form.h

install-headers: install-headers-terminfo install-headers-curses install-headers-panel install-headers-menu install-headers-form

install-stalib-curses: $(CU_LIBA)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libncurses.a
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libncursesw.a

install-stalib-terminfo: $(TI_LIBA)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libtermcap.a

install-stalib-panel: $(PA_LIBA)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w.a

install-stalib-menu: $(ME_LIBA)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w.a

install-stalib-form: $(FO_LIBA)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w.a

install-stalibs: install-stalib-terminfo install-stalib-curses install-stalib-panel install-stalib-menu install-stalib-form

install-dynlib-curses: $(CU_LIBSO)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libncurses$(SO_SUFFIX)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libncursesw$(SO_SUFFIX)

install-dynlib-terminfo: $(TI_LIBSO)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/libtermcap$(SO_SUFFIX)

install-dynlib-panel: $(PA_LIBSO)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w$(SO_SUFFIX)

install-dynlib-menu: $(ME_LIBSO)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w$(SO_SUFFIX)

install-dynlib-form: $(FO_LIBSO)
	$(INSTALL) -Dm 644 $< $(DESTDIR)$(LIBDIR)/$(notdir $<)
	$(LN) -sf $(notdir $<) $(DESTDIR)$(LIBDIR)/$(basename $(notdir $<))w$(SO_SUFFIX)

install-dynlibs: install-dynlib-terminfo install-dynlib-curses install-dynlib-panel install-dynlib-menu install-dynlib-form
install-libs: install-stalibs install-dynlibs

install-pc-dir:
	$(MKDIR) -p $(DESTDIR)$(LIBDIR)/pkgconfig

install-pc-curses: install-pc-dir
	./genpc curses "curses terminal library" "-lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/curses.pc
	./genpc ncurses "ncurses terminal library" "-lncurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/ncurses.pc
	./genpc ncursesw "ncurses terminal library" "-lncurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/ncursesw.pc

install-pc-terminfo: install-pc-dir
	./genpc terminfo "terminfo library" "-lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/terminfo.pc
	./genpc termcap "termcap library" "-lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/termcap.pc

install-pc-panel: install-pc-dir
	./genpc panel "curses panel library" "-lpanel -lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/panel.pc
	./genpc panelw "curses panel library" "-lpanel -lncurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/panelw.pc

install-pc-menu: install-pc-dir
	./genpc menu "curses menu library" "-lmenu -lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/menu.pc
	./genpc menuw "curses menu library" "-lmenu -lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/menuw.pc

install-pc-form: install-pc-dir
	./genpc form "curses form library" "-lform -lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/form.pc
	./genpc formw "curses form library" "-lform -lcurses -lterminfo" > $(DESTDIR)$(LIBDIR)/pkgconfig/formw.pc

install-pcs: install-pc-form install-pc-menu install-pc-panel install-pc-terminfo install-pc-curses

install: install-headers install-libs install-progs install-pcs

install-static: install-headers install-progs install-pcs install-stalibs
install-dynamic: TI_LINKLIB=$(TI_LIBSO)
install-dynamic: install-headers install-progs install-pcs install-dynlibs

# TODO: gen (if necessary) and install manpages

clean:
	rm -f $(NBPERF_OBJS) $(TOOL_NBPERF) $(TIC_OBJS) $(TIC_HOBJS) \
	      $(TOOL_TTIC) $(TOOL_TIC) $(CLEANFILES) $(TI_LIBA) \
	      $(CU_LIBA) $(TI_OBJS) $(CU_OBJS) $(CU_LOBJS) $(TI_LOBJS) \
	      $(CU_LIBSO) $(TI_LIBSO) \
	      $(PA_LIBA) $(PA_LIBSO) $(PA_OBJS) $(PA_LOBJS) \
	      $(ME_LIBA) $(ME_LIBSO) $(ME_OBJS) $(ME_LOBJS) \
	      $(FO_LIBA) $(FO_LIBSO) $(FO_OBJS) $(FO_LOBJS) \
	      $(TS_OBJS) tset/tset $(TP_OBJS) tput/tput

$(TOOL_NBPERF): $(NBPERF_OBJS)
	$(HOSTCC) $(LDFLAGS_HOST) $^ -o $@

tset/tset: $(TI_LINKLIB)
tset/tset: $(TS_OBJS)
	$(CC) -o $@ $^ -L./libterminfo -lterminfo $(LDFLAGS)

tput/tput: $(TI_LINKLIB)
tput/tput: $(TP_OBJS)
	$(CC) -o $@ $^ -L./libterminfo -lterminfo $(LDFLAGS)

tset/%.o: tset/%.c
	$(CC) $(CPPFLAGS) -I./tset -I. -I./libterminfo $(CFLAGS) -c -o $@ $<
tput/%.o: tput/%.c
	$(CC) $(CPPFLAGS) -I./tset -I. -I./libterminfo $(CFLAGS) -c -o $@ $<
nbperf/%.o: nbperf/%.c
	$(HOSTCC) $(CPPFLAGS) -O0 -g0 $(CFLAGS_HOST) -c -o $@ $<

ifeq ($(CROSSCOMPILING),0)
tic/%.ho: tic/%.o
	$(LN) -sf $(notdir $<) $@

$(TOOL_HOSTTIC): $(TOOL_TTIC)
	$(LN) -sf $(notdir $<) $@
else
tic/%.ho: tic/%.c
	$(HOSTCC) $(CPPFLAGS) -I./tic -O0 -g0 $(CFLAGS_HOST) -c -o $@ $<

$(TOOL_HOSTTIC): $(TIC_HOBJS)
	$(HOSTCC) $(LDFLAGS_HOST) $^ -o $@
endif

tic/%.o: tic/%.c
	$(CC) $(CPPFLAGS) -I./tic $(CFLAGS) -c -o $@ $<

libcurses/%.o: libcurses/%.c
	$(CC) $(CPPFLAGS) -I./libcurses $(CFLAGS) -c -o $@ $<

libmenu/%.o: libmenu/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libmenu $(CFLAGS) -c -o $@ $<

libpanel/%.o: libpanel/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libpanel $(CFLAGS) -c -o $@ $<

libform/%.o: libform/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libmenu -I./libform $(CFLAGS) -c -o $@ $<

ifeq ($(ALLPIC),1)
$(CU_LOBJS): $(CU_OBJS)
libcurses/%.lo: libcurses/%.o
	$(LN) -sf $(notdir $<) $@

$(TI_LOBJS): $(TI_OBJS)
libterminfo/%.lo: libterminfo/%.o
	$(LN) -sf $(notdir $<) $@

$(PA_LOBJS): $(PA_OBJS)
libpanel/%.lo: libpanel/%.o
	$(LN) -sf $(notdir $<) $@

$(ME_LOBJS): $(ME_OBJS)
libmenu/%.lo: libmenu/%.o
	$(LN) -sf $(notdir $<) $@

$(FO_LOBJS): $(FO_OBJS)
libform/%.lo: libform/%.o
	$(LN) -sf $(notdir $<) $@
else
libcurses/%.lo: libcurses/%.c
	$(CC) $(CPPFLAGS) -I./libcurses $(CFLAGS) $(PIC) -c -o $@ $<

libterminfo/%.lo: libterminfo/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PIC) -c -o $@ $<

libpanel/%.lo: libpanel/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libpanel $(CFLAGS) $(PIC) -c -o $@ $<

libmenu/%.lo: libmenu/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libmenu $(CFLAGS) $(PIC) -c -o $@ $<

libform/%.lo: libform/%.c
	$(CC) $(CPPFLAGS) -I./libcurses -I./libmenu -I./libform $(CFLAGS) $(PIC) -c -o $@ $<
endif

tic/hash.c: libterminfo/hash.c
	cp libterminfo/hash.c tic/hash.c

$(TOOL_TTIC): $(TIC_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

TERMINFODIR=./terminfo
SCRIPT_ENV=	\
	TERMINFODIR=$(TERMINFODIR) \
	TOOL_AWK=awk \
	TOOL_NBPERF=$(TOOL_NBPERF) \
	TOOL_SED=sed \
	TOOL_SORT=sort \
	TOOL_TIC=$(TOOL_TIC)

$(TI_LIBA): $(TI_OBJS)
	rm -f $@
	$(AR) crs $@ $^
	$(RANLIB) $@

$(TI_LIBSO): $(TI_LOBJS)
	$(CC) -shared -o $@ $^ -Wl,-soname=$(notdir $@)

libterminfo/hash.c: libterminfo/genhash libterminfo/term.h
		@echo "Generating terminfo hash"
		$(SCRIPT_ENV) $(HOST_SH) $^ > $@

libterminfo/termcap_hash.c: libterminfo/genthash libterminfo/termcap_map.c
		@echo "Generating termcap hash"
		${SCRIPT_ENV} ${HOST_SH} $^ > $@

# Allow terminfo descriptions to be compiled into libterminfo
libterminfo/compiled_terms.c: libterminfo/genterms libterminfo/term.h ${TERMINFODIR}/terminfo
		@echo "Generating compiled terminfo descriptions"
		${SCRIPT_ENV} ${HOST_SH} $^ > $@

libterminfo/hash.c: $(TOOL_NBPERF)
libterminfo/termcap_hash.c: $(TOOL_NBPERF)
libterminfo/compiled_terms.c: $(TOOL_TIC)
libterminfo/term.c: libterminfo/compiled_terms.c
libterminfo/termcap.c: libterminfo/termcap_hash.c

CLEANFILES+=libterminfo/hash.c libterminfo/termcap_hash.c \
            libterminfo/compiled_terms.c

# Generate our man pages
#terminfo.5: genman terminfo.5.in term.h termcap_map.c
#		@echo "Generating terminfo man pages"
#		${SCRIPT_ENV} ${HOST_SH} $^ > $@

$(CU_LIBA): $(CU_OBJS)
	rm -f $@
	$(AR) crs $@ $^
	$(RANLIB) $@

$(CU_LIBSO): $(TI_LIBSO)
$(CU_LIBSO): $(CU_LOBJS)
	$(CC) -shared -o $@ $^ -Wl,-soname=$(notdir $@)

libcurses/fileio.h: libcurses/shlib_version libcurses/genfileioh.awk
	$(AWK) -f ./genfileioh.awk < ./shlib_version > ./fileio.h

$(PA_LIBA): $(PA_OBJS)
	rm -f $@
	$(AR) crs $@ $^
	$(RANLIB) $@

$(PA_LIBSO): $(CU_LIBSO)
$(PA_LIBSO): $(PA_LOBJS)
	$(CC) -shared -o $@ $^ -Wl,-soname=$(notdir $@)

$(ME_LIBA): $(ME_OBJS)
	rm -f $@
	$(AR) crs $@ $^
	$(RANLIB) $@

$(ME_LIBSO): $(CU_LIBSO)
$(ME_LIBSO): $(ME_LOBJS)
	$(CC) -shared -o $@ $^ -Wl,-soname=$(notdir $@)

$(FO_LIBA): $(FO_OBJS)
	rm -f $@
	$(AR) crs $@ $^
	$(RANLIB) $@

$(FO_LIBSO): $(CU_LIBSO)
$(FO_LIBSO): $(FO_LOBJS)
	$(CC) -shared -o $@ $^ -Wl,-soname=$(notdir $@)

