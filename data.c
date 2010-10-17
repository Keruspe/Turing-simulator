#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Data *
_newData()
{
	Data * data = (Data *) malloc(sizeof(Data));
	data->data = (Letter *) malloc(DATA_BASE_LENGTH * sizeof(Letter));
	data->data_length = 0;
	return data;
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

	/* TODO */

	fclose(dataFile);
	return data;
}

void
freeData(Data * data)
{
	free(data->data);
	free(data);
}
