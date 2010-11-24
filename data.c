/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"

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
newData(Machine * machine)
{
	FILE * data_file = NULL;
	char data_filename[MAX_FILENAME_LENGTH];
	unsigned int line_number = 0;

	printf("Where is the file containing the data for your machine ?\n");
	/* Read the name of the file in which the Data are stored, don't fail if we cannot, just ask for another one */
	while (!scanf("%s", data_filename) || (data_file = fopen(data_filename, "r")) == NULL)
		printf("Failed to read file: %s, please enter a new one.\n", data_filename);

	Data * data = _newData(); /* Allocate memory */
	/* Extract Data from data_file */
	extractData(machine, data_file, &(data->data), &(data->data_length), &line_number, true);

	/* Close the data_file */
	fclose(data_file);
	return data;
}

void
freeData(Data * data)
{
	/* Free the memory used by a Data set */
	unsigned int i;
	for (i = 0 ; i < data->data_length ; ++i)
		free(data->data[i]); /* Free each Letter from data */
	/* Free remaining stuff */
	free(data->data);
	if (data->extra_data != NULL)
	{
		for (i = 0 ; i < data->extra_data_length ; ++i)
			free(data->extra_data[i]); /* Free each Letter from extra_data */
		free(data->extra_data);
	}
	free(data);
}

Letter
getLetter(Machine * machine)
{
	/* Local variables for readability */
	unsigned int index;
	Data * data = machine->data;

	if (machine->data_index >= 0) /* Positive index -> data */
	{
		/* Get the positive value of the index */
		index = (unsigned int)(machine->data_index);
		/* TODO: WTF >= ??? */
		if (index >= data->data_length) /* If we didn't reach that point yet */
		{
			if (!hasDefaultLetter(machine)) /* Machine does'nt support the default Letter and we need it */
				DefaultLetterException(machine);

			/* If full, increase size, then increment the length */
			if (((data->data_length)++ % BASE_STORAGE_LENGTH) == 0)
			{
				data->data = (LettersCollection) realloc(data->data,
					(data->data_length + BASE_STORAGE_LENGTH) * sizeof(Letter));
			}
			data->data[index] = NULL; /* Initialize to NULL so that memory will get allocated */
			setLetter(data, index, DEFAULT_LETTER); /* Set to default Letter */
		}
		/* Return the Letter we're up to */
		return data->data[index];
	}
	else /* Negative index -> extra_data */
	{
		/* Get the positive value of the index */
		index = (unsigned int)(-machine->data_index);
		if (index >= data->extra_data_length) /* If we didn't reach that point yet */
		{
			if (!hasDefaultLetter(machine)) /* Machine does'nt support the default Letter and we need it */
				DefaultLetterException(machine);

			/* If full, increase size */
			if ((data->extra_data_length % BASE_STORAGE_LENGTH) == 0)
			{
				data->extra_data = (LettersCollection) realloc(data->extra_data,
					(data->extra_data_length + BASE_STORAGE_LENGTH) * sizeof(Letter));
			}
			/* Increment the length */
			++(data->extra_data_length);
			data->extra_data[index-1] = NULL; /* Initialize to NULL so that memory will get allocated */
			setLetter(data, machine->data_index, DEFAULT_LETTER); /* Set to default Letter */
		}
		return data->extra_data[index-1]; /* Return the Letter we reached */
	}
}

void
setLetter(Data * data, const int index, const Letter letter)
{
	if (index >= 0) /* Positive index -> data */
	{
		/**
		 * If there already was a Letter here, adjust size to the size of the new one
		 * Otherwise allocate memory
		 */
		data->data[index] = (Letter) realloc(data->data[index], (strlen(letter) + 1) * sizeof(char));
		strcpy(data->data[index], letter); /* Copy the letter */
	}
	else /* Negative index -> extra_data */
	{
		/**
		 * Here we deal with -index-1, not with index, because we want -4 to become 3 and so on
		 * (negative data index to positive extra_data index, see Data struct declaration)
		 **
		 * If there already was a Letter here, adjust size to the size of the new one
		 * Otherwise allocate memory
		 */
		data->extra_data[-index-1] = (Letter) realloc(data->extra_data[-index-1], (strlen(letter) + 1) * sizeof(char));
		strcpy(data->extra_data[-index-1], letter); /* Copy the letter */
	}
}

void
printData(Data * data, int index)
{
	/**
	 * The if/else blocks here are to put the Letter to which the Machine is up to in red
	 */
	unsigned int i;
	/* Go through negative index */
	for (i = data->extra_data_length ; i > 0 ; --i)
	{
		if (i == (unsigned int)(-index))
			printf(" \033[0;31m%s\033[0m", data->extra_data[i-1]);
		else
			printf(" %s", data->extra_data[i-1]);
	}
	/* Go through positive index */
	for (i = 0 ; i < data->data_length ; ++i)
	{
		if (i == (unsigned int)index)
			printf(" \033[0;31m%s\033[0m", data->data[i]);
		else
			printf(" %s", data->data[i]);
	}
	printf("\n");
}
