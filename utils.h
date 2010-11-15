/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
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

/* Avoid infinite loop */
#define MAX_STEPS 999

#define DEFAULT_LETTER "_"

/* Skip the rest of the line we're reading (for comments) */
void skipLine(FILE *, unsigned int * line_number);

/* Clear the buffer after reading an entry */
void clearBuffer();

/* Used to get user choice in main */
bool getChoice();

/**
 * parsing stuff
 */

/* Read the next element in the file */
bool readElement(FILE *, Element *, unsigned int * line_number);

/**
 * Extract data section (until we meet '#') and put all elements into the storage area we pass a pointer to,
 * keeping by the way its length up to date.
 * The storage is a pointer to an Array of Element (State/Letter).
 * We need this and not only the Array because we want to be able to resize it.
 */
void extractData(FILE * file, ElementsCollection * storage, int * storage_length, unsigned int * line_number);

/* Get the size of an integer */
int getUnsignedIntegerLength(unsigned int);

#endif /* __UTILS_H__ */
