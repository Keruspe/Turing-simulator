/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"
#include "validator.h"
#include <string.h>

Machine *
_newMachine()
{
	/* Basic allocation and initialisation stuff to get a ready base Machine */
	Machine * machine = (Machine *) malloc(sizeof(Machine));
	machine->alphabet = (LettersCollection) malloc(BASE_STORAGE_LENGTH * sizeof(Letter));
	machine->alphabet_length = 0;
	machine->states = (StatesCollection) malloc(BASE_STORAGE_LENGTH * sizeof(State));
	machine->states_length = 0;
	machine->transitions = (TransitionsCollection) malloc(BASE_TRANSITIONS_LENGTH * sizeof(Transition));
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
_readAlphabet(Machine * machine, FILE * machine_file, int * line_number)
{
	/* Read the alphabet recognized by the Machine from machine_file */
	extractData(machine_file, &(machine->alphabet), &(machine->alphabet_length), line_number);
	if (machine->alphabet_length == 0) /* Fail if no alphabet in the machine file */
		MalformedFileException(machine, machine_file, "your Machine has no alphabet.", *line_number);
}

void
_readStates(Machine * machine, FILE * machine_file, int * line_number)
{
	/* Read the states in which the Machine can be from machine_file */
	extractData(machine_file, &(machine->states), &(machine->states_length), line_number);
	if (machine->states_length == 0) /* Fail if no states in the machine file */
		MalformedFileException(machine, machine_file, "your Machine has no states.", *line_number);
}

bool
_readTransitionElement(Machine * machine, FILE * machine_file, Transition * transition, Element * element, int * line_number)
{
	bool notYetTheEnd = readElement(machine_file, element, line_number); /* Read an Element */
	if ((*element)[0] == '\0' || (*element)[0] == '#') /* If the element is dummy */
	{
		if (transition->start_state != *element)
		{ /* We're not reading the first Element of the Transition, it's malformed */
			freeTransition(*transition);
			BadTransitionException(machine, machine_file, NULL, *line_number);
		}
		else if (machine->transitions_length == 0)
		{ /* We're reading the first Element of the first Transition, there is no Transition */
			free (*element);
			BadTransitionException(machine, machine_file, "your Machine has no Transition.", *line_number);
		}
		/* Else let the dummy Transition being treated later */
	}
	return notYetTheEnd;
}

bool
_readTransition(Machine * machine, FILE * machine_file, Transition * transition, int * line_number)
{
	_initTransition(transition); /* Initialize the Transition */
	/* The first element we just read is the start state */
	_readTransitionElement(machine, machine_file, transition, &(transition->start_state), line_number);
	if (transition->start_state[0] == '\0' || transition->start_state[0] == '#') /* No more transitions to read */
		return false;
	/* Then read the conditionnal value */
	_readTransitionElement(machine, machine_file, transition, &(transition->cond), line_number);
	/* Then the substitution value */
	_readTransitionElement(machine, machine_file, transition, &(transition->subst), line_number);
	/* Continue with the next state */
	_readTransitionElement(machine, machine_file, transition, &(transition->next_state), line_number);
	/* Temporary element to avoid a memory leak */
	Element element = NULL;
	bool notYetTheEnd = _readTransitionElement(machine, machine_file, transition, &element, line_number);
	transition->move = element[0]; /* We only want the first char of it */
	free(element); /* So free this array not to have any leak */
	/* Move was not 'R' or 'L', we don't know any other, abort */
	if (transition->move != 'R' && transition->move != 'L')
		BadTransitionException(machine, machine_file,
			"bad move in Transition, only 'R' or 'L' are allowed", *line_number);
	return notYetTheEnd;
}

void
_readTransitions(Machine * machine, FILE * machine_file, int * line_number)
{
	/* Read the available transitions */
	Transition transition; /* Will be used to store each transition */
	/* Keep reading while there are things to read (we did not meet '#') */
	while (_readTransition(machine, machine_file, &transition, line_number))
	{ /* While there are still transitions to read */
		/* Store the transition and increase the number of transitions available */
		machine->transitions[machine->transitions_length++] = transition;
		/* Validate the transition */
		if (!validateTransition(transition, machine))
			BadTransitionException(machine, machine_file, NULL, *line_number);
		/* When the Array is full, increase its size */
		if ((machine->transitions_length % BASE_TRANSITIONS_LENGTH) == 0)
			machine->transitions = (TransitionsCollection) realloc(machine->transitions,
				(machine->transitions_length + BASE_TRANSITIONS_LENGTH) * sizeof(Transition));
	}
	if (transition.start_state[0] != '\0') /* We're not facing a dummy transition */
		machine->transitions[machine->transitions_length++] = transition; /* Store the last transition */
	else /* Dummy transition only has a start_state, but we need to free it */
		free (transition.start_state);
}

void
_readStartAndEndPoints(Machine * machine, FILE * machine_file, int * line_number)
{
	/* Read the initial state of the Machine */
	do
	{
		readElement(machine_file, &(machine->initial_state), line_number);
	} while(machine->initial_state[0] == '#'); /* Ignore all # at this point */
	if(machine->initial_state[0] == '\0')
		MalformedFileException(machine, machine_file, "no initial state", *line_number);
	/* Read the final state of the Machine */
	do
	{
		readElement(machine_file, &(machine->final_state), line_number);
	} while(machine->final_state[0] == '#'); /* Ignore all # at this point */
	if(machine->final_state[0] == '\0')
		MalformedFileException(machine, machine_file, "no final state", *line_number);
}

Machine *
newMachine()
{
	FILE * machine_file = NULL;
	char machine_filename[MAX_FILENAME_LENGTH] = "";
	int line_number = 0;

	printf("Where is the file describing your turing machine ?\n");
	/* Read the name of the file in which the Machine is described, fail and exit if we cannot load it */
	if (!scanf("%s", machine_filename) || (machine_file = fopen(machine_filename, "r")) == NULL)
		NoSuchFileException(machine_filename);

	Machine * machine = _newMachine(); /* Allocate memory */
	/* Read machine stuff from machine_file */
	_readAlphabet(machine, machine_file, &line_number);
	_readStates(machine, machine_file, &line_number);
	_readTransitions(machine, machine_file, &line_number);
	_readStartAndEndPoints(machine, machine_file, &line_number);

	fclose(machine_file); /* Close the machine_file */
	validate(machine); /* Validate the Machine */

	return machine;
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
	machine->data_index = -1; /* The Machine starts at the left of the first cell */
	machine->data = newData(); /* Then read Data */
	validateData(machine); /* Validate the new Data set */
	clearBuffer(); /* Clear buffer */
}

Letter
go(Machine * machine, Move move)
{
	switch (move)
	{
	case 'R': /* Go right */
		++(machine->data_index);
		break;
	case 'L': /* Go left */
		--(machine->data_index);
		break;
	default: /* Really weird */
		RuntimeException(machine, "bad move found, this is weird, should have been caught by parser.");
	}
	return getLetter(machine);
}

Transition *
_getTransition(Machine * machine, State current_state, Letter current_letter)
{
	/* Get the transition we must now apply */
	Transition * current_transition;
	int count;
	for (count = 0 ; count < machine->transitions_length ; ++count)
	{
		current_transition = &(machine->transitions[count]);
		/* If the conditions of the Transition are satisfied by the current state, return it, it's the one '*/
		if ((strcmp(current_transition->start_state, current_state) == 0) &&
			(strcmp(current_transition->cond, current_letter) == 0))
				return current_transition;
	}
	return NULL; /* No good Transition has been found */
}

void
execute(Machine * machine)
{
	Move move = 'R'; /* First move */
	/* Initializations */
	Transition * current_transition;
	State current_state = machine->initial_state;
	Letter current_letter = NULL;
	int steps=0;

	/* Print the start state (Data) */
	printf("\n===========================================\nInitial Data:\n");
	printData(machine->data);
	printf("\n================== Start ==================\nExecuting your Machine:\n");

	printData(machine->data);
	while (strcmp(current_state, machine->final_state) != 0) /* Loop until we reach the final state */
	{
		current_letter = go(machine, move); /* Move and get the new Letter */
		/* Get the good Transition to apply */
		if ((current_transition = _getTransition(machine, current_state, current_letter)) == NULL)
			RuntimeException(machine, "no matching transition found.");
		/* Apply Transition */
		current_state = current_transition->next_state;
		move = current_transition->move;
		setLetter(machine->data, machine->data_index, current_transition->subst);
		printf("\n");
		printData(machine->data);

		if (++steps >= MAX_STEPS) /* Avoid infinite loop */
		{
			TooMuchStepsException(machine);
		}
	}

	printf("\n=================== Done ==================\nResult after %d steps:\n", steps);
	printData(machine->data);
	printf("\n===========================================\n\n");
}
