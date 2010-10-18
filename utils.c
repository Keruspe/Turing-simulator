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

char *
_readElement(FILE * file)
{
	char c;
	char * element = (char *) malloc((1 + BASE_ELEMENT_SIZE) * sizeof(char *));
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
			element[element_size] = '\0';
			return element;
		default:
			element[element_size] = c;
			if ((++element_size % BASE_ELEMENT_SIZE) == 0)
				element = realloc(element, (element_size + BASE_ELEMENT_SIZE) * sizeof(char *));
		}
	}
	return NULL;
}

void
_storeElement(char ** storage, int * storage_length, char ** element, int * element_size)
{
	(*element)[*element_size] = '\0';
	storage[(*storage_length)++] = *element;
}

void
_nextElement(char *** storage, int * storage_length, char ** element, int * element_size)
{
	if (*element_size == 0)
		return;
	_storeElement(*storage, storage_length, element, element_size);
	*element_size = 0;
	*element = (char *) malloc(BASE_ELEMENT_SIZE * sizeof(char));
	if ((*storage_length % BASE_ALPHABET_LENGTH) == 0)
		*storage = realloc(*storage, (*storage_length + BASE_ALPHABET_LENGTH) * sizeof(char**));
}

bool
_handleData(char c, FILE * file, char *** storage, int * storage_length, char ** element, int * element_size)
{
	switch (c)
	{
	case '$':
		skipLine(file);
	case '\n':
	case '\t':
	case ' ':
		_nextElement(storage, storage_length, element, element_size);
		return false;
	case '#':
		if (*element_size != 0)
			_storeElement(*storage, storage_length, element, element_size);
		else
			free(*element);
		return true;
	default:
		(*element)[*element_size] = c;
		if ((++(*element_size) % BASE_ELEMENT_SIZE) == 0)
			*element = realloc(*element, (*element_size + BASE_ELEMENT_SIZE) * sizeof(char));
		return false;
	}
}

void
_extractData(FILE * file, char *** storage, int * storage_length)
{
	char c;
	Letter element = (Letter) malloc((1 + BASE_ELEMENT_SIZE) * sizeof(char));
	int element_size = 0;
	while ((c = fgetc(file)) != EOF)
	{
		if (_handleData(c, file, storage, storage_length, &element, &element_size))
			return;
	}
}
