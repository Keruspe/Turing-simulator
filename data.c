#include "data.h"
#include <stdio.h>
#include <stdlib.h>

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
nextData(Data * data, Letter * current, int * current_size)
{
	if (*current_size == 0)
		return;
	(*current)[*current_size] = '\0';
	data->data[data->data_length] = *current;
	*current_size = 0;
	*current = (Letter) malloc(BASE_LETTER_SIZE * sizeof(char));
	if ((++(data->data_length) % BASE_DATA_LENGTH) == 0)
		data->data = realloc(data->data, (data->data_length + BASE_DATA_LENGTH) * sizeof(Letter));
}

void
extractData(Data * data, FILE * dataFile)
{
	char c;
	Letter current = (Letter) malloc((1 + BASE_LETTER_SIZE) * sizeof(char));
	int current_size = 0;
	while ((c = fgetc(dataFile)) != EOF)
	{
		if (c == '\n' || c == '\t' || c == ' ')
		{
			nextData(data, &current, &current_size);
			continue;
		}
		if (c == '$')
		{
			nextData(data, &current, &current_size);
			while (fgetc(dataFile) != '\n');
			continue;
		}
		if (c == '#')
		{
			nextData(data, &current, &current_size);
			free(current);
			break;
		}
		current[current_size] = c;
		if ((++current_size % BASE_LETTER_SIZE) == 0)
			current = realloc(current, (current_size + BASE_LETTER_SIZE) * sizeof(char));
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
	extractData(data, dataFile);

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
