/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "utils.h"
#include "machine.h"

void
skipLine(FILE * file)
{
	/* Read every char until next line in file */
	while (fgetc(file) != '\n');
}

void
clearBuffer()
{
	/* Read every char until 'enter' from standard input */
	while (getchar() != '\n');
}

void
_Exception(char * reason)
{
	fprintf(stderr, "Exception: %s\n", reason);
	free(reason);
	fprintf(stderr, "Exiting...\n");
	exit(EXIT_FAILURE);
}

void
NoSuchFileException(const char * filename)
{
	char * full_reason = (char *) malloc((22 + strlen(filename)) * sizeof(char));
	sprintf(full_reason, "failed to read file: %s", filename);
	_Exception(full_reason);
}

void
MalformedFileException(Machine * machine, FILE * file, const char * reason)
{
	if (machine)
		freeMachine(machine);
	if (file)
		fclose(file);
	char * full_reason = (char *) malloc ((17 + strlen(reason)) * sizeof(char));
	sprintf(full_reason, "malformed file, %s", reason);
	_Exception(full_reason);
}

void
_BadTransitionException(Machine * machine, FILE * file)
{
	MalformedFileException(machine, file, "a malformed transition has been found.\nExpected: State Letter Letter State Move");
}

void
BadTransitionException(Machine * machine, FILE * file, const char * reason)
{
	if (reason == NULL)
		_BadTransitionException(machine, file);
	else
		MalformedFileException(machine, file, reason);
}

Element
_readElement(FILE * file)
{
	char c; /* Will store each character we read */
	Element element; /* The element we're reading */
	/* Initialize the element */
	element.endOfElements = false;
	element.element = (char *) malloc((1 + BASE_ELEMENT_SIZE) * sizeof(char));
	int element_size = 0; /* Size of the element */
	while ((c = fgetc(file)) != EOF) /* While we're not at the end of file */
	{
		switch (c) /* Deal with the character we just read */
		{
		case '$': /* We're facing a comment, ignore the end of line */
			skipLine(file);
			break;
		case ' ':
		case '\t':
		case '\n':
			if (element_size == 0)
				break; /* We're reading a chain of blank stuff, go to next char */
			/* We just finished to read an element, close the string and return it */
			element.element[element_size] = '\0';
			return element;
		case '#':
			/* We reached the end of this data section, set endOfElements to true, close string and return element */
			element.endOfElements = true;
			element.element[element_size] = '\0';
			return element;
		default:
			/* We read a basic character, nothing special to do here, juste append it to the end of the element */
			element.element[element_size] = c;
			/* Increase the element size, if it's full, increase its size */
			if ((++element_size % BASE_ELEMENT_SIZE) == 0)
				element.element = (char *) realloc(element.element, (element_size + 1 + BASE_ELEMENT_SIZE) * sizeof(char));
		}
	}
	/* We only get here if there was no '#', fallback considerating that the end of file is a '#' */
	element.endOfElements = true;
	element.element[element_size] = '\0';
	return element;
}

void
_extractData(FILE * file, char *** storage, int * storage_length)
{
	Element element; /* Will store each element we read */
	/* While there are still data to be read, read them (until next '#') */
	while (! ((element = _readElement(file)).endOfElements))
	{
		(*storage)[*storage_length] = element.element; /* Store the element in the storage area */
		/* Increase the number of elements in the storage area, and increase its size if it's full */
		if ((++(*storage_length) % BASE_ALPHABET_LENGTH) == 0)
			*storage = (char **) realloc(*storage, (*storage_length + BASE_ALPHABET_LENGTH) * sizeof(char *));
	}
	if (element.element[0] != '\0') /* If a last element was stuck to the '#' */
		/* Store it and increase number of elements into the storage area */
		(*storage)[(*storage_length)++] = element.element;
	else
		free(element.element); /* Else free the dummy element to avoid memory leaks */
}
