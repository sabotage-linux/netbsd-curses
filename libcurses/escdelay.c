#include "curses.h"

int set_escdelay(int value) {
	ESCDELAY = value;
	return OK;
}

