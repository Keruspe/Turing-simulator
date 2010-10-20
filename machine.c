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
	machine->initial_state = NULL;
	machine->final_state = NULL;
	machine->data = NULL;
	return machine;
}

void
_initTransition(Transition * transition)
{
	/* Basic initialisation of a Transition */
	transition->start_state = NULL;
	transition->cond = NULL;
	transition->subst = NULL;
	transition->next_state = NULL;
	transition->move = '\0';
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

Element
_readTransitionElement(Machine * machine, FILE * machineFile)
{
	Element element = _readElement(machineFile);
	if (element.element[0] == '\0')
	{
		free(element.element);
		if (machine->transitions_length == 0)
			MalformedFileException(machine, machineFile, "your Machine has no Transition.");
		else
			BadTransitionException(machine, machineFile);
	}
	return element;
}

void
_readTransitions(Machine * machine, FILE * machineFile)
{
	/* Read the available transitions */
	Element element; /* Will be used to store read data */
	Transition transition; /* Will be used to store each transition */
	/* Keep reading while there are things to read (we did not meet '#') */
	while (! ((element = _readTransitionElement(machine, machineFile)).endOfElements))
	{
		_initTransition(&transition); /* Initialize Transition */
		transition.start_state = element.element; /* The first element we just read is the start state */
		transition.cond = _readTransitionElement(machine, machineFile).element; /* Then read the conditionnal value */
		transition.subst = _readTransitionElement(machine, machineFile).element; /* Then the substitution value */
		transition.next_state = _readTransitionElement(machine, machineFile).element; /* Continue with the next state */
		element = _readTransitionElement(machine, machineFile); /* Now read what will be the direction of the next move */
		transition.move = element.element[0]; /* We only want the first char of it */
		free(element.element); /* So free this array not to have any leak */
		/* Store the transition and increase the number of transitions available */
		machine->transitions[machine->transitions_length++] = transition;
		/* Move was not 'R' or 'L', we don't know any other, abort */
		if (transition.move != 'R' && transition.move != 'L')
			MalformedFileException(machine, machineFile, "bad move in transition, only 'R' or 'L' are allowed");
		if (element.endOfElements) /* If we reached a '#', just exit this function */
			return;
		/* When the Array is full, increase its size */
		if ((machine->transitions_length % BASE_TRANSITIONS_LENGTH) == 0)
			machine->transitions = (Transition *) realloc(machine->transitions, (machine->transitions_length + BASE_TRANSITIONS_LENGTH) * sizeof(Transition));
	}
}

char *
_readStartAndEndPoints(Machine * machine, FILE * machineFile)
{
	/* Read the initial state of the Machine */
	machine->initial_state = _readElement(machineFile).element;
	if(machine->initial_state[0] == '\0')
		return "no initial state";
	/* Read the final state of the Machine */
	machine->final_state = _readElement(machineFile).element;
	if(machine->final_state[0] == '\0')
		return "no final state";
	return NULL;
}

Machine *
newMachine()
{
	FILE * machineFile = NULL;
	char machineFilename[MAX_FILENAME_LENGTH] = "";

	printf("Where is the file describing your turing machine ?\n");
	/* Read the name of the file in which the Machine is described, fail and exit if we cannot load it */
	if (!scanf("%s", machineFilename) || (machineFile = fopen(machineFilename, "r")) == NULL)
		NoSuchFileException(machineFilename);

	Machine * machine = _newMachine(); /* Allocate memory */
	/* Read machine stuff from machineFile */
	_readAlphabet(machine,machineFile);
	/* Fail if no alphabet in the machineFile */
	if (machine->alphabet_length == 0)
		MalformedFileException(machine, machineFile, "your Machine has no alphabet.");
	_readStates(machine, machineFile);
	/* Fail if no states in the machineFile */
	if (machine->states_length == 0)
		MalformedFileException(machine, machineFile, "your Machine has no states.");
	_readTransitions(machine, machineFile);
	/* Fail if no initial or final state in the machineFile */
	char * failure_reason = _readStartAndEndPoints(machine, machineFile);
	if (failure_reason != NULL)
		MalformedFileException(machine, machineFile, failure_reason);

	/* Close the machineFile */
	fclose(machineFile);
	return machine;
}

void
freeTransition(Transition transition)
{
	/* Free the memory used by a Transition */
	free(transition.start_state);
	if (transition.cond)
		free(transition.cond);
	if (transition.subst)
		free(transition.subst);
	if (transition.next_state)
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
	free(machine->alphabet);
	free(machine->states);
	free(machine->transitions);
	if (machine->initial_state)
		free(machine->initial_state);
	if (machine->final_state)
		free(machine->final_state);
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
