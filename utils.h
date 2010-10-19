#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* Define a few parameters */
#define MAX_FILENAME_LENGTH 100
#define BASE_ELEMENT_SIZE 10
#define BASE_ALPHABET_LENGTH 10
#define BASE_TRANSITIONS_LENGTH 25
#define BASE_DATA_LENGTH 50

/* Skip the rest of the line we're reading (for comments) */
void skipLine(FILE *);
/* Clear the buffer after reading an entry */
void clearBuffer();

/*
 * parsing stuff
 */

/* A structure designed to be the result of an element extraction */
struct _Element
{
	/* The element we read (State or Letter) */
	char * element;
	/* Did we met a '#' (aka end of data section) ? */
	bool endOfElements;
};

/* Read the next element in file */
Element _readElement(FILE * file);

/*
 * Extract data section (until we meet '#') and put all elements into the
 * storage area we pass a pointer to, keeping its length up to date.
 */
void _extractData(FILE * file, char *** storage, int * storage_length);

#endif /* __UTILS_H__ */
