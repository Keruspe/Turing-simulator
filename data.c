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

Data *
newData()
{
	FILE * dataFile = NULL;
	char dataFilename[MAX_FILENAME_LENGTH];

	printf("Where is the file containing the data for your machine ?\n");
	while (!scanf("%s", dataFilename) || (dataFile = fopen(dataFilename, "r")) == NULL)
		printf("Failed to read file: %s, please enter a new one.\n", dataFilename);

	Data * data = _newData();
	_extractData(&(data->data), &(data->data_length), dataFile);

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
