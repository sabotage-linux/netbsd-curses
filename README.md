netbsd-libcurses portable edition
=================================

this is a port of netbsd's curses library for usage on Linux systems
(tested and developed on sabotage linux, based on musl libc).
It should be possible to use on other POSIX platforms as well with minor
changes.

it supports widechar (like the ncursesw build variant) and is close to being
a 100% feature-complete drop-in replacement for ncurses.

to this date, there are only 3 implementations of the curses API:

- the ubiquitous ncurses library, which is used on almost every UNIX system
  (Linux, Mac OS X, FreeBSD, OpenBSD and others).
- netbsd curses, previously used on netbsd only.
- pdcurses, targetting the windows terminal, SDL and X11 (unusable on UNIX
  terminals).

Why do we need an ncurses replacement ?
---------------------------------------
- Competition is good.
- Ncurses is one of the most used libraries in the linux ecosystem and
  therefore it's usually one of the first libraries built in a source-based
  distro.
  There's basically no way around it, unless you're building a single-purpose
  appliance.
- Readability/Hacking.
  even though ncurses is free software, it is very hard to change its source
  code:
  - extremely complicated build process with several layers of preprocessing
    using tools like awk, sed and the output of the C preprocessor with reliance
    on implementation details, [as shown by the recent breakage when distros
    updated to GCC 5][0].
  - heavy use of macros, making the code hard to read.
  - very much code (bloat).
- Usability.
  the default configuration of ncurses is usually not what one wants, and it
  features several dozens of configure options to customize the build,
  for example making it split up in several smaller libraries, with or without
  widechar support, etc.

  this makes it hard to guess which files to link against and which headers to
  use when building a package against ncurses.
  `-lncurses`? `-lncursesw -lterminfo -ltic`? `curses.h`? `ncurses/ncursesw.h`?
  this filename chaos [is often fixed in a distro-specific manner][1].

  to accomodate for this, ncurses ships its own config tool ncurses(w)5-config
  instead of a standardized pkg-config description file (disabled by default)
  to query the necessary CFLAGS and LDFLAGS.
  unfortunately like every other homebrewed pkg-config replacement, this config
  utility was designed without cross-compilation in mind, so many packages
  using ncurses and autoconf fail to cross-compile when unpatched.
- Size and build time.

Table 1: Comparison between ncurses and netbsd curses
-----------------------------------------------------
|                             | NCURSES      | NETBSD CURSES |
|:----------------------------|:------------:|:-------------:|
| Size of extracted source    | 15.8 MB      | 3.3 MB        |
| Installed size              | 15.9 MB      | 1.3 MB        |
| Installed size w/debuginfo  | 128.3 MB     | 19.9 MB       |
| Build time (make -j2)       | 59 sec       | 9 sec         |
| size of libncursesw.so      | 346 KB       | 150 KB        |
| size of static linked nano  | 334 KB       | 288 KB        |

  
  when scaling up to more build jobs, it is expected that netbsd curses fares a
  lot better since its Makefile is fully parallelizable, while ncurses spends a
  lot of time in a single process executing the configure script.

Differences from ncurses
------------------------
- the structures used differ. some programs access ncurses structure members directly,
  they must be patched to use a portable approach.
- mouse handling is lacking. ncurses mouse handling routines are provided as no-op
  macros, so software using it builds without having to patch it.
- some rarely used functions like vidputs() are missing. users must be patched.
- the terminfo database is compiled (via tic) into a binary database in CDB format.
- "only" 510 color pairs are supported.
- some functionality like tgetent() lives in libterminfo rather than in libcurses.
  it may be necessary to add -lterminfo to LDFLAGS for packages using those functions.

TODO
----
- installation of global terminfo db
  (rather than just a small built-in, handpicked set)

Compilation
-----------
netbsd-curses ships without a configure script, and requires GNU make.
variables for compilation can be passed directly to make, or be saved into config.mak.
recognized variables are:

- CC - the C compiler
- HOSTCC - the C compiler used for host programs when crosscompiling.
  if set, and different from CC, cross-compilation is assumed.
- CFLAGS - flags for the compiler
- CPPFLAGS - flags for the preprocessor
- LDFLAGS - flags for the linker
- PREFIX - filesystem prefix used for hardcoded paths and installation
- DESTDIR - staging directory for installation

