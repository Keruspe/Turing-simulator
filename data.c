/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"
#include "validator.h"
#include <string.h>

Data *
_newData()
{
	/* Basic allocation and initialisation stuff to get a ready base Data */
	Data * data = (Data *) malloc(sizeof(Data));
	data->data = (LettersCollection) malloc(BASE_DATA_LENGTH * sizeof(Letter));
	data->data_length = 0;
	data->extra_data = NULL;
	data->extra_data_length = 0;
	return data;
}

Data *
newData()
{
	FILE * data_file = NULL;
	char data_filename[MAX_FILENAME_LENGTH];
	int line_number = 0;

	printf("Where is the file containing the data for your machine ?\n");
	/* Read the name of the file in which the Data are stored, don't fail if we cannot, just ask for another one */
	while (!scanf("%s", data_filename) || (data_file = fopen(data_filename, "r")) == NULL)
		printf("Failed to read file: %s, please enter a new one.\n", data_filename);

	Data * data = _newData(); /* Allocate memory */
	/* Extract Data from data_file */
	extractData(data_file, &(data->data), &(data->data_length), &line_number);

	/* Close the data_file */
	fclose(data_file);
	return data;
}

void
freeData(Data * data)
{
	/* Free the memory used by a Data set */
	int count;
	for (count = 0 ; count < data->data_length ; ++count)
		free(data->data[count]); /* Free each Letter from data */
	/* Free remaining stuff */
	free(data->data);
	if (data->extra_data)
	{
		for (count = 0 ; count < data->extra_data_length ; ++count)
			free(data->extra_data[count]); /* Free each Letter from extra_data */
		free(data->extra_data);
	}
	free(data);
}

Letter
getLetter(Machine * machine)
{
	/* Local variables for readability */
	int index = machine->data_index;
	Data * data = machine->data;

	if (index >= 0) /* Positive index -> data */
	{
		if (index >= data->data_length) /* If we didn't reach that point yet */
		{
			/* If full, increase size, then increment the length */
			if (((data->data_length)++ % BASE_STORAGE_LENGTH) == 0)
			{
				data->data = (LettersCollection) realloc(data->data,
					(data->data_length + BASE_STORAGE_LENGTH) * sizeof(Letter));
			}
			data->data[index] = NULL; /* Initialize to NULL so that memory will get allocated */
			if (hasDefaultLetter(machine))
				setLetter(data, index, DEFAULT_LETTER); /* Set to default Letter */
			else
				DefaultLetterException(machine);
		}
		return data->data[index];
	}
	else /* Negative index -> extra_data */
	{
		if (index < data->extra_data_length) /* If we didn't reach that point yet */
		{
			if (data->extra_data_length == 0) /* If we don't have extra data yet, allocate memory */
				data->extra_data = (LettersCollection) malloc(BASE_DATA_LENGTH * sizeof(Letter));
			/* If full, increase size, then increment the length */
			else if ((data->extra_data_length % BASE_STORAGE_LENGTH) == 0)
			{
				data->extra_data = (LettersCollection) realloc(data->extra_data,
					(data->extra_data_length + BASE_STORAGE_LENGTH) * sizeof(Letter));
			}
			++(data->extra_data_length);
			/* See in setLetter (right behind) for an explanation of the "-index-1" */
			data->extra_data[-index-1] = NULL; /* Initialize to NULL so that memory will get allocated */
			if (hasDefaultLetter(machine))
				setLetter(data, index, DEFAULT_LETTER); /* Set to default Letter */
			else
				DefaultLetterException(machine);
		}
		return data->extra_data[-index-1]; /* Return the Letter we reached */
	}
}

void
setLetter(Data * data, int index, Letter letter)
{
	if (index >= 0) /* Positive index -> data */
	{
		/* If there already was a Letter here, adjust size to the size of the new one */
		if (data->data[index])
			data->data[index] = (Letter) realloc(data->data[index], (strlen(letter) + 1) * sizeof(char));
		else /* Allocate memory if it's the first letter here */
			data->data[index] = (Letter) malloc((strlen(letter) + 1) * sizeof(char));
		strcpy(data->data[index], letter); /* Copy the letter */
	}
	else /* Negative index -> extra_data */
	{
		/*
		 * Here we deal with -index-1, not with index, because we want -4 to become 3 and so on
		 * (negative data index to positive extra_data index, see Data struct declaration)
		 */
		/* If there already was a Letter here, adjust size to the size of the new one */
		if (data->extra_data[-index-1])
		{
			data->extra_data[-index-1] = (Letter) realloc(
				data->extra_data[-index-1], (strlen(letter) + 1) * sizeof(char));
		}
		else /* Allocate memory if it's the first letter here */
			data->extra_data[-index-1] = (Letter) malloc((strlen(letter) + 1) * sizeof(char));
		strcpy(data->extra_data[-index-1], letter); /* Copy the letter */
	}
}

void
printData(Data * data)
{
	int count;
	for (count = data->extra_data_length ; count > 0 ; --count)
		printf("%s ", data->extra_data[count-1]);
	for (count = 0 ; count < data->data_length ; ++count)
		printf("%s ", data->data[count]);
}
