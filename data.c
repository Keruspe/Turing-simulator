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
newData(char * filename)
{
	FILE * dataFile = fopen(filename, "r");
	if (dataFile == NULL)
		return NULL;

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