examples:

    make CFLAGS="-Os -Wall -fPIC" PREFIX=/usr/local -j4 all install
    make CFLAGS=-O2 LDFLAGS=-static PREFIX=/usr all-static install-static

example config.mak:

    CFLAGS = -O3 -Wall
    PREFIX = /usr
    DESTDIR = foo

if you're using config.mak, you can just run `make && make install` and the
variables will be picked up automatically.

the `all` and `install` Makefile targets will build/install all programs,
shared and static libs, and headers.
the `all-static` and `install-static` targets will build/install all programs,
static libs, and headers.
the `all-dynamic` and `install-dynamic` targets will build/install all programs
shared libs, and headers.

the `all` build can be sped up by putting `-fPIC` in CFLAGS.
this has the effect that the same object files will be used for the dynamic and
static libs; otherwise they will be compiled twice with different CFLAGS.

Compiled-in terminal database
-----------------------------
support for about a dozen common TERMs is built-in already. if you need one
that's not included, you can either add yours to `libterminfo/genterms` or
`make terminfo/terminfo.cdb` and install the result to either
`$PREFIX/share/terminfo.cdb` or `$HOME/.terminfo.cdb`.

Compiling software against netbsd-curses
----------------------------------------
the functionality that ncurses offers is usually (if not configured to split
into several separate libs) available in a single `libncurses` library.
netbsd-curses on the other hand has it always split into `libcurses` and
`libterminfo`.
this difference requires to give the build system a hint that it needs to
link to both libcurses and libterminfo.

- programs using pkg-config(1) automatically get the right options
  due to the supplied .pc files.

- for autoconf based software (using a configure script) it is usually
  sufficient to invoke configure like this:

    LIBS="-lcurses -lterminfo" ./configure ...

- for Makefile-only based build systems, it should be sufficient to add
  the libs to LDFLAGS:

    make LDFLAGS="-lcurses -lterminfo" ...

with these instructions it is easy to compile the majority of ncurses apps
without problems against netbsd-curses.

a small percentage of apps written for ncurses poke at internals and need
light patching:

- [patch for midnight commander][2] - merged upstream as rev 38d4c655d
- [patch for python 2.7.x and 3.6][3]

if you have trouble compiling a specific package, first look at the
[sabotage linux build recipes][4].
if you still can't get the package to compile, feel free to open an issue
[at the netbsd-curses issue tracker][5].

License
-------
libcurses, tset and tput are (C) 1980-1994
"The Regents of the University of California" licensed under the 3-clause BSD
license.
Everything else is (C) 1999-2016 The NetBSD Foundation, Inc. and Contributors,
licensed under the 2-clause BSD license.
see COPYING for the full text, and in doubt, consult the copyright clauses in
the respective .c files. Files without copyright clauses in the file header
are explicitly released under the terms of the 2-clause BSD license as well.


APPENDIX A: Test Setup used for comparison in Table 1
-----------------------------------------------------
All tests were done on a dual core x86_64 sabotage linux system, with the following
features:
installation of shared and static libs, headers, etc,
i.e. make -j2 all install.
ncurses and netbsd curses were both configured with support for widechars, and
built-in terminfo database for a handful of terminals.

    CFLAGS (optimized for size):
    -g0 -fdata-sections -ffunction-sections -Os -fno-unwind-tables
    -fno-asynchronous-unwind-tables -Wa,--noexecstack

    LDFLAGS:
    -s -Wl,--gc-sections -Wl,-z,relro,-z,now

netbsd curses was installed without manpages (ncurses: 1.1 MB) and terminfo
database (ncurses: 6.4MB).
the debug info build was created with -g3 and debuginfo stripped into external
files via objcopy.

APPENDIX B: History of NetBSD curses
------------------------------------

Thomas Dickey did a [thorough investigation][6] on the history of NetBSD curses.


[0]:http://trac.sagemath.org/ticket/18301
[1]:https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/log/scripts/kconfig/lxdialog/check-lxdialog.sh
[2]:https://github.com/MidnightCommander/mc/commit/38d4c655d322837574e957b4a824f4a0d1bb3b86
[3]:https://github.com/sabotage-linux/sabotage/blob/06a4a815/KEEP/python2710-curses.patch
[4]:https://github.com/sabotage-linux/sabotage/tree/master/pkg
[5]:https://github.com/sabotage-linux/netbsd-curses/issues
[6]:http://invisible-island.net/ncurses/ncurses-netbsd.html

