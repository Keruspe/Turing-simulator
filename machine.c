#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

Machine *
_newMachine()
{
	Machine * machine = (Machine *) malloc(sizeof(Machine));
	machine->alphabet = (Letter *) malloc(BASE_ALPHABET_LENGTH * sizeof(Letter));
	machine->alphabet_length = 0;
	machine->states = (State *) malloc(BASE_STATES_LENGTH * sizeof(State));
	machine->states_length = 0;
	machine->transitions = (Transition *) malloc(BASE_TRANSITIONS_LENGTH * sizeof(Transition));
	machine->transitions_length = 0;
	machine->data = NULL;
	return machine;
}

Machine *
newMachine()
{
	FILE * machineFile = NULL;
	char machineFilename[MAX_FILENAME_LENGTH];

	printf("Where is the file describing your turing machine ?\n");
	if (!scanf("%s", machineFilename) || (machineFile = fopen(machineFilename, "r")) == NULL)
	{
		printf("Failed to read file: %s, exiting...\n", machineFilename);
		exit(EXIT_FAILURE);
	}

	Machine * machine = _newMachine();

	/* TODO */

	fclose(machineFile);
	return machine;
}

void
freeMachine(Machine * machine)
{
	free(machine->alphabet);
	free(machine->states);
	free(machine->transitions);
	if (machine->data)
		freeData(machine->data);
	free(machine);
}

void
reloadData(Machine * machine)
{
	if (machine->data)
		freeData(machine->data);

	machine->data = newData();
	while (getchar() != '\n'); /* Clear buffer */
}

