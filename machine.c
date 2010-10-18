#include "machine.h"

Machine *
_newMachine()
{
	Machine * machine = (Machine *) malloc(sizeof(Machine));
	machine->alphabet = (Letter *) malloc(BASE_ALPHABET_LENGTH * sizeof(Letter));
	machine->alphabet_length = 0;
	machine->states = (State *) malloc(BASE_ALPHABET_LENGTH * sizeof(State));
	machine->states_length = 0;
	machine->transitions = (Transition *) malloc(BASE_TRANSITIONS_LENGTH * sizeof(Transition));
	machine->transitions_length = 0;
	machine->data = NULL;
	return machine;
}

void
_readAlphabet(Machine * machine, FILE * machineFile)
{
	_extractData(machineFile, &(machine->alphabet), &(machine->alphabet_length));
}

void
_readStates(Machine * machine, FILE * machineFile)
{
	_extractData(machineFile, &(machine->states), &(machine->states_length));
}

void
_readTransitions(Machine * machine, FILE * machineFile)
{
	/* Dummy stuff to skip this for now */
	char c;
	while ((c = fgetc(machineFile)) != EOF)
	{
		switch (c)
		{
		case '#':
			return;
		case '$':
			skipLine(machineFile);
		}
		machine->transitions_length = 0; /* Silent warning */
	}
}

void
_readStartAndEndPoints(Machine * machine, FILE * machineFile)
{
	machine->initial_state = _readElement(machineFile).element;
	machine->final_state = _readElement(machineFile).element;
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
	_readAlphabet(machine,machineFile);
	_readStates(machine, machineFile);
	_readTransitions(machine, machineFile);
	_readStartAndEndPoints(machine, machineFile);

	fclose(machineFile);
	return machine;
}

void
freeMachine(Machine * machine)
{
	int i;
	for (i = 0 ; i < machine->alphabet_length ; ++i)
		free(machine->alphabet[i]);
	for (i = 0 ; i < machine->states_length ; ++i)
		free(machine->states[i]);
	free(machine->initial_state);
	free(machine->final_state);
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
	clearBuffer(); /* Clear buffer */
}

