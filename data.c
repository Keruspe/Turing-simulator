#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Data *
newData(char * filename)
{
	FILE * dataFile = fopen(filename, "r");
	if (dataFile == NULL)
		return NULL;
	Data * data = (Data *) malloc(sizeof(Data));
	/* TODO */
	fclose(dataFile);
	return data;
}
