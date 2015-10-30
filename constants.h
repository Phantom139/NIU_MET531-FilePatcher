#ifndef CONSTANTS_H
#define CONSTANTS_H

static struct constants {
	// --- Constants
	static int dim[12];
	static char *mon[12];
};

int constants::dim[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
char *constants::mon[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

#endif