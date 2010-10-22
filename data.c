/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"

Data *
_newData()
{
	/* Basic allocation and initialisation stuff to get a ready base Data */
	Data * data = (Data *) malloc(sizeof(Data));
	data->data = (LettersCollection) malloc(BASE_DATA_LENGTH * sizeof(Letter));
	data->data_length = 0;
	data->extra_data = (LettersCollection) malloc(BASE_DATA_LENGTH * sizeof(Letter));
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
	extractData(dataFile, &(data->data), &(data->data_length));

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

Letter
getLetter(Data * data, int index)
{
	if (index >= 0) /* Positive index -> data */
	{
		if (index >= data->data_length) /* If we didn't reach that point yet */
		{
			if (((data->data_length)++ % BASE_STORAGE_LENGTH) == 0) /* If full, increase size, then increment the length */
				data->data = (ElementsCollection) realloc(data->data, (data->data_length + BASE_STORAGE_LENGTH) * sizeof(Element));
			DEFAULT_LETTER(letter); /* Macro to create a default Letter named letter */
			data->data[index] = letter;
		}
		return data->data[index];
	}
	else /* Negative index -> extra_data */
	{
		if (index < data->extra_data_length) /* If we didn't reach that point yet */
		{
			if (((data->extra_data_length)++ % BASE_STORAGE_LENGTH) == 0) /* If full, increase size, then increment the length */
				data->extra_data = (ElementsCollection) realloc(data->extra_data, (data->extra_data_length + BASE_STORAGE_LENGTH) * sizeof(Element));
			DEFAULT_LETTER(letter); /* Macro to create a default Letter named letter */
			data->extra_data[index + 1] = letter;
		}
		return data->extra_data[index + 1]; /* Return the Letter we reached */
	}
}
