#ifndef EMALLOC_H
#define EMALLOC_H

/* replacement functions for emalloc() and friends used in netbsd code */

#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static void mallerr(void) {
	dprintf(2, "error: out of memory\n");
	abort();
}
static inline void* emalloc(size_t x) {
	void *p=malloc(x);
	if(!p) mallerr();
	return p;
}
static inline void* ecalloc(size_t x, size_t y) {
	void *p=calloc(x, y);
	if(!p) mallerr();
	return p;
}
static inline void* erealloc(void *x, size_t y) {
	void *p=realloc(x, y);
	if(!p) mallerr();
	return p;
}
static inline char* estrdup(const char* x) {
	char *p = strdup(x);
	if(!p) mallerr();
	return p;
}
static int easprintf(char **s, const char *fmt, ...) {
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vasprintf(s, fmt, ap);
	va_end(ap);
	if(ret == -1) mallerr();
	return ret;
}

#endif

