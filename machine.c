/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"

Machine *
_newMachine()
{
	/* Basic allocation and initialisation stuff to get a ready base Machine */
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
	/* Read the alphabet recognized by the Machine from machineFile */
	_extractData(machineFile, &(machine->alphabet), &(machine->alphabet_length));
}

void
_readStates(Machine * machine, FILE * machineFile)
{
	/* Read the states in which the Machine can be from machineFile */
	_extractData(machineFile, &(machine->states), &(machine->states_length));
}

void
_readTransitions(Machine * machine, FILE * machineFile)
{
	/* Read the available transitions */
	Element element; /* Will be use to store read data */
	/* Keep reading while there are things to read (we did not meet '#') */
	while (! ((element = _readElement(machineFile)).endOfElements))
	{
		/* The first element we just read is the start state */
		machine->transitions[machine->transitions_length].start_state = element.element;
		/* Then read the conditionnal value */
		machine->transitions[machine->transitions_length].cond = _readElement(machineFile).element;
		/* Then the substitution value */
		machine->transitions[machine->transitions_length].subst = _readElement(machineFile).element;
		/* Continue with the next state */
		machine->transitions[machine->transitions_length].next_state = _readElement(machineFile).element;
		element = _readElement(machineFile); /* Now read what will be the direction of the next move */
		Move move = element.element[0]; /* We only want the first char of it */
		free(element.element); /* So free this array not to have any leak */
		if (move != 'R' && move != 'L')
		{
			/* Move was not 'R' or 'L', we don't know any other, abort */
			printf("Bad move, only 'R' or 'L' are allowed, exiting...\n");
			++machine->transitions_length;
			freeMachine(machine);
			exit(EXIT_FAILURE);
		}
		/* Store the move and increase the number of transitions available */
		machine->transitions[machine->transitions_length++].move = move;
		if (element.endOfElements) /* If we reached a '#', just exit this function */
			return;
		/* When the Array is full, increase its size */
		if ((machine->transitions_length % BASE_TRANSITIONS_LENGTH) == 0)
			machine->transitions = (Transition *) realloc(machine->transitions, (machine->transitions_length + BASE_TRANSITIONS_LENGTH) * sizeof(Transition));
	}
}

void
_readStartAndEndPoints(Machine * machine, FILE * machineFile)
{
	/* Read the initial state, and the the final state of the Machine */
	machine->initial_state = _readElement(machineFile).element;
	machine->final_state = _readElement(machineFile).element;
}

Machine *
newMachine()
{
	FILE * machineFile = NULL;
	char machineFilename[MAX_FILENAME_LENGTH];

	printf("Where is the file describing your turing machine ?\n");
	/* Read the name of the file in which the Machine is described, fail and exit if we cannot load it */
	if (!scanf("%s", machineFilename) || (machineFile = fopen(machineFilename, "r")) == NULL)
	{
		printf("Failed to read file: %s, exiting...\n", machineFilename);
		exit(EXIT_FAILURE);
	}

	Machine * machine = _newMachine(); /* Allocate memory */
	/* Read machine stuff from machineFile */
	_readAlphabet(machine,machineFile);
	_readStates(machine, machineFile);
	_readTransitions(machine, machineFile);
	_readStartAndEndPoints(machine, machineFile);

	/* Close the machineFile */
	fclose(machineFile);
	return machine;
}

void
freeTransition(Transition transition)
{
	/* Free the memory used by a Transition */
	free(transition.start_state);
	free(transition.cond);
	free(transition.subst);
	free(transition.next_state);
}

void
freeMachine(Machine * machine)
{
	/* Free the memory used by a Machine */
	int i; /* Will be a basic counter */
	for (i = 0 ; i < machine->alphabet_length ; ++i)
		free(machine->alphabet[i]); /* Free each Letter in the alphabet */
	for (i = 0 ; i < machine->states_length ; ++i)
		free(machine->states[i]); /* Free each possible State */
	for (i = 0 ; i < machine->transitions_length ; ++i)
		freeTransition(machine->transitions[i]); /* Free each available Transition */
	/* Free remaining stuff */
	free(machine->initial_state);
	free(machine->final_state);
	free(machine->alphabet);
	free(machine->states);
	free(machine->transitions);
	if (machine->data) /* If the machine had Data, free it */
		freeData(machine->data);
	free(machine);
}

void
reloadData(Machine * machine)
{
	if (machine->data) /* If the machine had Data, free it */
		freeData(machine->data);

	/* Then read Data */
	machine->data = newData();
	clearBuffer(); /* Clear buffer */
}
