netbsd-libcurses portable edition
=================================

this is a port of netbsd's curses library for usage on Linux systems
(tested and developed on sabotage linux, based on musl libc).
It should be possible to use on other POSIX platforms as well with minor
changes.

it supports widechar (like the ncursesw build variant) and is close to being
a 100% feature-complete drop-in replacement for ncurses.

to this date, there are only 3 implementation of the curses API:

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
  utility was designed without cross-compilation in mind, so almost any package
  using ncurses and autoconf fails to cross-compile when unpatched.
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
- manpage installation
- installation of global terminfo db
  (rather than just a small built-in, handpicked set)

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

[0]:http://trac.sagemath.org/ticket/18301
[1]:https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/log/scripts/kconfig/lxdialog/check-lxdialog.sh
