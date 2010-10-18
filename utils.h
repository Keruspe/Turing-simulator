#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 100

#define BASE_ELEMENT_SIZE 10

#define BASE_ALPHABET_LENGTH 10
#define BASE_TRANSITIONS_LENGTH 25
#define BASE_DATA_LENGTH 50

void skipLine(FILE *);
void clearBuffer();

/* parsing stuff */
struct _Element
{
	char * element;
	bool endOfElements;
};
Element _readElement(FILE * file);
void _storeElement(char **, int *, char **, int *);
void _nextElement(char ***, int *, char **, int *);
bool _handleData(char, FILE *, char ***, int *, char **, int *);
void _extractData(FILE *, char ***, int *);

#endif /* __UTILS_H__ */
