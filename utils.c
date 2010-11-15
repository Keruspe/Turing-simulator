/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "types.h"
#include "utils.h"

void
skipLine(FILE * file, unsigned int * line_number)
{
	/* Read every char until next line in file */
	while (fgetc(file) != '\n');
	++(*line_number);
}

void
clearBuffer()
{
	/* Read every char until 'enter' from standard input */
	while (getchar() != '\n');
}

bool
getChoice()
{
	/* Get user choice (Y/n) */
	bool answer = true; /* Default to yes */
	char c;
	if ((c = getchar()) == 'n')
		answer = false; /* User answered no */
	if (c != '\n') /* Default doesn't need any value */
		clearBuffer();
	return answer;
}

bool
readElement(FILE * file, Element * element, unsigned int * line_number)
{
	char c; /* Will store each character we read */
	*element = (Element) malloc((1 + BASE_ELEMENT_SIZE) * sizeof(char)); /* Initialization/allocation */
	int element_size = 0; /* Size of the element */
	while ((c = fgetc(file)) != EOF) /* While we're not at the end of file */
	{
		switch (c) /* Deal with the character we just read */
		{
		case '$': /* We're facing a comment, ignore the end of line */
			skipLine(file, line_number);
		case '\0': /* Ignore \0 if by chance we meet it */
			break;
		case '\n':
			++(*line_number);
		case ' ':
		case '\t':
			if (element_size == 0)
				break; /* We're reading a chain of blank stuff, go to next char */
			/* We just finished to read an element, close the string and return true */
			(*element)[element_size] = '\0';
			return true;
		case '#':
			/* We reached the end of this data section, close string or set to # and return false */
			(*element)[element_size] = (element_size == 0) ? '#' : '\0';
			return false;
		default:
			/* We read a basic character, nothing special to do here, juste append it to the end of the element */
			(*element)[element_size] = c;
			/* Increase the element size, if it's full, increase its size */
			if ((++element_size % BASE_ELEMENT_SIZE) == 0)
				*element = (Element) realloc(*element, (element_size + 1 + BASE_ELEMENT_SIZE) * sizeof(char));
		}
	}
	/**
	 * We only get here if there was no '#'
	 * Fallback considerating that the end of file is a '#' but don't return '#'
	 */
	(*element)[element_size] = '\0';
	return false;
}

void
extractData(FILE * file, ElementsCollection * storage, int * storage_length, unsigned int * line_number)
{
	Element element; /* Will store each element we read */
	/* While there are still data to be read, read them (until next '#') */
	while (readElement(file, &element, line_number))
	{
		(*storage)[*storage_length] = element; /* Store the element in the storage area */
		/* Increase the number of elements in the storage area, and increase its size if it's full */
		if ((++(*storage_length) % BASE_STORAGE_LENGTH) == 0)
		{
			*storage = (ElementsCollection) realloc(*storage,
				(*storage_length + BASE_STORAGE_LENGTH) * sizeof(Element));
		}
	}
	if (element[0] != '#' && element[0] != '\0') /* If a last element was stuck to the '#' or to the end of file */
		/* Store it and increase number of elements into the storage area */
		(*storage)[(*storage_length)++] = element;
	else
		free(element); /* Else free the dummy element to avoid memory leaks */
}

int
getUnsignedIntegerLength(unsigned int integer)
{
	int size;
	for (size = 1 ; (integer/=10) != 0 ; ++size);
	return size;
}
