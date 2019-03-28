/*	$NetBSD: printw.c,v 1.26 2019/03/28 23:24:22 uwe Exp $	*/

/*
 * Copyright (c) 1981, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <netbsd_sys/cdefs.h>

#include <stdarg.h>

#include "curses.h"
#include "curses_private.h"

#if 0
#warning "funopen unimplemented!"
static inline void* funopen(void* a, ...) {
	dprintf(666, "warning: funopen unimplemented\n");
	return 0;
}
#endif

/*
 * printw and friends.
 */

/*
 * printw --
 *	Printf on the standard screen.
 */
int
printw(const char *fmt,...)
{
	va_list ap;
	int     ret;

	va_start(ap, fmt);
	ret = vw_printw(stdscr, fmt, ap);
	va_end(ap);
	return ret;
}
/*
 * wprintw --
 *	Printf on the given window.
 */
int
wprintw(WINDOW *win, const char *fmt,...)
{
	va_list ap;
	int     ret;

	va_start(ap, fmt);
	ret = vw_printw(win, fmt, ap);
	va_end(ap);
	return ret;
}
/*
 * mvprintw, mvwprintw --
 *	Implement the mvprintw commands.  Due to the variable number of
 *	arguments, they cannot be macros.  Sigh....
 */
int
mvprintw(int y, int x, const char *fmt,...)
{
	va_list ap;
	int     ret;

	if (move(y, x) != OK)
		return ERR;
	va_start(ap, fmt);
	ret = vw_printw(stdscr, fmt, ap);
	va_end(ap);
	return ret;
}

int
mvwprintw(WINDOW * win, int y, int x, const char *fmt,...)
{
	va_list ap;
	int     ret;

	if (wmove(win, y, x) != OK)
		return ERR;

	va_start(ap, fmt);
	ret = vw_printw(win, fmt, ap);
	va_end(ap);
	return ret;
}
#if 0
/*
 * Internal write-buffer-to-window function.
 */
static ssize_t
winwrite(void *cookie, const void *vbuf, size_t n)
{
	WINDOW *win = cookie;
	const char *buf = vbuf;
	int status;

	status = waddnstr(win, buf, n);
	if (status == ERR)
	    return -1;

	return (ssize_t)n;
}
#endif
/*
 * vw_printw --
 *	This routine actually executes the printf and adds it to the window.
 */
int
vw_printw(WINDOW *win, const char *fmt, va_list ap)
{
#if 0
	if (win->fp == NULL) {
		win->fp = funopen2(win, NULL, winwrite, NULL, NULL, NULL);
		if (win->fp == NULL)
			return ERR;
	}
	vfprintf(win->fp, fmt, ap);
	fflush(win->fp);
	return OK;
#else
	char buf[4096], *p = buf;
	int c, ret = vsnprintf(buf, sizeof buf, fmt, ap);
	if(ret < 0 || ret >= sizeof buf) return ERR;
	for (c = ret; --c >= 0;)
		if (waddch(win, (chtype) (*p++ & __CHARTEXT)) == ERR)
                        return ERR;
	return OK;
#endif
}

__strong_alias(vwprintw, vw_printw)

