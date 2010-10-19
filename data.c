#include "data.h"

Data *
_newData()
{
	/* Basic allocation and initialisation stuff to get a ready base Data */
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
	/* Read the name of the file in which the Data are stored, don't fail if we cannot, just ask for another one */
	while (!scanf("%s", dataFilename) || (dataFile = fopen(dataFilename, "r")) == NULL)
		printf("Failed to read file: %s, please enter a new one.\n", dataFilename);

	Data * data = _newData(); /* Allocate memory */
	/* Extract Data from dataFile */
	_extractData(dataFile, &(data->data), &(data->data_length));

	/* Close the dataFile */
	fclose(dataFile);
	return data;
}

void
freeData(Data * data)
{
	/* Free the memory used by a Data set */
	int i; /* Counter */
	for (i = 0 ; i < data->data_length ; ++i)
		free(data->data[i]); /* Free each Letter from data */
	/* Free remaining stuff */
	free(data->data);
	free(data->extra_data);
	free(data);
}
