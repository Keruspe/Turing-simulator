#ifndef __UTILS_H__
#define __UTILS_H__

#include "types.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_FILENAME_LENGTH 100

#define BASE_LETTER_SIZE 10

#define BASE_ALPHABET_LENGTH 10
#define BASE_TRANSITIONS_LENGTH 25
#define BASE_DATA_LENGTH 50

void skipLine(FILE *);
void clearBuffer();

/* parsing stuff */
void _storeData(char **, int *, char **, int *);
void _nextData(char ***, int *, char **, int *);
bool _handleData(char, FILE *, char ***, int *, char **, int *);
void _extractData(char ***, int *, FILE *);

#endif /* __UTILS_H__ */
