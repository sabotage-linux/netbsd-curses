/*	$NetBSD: initscr.c,v 1.35 2020/03/12 15:50:11 roy Exp $	*/

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

#include <stdlib.h>

#include "curses.h"
#include "curses_private.h"


/*
 * initscr --
 *	Initialize the current and standard screen.
 */
WINDOW *
initscr(void)
{
	const char *sp;

#ifdef DEBUG
	__CTRACE(__CTRACE_INIT, "initscr\n");
#endif

	/*
	 * If My_term is set, or can't find a terminal in the environment,
	 * use Def_term.
	 */
	if (My_term || (sp = getenv("TERM")) == NULL)
		sp = Def_term;

	/* LINTED const castaway; newterm does not modify sp! */
	if ((_cursesi_screen = newterm((char *) sp, stdout, stdin)) == NULL) {
		/*
		 * POSIX says we should write a diagnostic and exit on error.
		 * As such some applications don't bother checking the return
		 * value at all.
		 */
		perror("initscr");
		exit(EXIT_FAILURE);
	}

	set_term(_cursesi_screen);
	wrefresh(curscr);
	__ripofftouch(_cursesi_screen);

	return stdscr;
}
