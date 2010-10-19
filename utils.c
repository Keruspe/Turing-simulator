#include "utils.h"

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

Element
_readElement(FILE * file)
{
	char c; /* Will store each character we read */
	Element element; /* The element we're reading */
	/* Assume we've not reach the end yet */
	element.endOfElements = false;
	/* Allocate memory */
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
			if ((++element_size % BASE_ELEMENT_SIZE) == 0) /* Increase the element size, if it's full, increase its size */
				element.element = (char *) realloc(element.element, (element_size + BASE_ELEMENT_SIZE) * sizeof(char));
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
	while (! ((element = _readElement(file)).endOfElements)) /* While there are still data to be read, read them (until next '#') */
	{
		(*storage)[*storage_length] = element.element; /* Store the element in the storage area */
		 if ((++(*storage_length) % BASE_ALPHABET_LENGTH) == 0) /* Increase the number of elements in the storage area, and increase its size if it's full */
			*storage = (char **) realloc(*storage, (*storage_length + BASE_ALPHABET_LENGTH) * sizeof(char *));
	}
	if (element.element[0] != '\0') /* If a last element was stuck to the '#' */
		(*storage)[(*storage_length)++] = element.element; /* Store it and increase number of elements into the storage area */
	else
		free(element.element); /* Else free the dummy element to avoid memory leaks */
}
