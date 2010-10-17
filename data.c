#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Data *
_newData()
{
	Data * data = (Data *) malloc(sizeof(Data));
	data->data = (Letter *) malloc(DATA_BASE_LENGTH * sizeof(Letter));
	data->data_length = 0;
	data->extra_data = (Letter *) malloc(DATA_BASE_LENGTH * sizeof(Letter));
	data->extra_data_length = 0;
	return data;
}

void
extractData(Data * data, FILE * dataFile)
{
	char c;
	int current_size = 0;
	while ((c = fgetc(dataFile)) != EOF)
	{
		if (c == '$' || c == '\n' || c == '\t' || c == ' ')
		{
			if (current_size != 0)
			{
				data->data[data->data_length][current_size] = '\0';
				current_size = 0;
				if ((++(data->data_length) % DATA_BASE_LENGTH) == 0)
					data->data = realloc(data->data, (data->data_length + DATA_BASE_LENGTH) * sizeof(Letter));
			}
			if (c == '$')
				while ((c = fgetc(dataFile)) != '\n');
			continue;
		}
		if (c == '#')
			break;
		data->data[data->data_length][current_size] = c;
		if (current_size++ == MAX_LETTER_SIZE)
		{
			data->data[data->data_length][current_size] = '\0';
			current_size = 0;
			if ((++(data->data_length) % DATA_BASE_LENGTH) == 0)
				data->data = realloc(data->data, (data->data_length + DATA_BASE_LENGTH) * sizeof(Letter));
		}
	}
}

Data *
newData()
{
	FILE * dataFile = NULL;
	char dataFilename[MAX_FILENAME_LENGTH];

	printf("Where is the file containing the data for your machine ?\n");
	if (!scanf("%s", dataFilename) || (dataFile = fopen(dataFilename, "r")) == NULL)
	{
		printf("Failed to read file: %s, exiting...\n", dataFilename);
		return NULL;
	}

	Data * data = _newData();
	extractData(data, dataFile);

	fclose(dataFile);
	return data;
}

void
freeData(Data * data)
{
	free(data->data);
	free(data->extra_data);
	free(data);
}
