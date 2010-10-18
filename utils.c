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

void
_storeData(char ** storage, int * storage_length, char ** current, int * current_size)
{
	(*current)[*current_size] = '\0';
	storage[(*storage_length)++] = *current;
}

void
_nextData(char *** storage, int * storage_length, char ** current, int * current_size)
{
	if (*current_size == 0)
		return;
	_storeData(*storage, storage_length, current, current_size);
	*current_size = 0;
	*current = (char *) malloc(BASE_LETTER_SIZE * sizeof(char));
	if ((*storage_length % BASE_ALPHABET_LENGTH) == 0)
		*storage = realloc(*storage, (*storage_length + BASE_ALPHABET_LENGTH) * sizeof(char**));
}

bool
_handleData(char c, FILE * file, char *** storage, int * storage_length, char ** current, int * current_size)
{
	switch (c)
	{
	case '$':
		skipLine(file);
	case '\n':
	case '\t':
	case ' ':
		_nextData(storage, storage_length, current, current_size);
		return false;
	case '#':
		if (*current_size != 0)
			_storeData(*storage, storage_length, current, current_size);
		else
			free(*current);
		return true;
	default:
		(*current)[*current_size] = c;
		if ((++(*current_size) % BASE_LETTER_SIZE) == 0)
			*current = realloc(*current, (*current_size + BASE_LETTER_SIZE) * sizeof(char));
		return false;
	}
}

void
_extractData(char *** storage, int * storage_length, FILE * dataFile)
{
	char c;
	Letter current = (Letter) malloc((1 + BASE_LETTER_SIZE) * sizeof(char));
	int current_size = 0;
	while ((c = fgetc(dataFile)) != EOF)
	{
		if (_handleData(c, dataFile, storage, storage_length, &current, &current_size))
			return;
	}
}
