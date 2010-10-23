/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Define a few parameters */
#define MAX_FILENAME_LENGTH 100
#define BASE_ELEMENT_SIZE 10
#define BASE_STORAGE_LENGTH 10
#define BASE_TRANSITIONS_LENGTH 25
#define BASE_DATA_LENGTH 5*BASE_STORAGE_LENGTH

/* Macro to create a default Letter */
#define DEFAULT_LETTER(letter) \
	Letter (letter) = (Letter) malloc(2 * sizeof(char)); \
	letter[0] = '_'; \
	letter[1] = '\0'

/* Avoid infinite loop */
#define MAX_STEPS 99999

/* Skip the rest of the line we're reading (for comments) */
void skipLine(FILE *);
/* Clear the buffer after reading an entry */
void clearBuffer();
/* Tool to rewrite steps count over previous */
void eraseSteps();

/*
 * parsing stuff
 */

/* Read the next element in file */
bool readElement(FILE * file, Element *);

/*
 * Extract data section (until we meet '#') and put all elements into the storage area we pass a pointer to,
 * keeping by the way its length up to date.
 * The storage is a pointer to an Array of Element (State/Letter).
 * We need this and not only the Array because we want to be able to resize it.
 */
void extractData(FILE * file, ElementsCollection * storage, int * storage_length);

void debug(Machine *);

#endif /* __UTILS_H__ */
