#include "data.h"

Data *
_newData()
{
	Data * data = (Data *) malloc(sizeof(Data));
	data->data = (Letter *) malloc(BASE_DATA_LENGTH * sizeof(Letter));
	data->data_length = 0;
	data->extra_data = (Letter *) malloc(BASE_DATA_LENGTH * sizeof(Letter));
	data->extra_data_length = 0;
	return data;
}

void
_storeData(Data * data, Letter * current, int * current_size)
{
	(*current)[*current_size] = '\0';
	data->data[data->data_length++] = *current;
}

void
_nextData(Data * data, Letter * current, int * current_size)
{
	if (*current_size == 0)
		return;
	_storeData(data, current, current_size);
	*current_size = 0;
	*current = (Letter) malloc(BASE_LETTER_SIZE * sizeof(char));
	if ((data->data_length % BASE_DATA_LENGTH) == 0)
		data->data = realloc(data->data, (data->data_length + BASE_DATA_LENGTH) * sizeof(Letter));
}

bool
_handleData(char c, FILE * dataFile, Data * data, Letter * current, int * current_size)
{
	switch (c)
	{
	case '$':
		skipLine(dataFile);
	case '\n':
	case '\t':
	case ' ':
		_nextData(data, current, current_size);
		return false;
	case '#':
		if (current_size != 0)
			_storeData(data, current, current_size);
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
_extractData(Data * data, FILE * dataFile)
{
	char c;
	Letter current = (Letter) malloc((1 + BASE_LETTER_SIZE) * sizeof(char));
	int current_size = 0;
	while ((c = fgetc(dataFile)) != EOF)
	{
		if (_handleData(c, dataFile, data, &current, &current_size))
			break;
	}
}

Data *
newData()
{
	FILE * dataFile = NULL;
	char dataFilename[MAX_FILENAME_LENGTH];

	printf("Where is the file containing the data for your machine ?\n");
	while (!scanf("%s", dataFilename) || (dataFile = fopen(dataFilename, "r")) == NULL)
		printf("Failed to read file: %s, please enter a new one.\n", dataFilename);

	Data * data = _newData();
	_extractData(data, dataFile);

	fclose(dataFile);
	return data;
}

void
freeData(Data * data)
{
	int i;
	for (i = 0 ; i < data->data_length ; ++i)
		free(data->data[i]);
	free(data->data);
	free(data->extra_data);
	free(data);
}
