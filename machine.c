#include "machine.h"
#include <stdio.h>
#include <stdlib.h>

Machine *
_newMachine()
{
	Machine * machine = (Machine *) malloc(sizeof(Machine));
	machine->alphabet = (Letter *) malloc(ALPHABET_BASE_LENGTH * sizeof(Letter));
	machine->alphabet_length = 0;
	machine->states = (State *) malloc(STATES_BASE_LENGTH * sizeof(State));
	machine->states_length = 0;
	machine->transitions = (Transition *) malloc(TRANSITIONS_BASE_LENGTH * sizeof(Transition));
	machine->transitions_length = 0;
	return machine;
}

Machine *
newMachine(char * filename)
{
	FILE * machineFile = fopen(filename, "r");
	if (machineFile == NULL)
		return NULL;

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
	free(machine);
}
