#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

Machine *
newMachine(char * filename)
{
	FILE * machineFile = fopen(filename, "r");
	if (machineFile == NULL)
		return NULL;
	Machine * machine = (Machine *) malloc(sizeof(Machine));
	/* TODO */
	fclose(machineFile);
	return machine;
}
