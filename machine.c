#include "machine.h"

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

void
_storeLetter(Machine * machine, Letter * current, int * current_size)
{
	(*current)[*current_size] = '\0';
	machine->alphabet[machine->alphabet_length++] = *current;
}

void
_nextLetter(Machine * machine, Letter * current, int * current_size)
{
	if (*current_size == 0)
		return;
	_storeLetter(machine, current, current_size);
	*current_size = 0;
	*current = (Letter) malloc(BASE_LETTER_SIZE * sizeof(char));
	if ((machine->alphabet_length % BASE_ALPHABET_LENGTH) == 0)
		machine->alphabet = realloc(machine->alphabet, (machine->alphabet_length + BASE_ALPHABET_LENGTH) * sizeof(Letter));
}

bool
_handleLetter(char c, FILE * machineFile, Machine * machine, Letter * current, int * current_size)
{
	switch (c)
	{
	case '$':
		skipLine(machineFile);
	case '\n':
	case '\t':
	case ' ':
		_nextLetter(machine, current, current_size);
		return false;
	case '#':
		if (*current_size != 0)
			_storeLetter(machine, current, current_size);
		else
			free(*current);
		return true;
	default:
		(*current)[*current_size] = c;
		if ((++(*current_size) % BASE_LETTER_SIZE) == 0)
			*current = realloc(*current, (*current_size + BASE_LETTER_SIZE) * sizeof(char));
		return false;
	}
}

void
_readAlphabet(Machine * machine, FILE * machineFile)
{
	char c;
	Letter current = (Letter) malloc((1 + BASE_LETTER_SIZE) * sizeof(char));
	int current_size = 0;
	while ((c = fgetc(machineFile)) != EOF)
	{
		if (_handleLetter(c, machineFile, machine, &current, &current_size))
			return;
	}
}

void
_readStates(Machine * machine, FILE * machineFile)
{
	return;
}

void
_readTransitions(Machine * machine, FILE * machineFile)
{
	return;
}

void
_readStartAndEndPoints(Machine * machine, FILE * machineFile)
{
	return;
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

