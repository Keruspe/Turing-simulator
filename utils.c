#include "utils.h"

void
skipLine(FILE * file)
{
	while (fgetc(file) != '\n');
}

void
clearBuffer()
{
	while (getchar() != '\n');
}

Element
_readElement(FILE * file)
{
	char c;
	Element element;
	element.endOfElements = false;
	element.element = (char *) malloc((1 + BASE_ELEMENT_SIZE) * sizeof(char *));
	int element_size = 0;
	while ((c = fgetc(file)) != EOF)
	{
		switch (c)
		{
		case '$':
			skipLine(file);
			break;
		case ' ':
		case '\t':
		case '\n':
			if (element_size == 0)
				break;
			element.element[element_size] = '\0';
			return element;
		case '#':
			element.endOfElements = true;
			element.element[element_size] = '\0';
			return element;
		default:
			element.element[element_size] = c;
			if ((++element_size % BASE_ELEMENT_SIZE) == 0)
				element.element = realloc(element.element, (element_size + BASE_ELEMENT_SIZE) * sizeof(char *));
		}
	}
	element.endOfElements = true;
	element.element[0] = '\0';
	return element;
}

void
_extractData(FILE * file, char *** storage, int * storage_length)
{
	Element element;
	while (! ((element = _readElement(file)).endOfElements))
	{
		(*storage)[*storage_length] = element.element;
		 if ((++(*storage_length) % BASE_ALPHABET_LENGTH) == 0)
			*storage = realloc(*storage, (*storage_length + BASE_ALPHABET_LENGTH) * sizeof(char**));
	}
	if (element.element[0] != '\0')
		(*storage)[(*storage_length)++] = element.element;
	else
		free(element.element);
}
