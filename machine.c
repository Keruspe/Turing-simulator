/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "transition.h"

#include <string.h>

static Machine *
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

static void
_readAlphabet(Machine * machine, FILE * machine_file, unsigned int * line_number)
{
	/* Read the alphabet recognized by the Machine from machine_file */
	extractData(machine, machine_file, &(machine->alphabet), &(machine->alphabet_length), line_number, false);
	if (machine->alphabet_length == 0) /* Fail if no alphabet in the machine file */
		ValidationException(machine, machine_file, "machine", "no alphabet found", NULL, *line_number);
}

static void
_readStates(Machine * machine, FILE * machine_file, unsigned int * line_number)
{
	/* Read the states in which the Machine can be from machine_file */
	extractData(machine, machine_file, &(machine->states), &(machine->states_length), line_number, false);
	if (machine->states_length == 0) /* Fail if no states in the machine file */
		ValidationException(machine, machine_file, "Machine", "no State found", NULL, *line_number);
}

static bool
_readTransitionElement(Machine * machine, FILE * machine_file, Transition * transition, Element * element, unsigned int * line_number)
{
	bool still_things_to_read = readElement(machine_file, element, line_number); /* Read an Element */
	if ((*element)[0] == '\0' || (*element)[0] == '#') /* If the element is dummy */
	{
		if (transition->start_state != *element)
		{ /* We're not reading the first Element of the Transition, it's malformed */
			freeTransition(transition);
			BadTransitionException(machine, machine_file, "incomplete Transition", NULL, *line_number);
		}
		else if (machine->transitions_length == 0)
		{ /* We're reading the first Element of the first Transition, there is no Transition */
			free (*element);
			BadTransitionException(machine, machine_file, "your Machine has no Transition", NULL, *line_number);
		}
		/* Else let the dummy Transition being treated later */
	}
	return still_things_to_read;
}

static bool
_readTransition(Machine * machine, FILE * machine_file, Transition * transition, unsigned int * line_number)
{
	initTransition(transition); /* Initialize the Transition */
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
	bool still_things_to_read = _readTransitionElement(machine, machine_file, transition, &element, line_number);
	transition->move = element[0]; /* We only want the first char of it */
	free(element); /* So free this array not to have any leak */
	return still_things_to_read;
}

static void
_readTransitions(Machine * machine, FILE * machine_file, unsigned int * line_number)
{
	/* Read the available transitions */
	Transition transition; /* Will be used to store each Transition */
	Element malformed = NULL; /* Will be used to store the malformed Element if a Transition fails validation */
	String reason = NULL; /* Will be used to store the reason if a Transition fails validation */
	/* Keep reading while there are things to read (we did not meet '#') */
	while (_readTransition(machine, machine_file, &transition, line_number))
	{ /* While there are still transitions to read */
		/* Store the transition and increase the number of transitions available */
		machine->transitions[machine->transitions_length++] = transition;
		/* Validate the transition */
		if (!validateTransition(transition, machine, &reason, &malformed))
			BadTransitionException(machine, machine_file, reason, malformed, *line_number);
		/* When the Array is full, increase its size */
		if ((machine->transitions_length % BASE_TRANSITIONS_LENGTH) == 0)
		{
			machine->transitions = (TransitionsCollection) realloc(machine->transitions,
				(machine->transitions_length + BASE_TRANSITIONS_LENGTH) * sizeof(Transition));
		}
	}
	if (transition.start_state[0] != '\0') /* We're not facing a dummy transition */
		machine->transitions[machine->transitions_length++] = transition; /* Store the last transition */
	else /* Dummy transition only has a start_state, but we need to free it */
		free (transition.start_state);
}

static void
_readStartAndEndPoints(Machine * machine, FILE * machine_file, unsigned int * line_number)
{
	/* Read the initial state of the Machine */
	do
	{
		readElement(machine_file, &(machine->initial_state), line_number);
	} while(machine->initial_state[0] == '#'); /* Ignore all # at this point, consider them as spaces */
	if(machine->initial_state[0] == '\0')
		ValidationException(machine, machine_file, "machine", "no initial state", NULL, *line_number);
	/* Read the final state of the Machine */
	do
	{
		readElement(machine_file, &(machine->final_state), line_number);
	} while(machine->final_state[0] == '#'); /* Ignore all # at this point, consider them as spaces */
	if(machine->final_state[0] == '\0')
		ValidationException(machine, machine_file, "machine", "no final state", NULL, *line_number);
}

Machine *
newMachine()
{
	FILE * machine_file = NULL;
	char machine_filename[MAX_FILENAME_LENGTH];
	unsigned int line_number = 0;

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

	validate(machine, machine_file, line_number); /* Validate the Machine */
	fclose(machine_file); /* Close the machine_file */

	return machine;
}

void
freeMachine(Machine * machine)
{
	/* Free the memory used by a Machine */
	unsigned int i;
	for (i = 0 ; i < machine->alphabet_length ; ++i)
		free(machine->alphabet[i]); /* Free each Letter in the alphabet */
	for (i = 0 ; i < machine->states_length ; ++i)
		free(machine->states[i]); /* Free each possible State */
	for (i = 0 ; i < machine->transitions_length ; ++i)
		freeTransition(&(machine->transitions[i])); /* Free each available Transition */
	/* Free remaining stuff */
	free(machine->alphabet);
	free(machine->states);
	free(machine->transitions);
	if (machine->initial_state != NULL)
		free(machine->initial_state);
	if (machine->final_state != NULL)
		free(machine->final_state);
	if (machine->data != NULL) /* If the machine had Data, free it */
		freeData(machine->data);
	free(machine);
}

void
reloadData(Machine * machine)
{
	if (machine->data != NULL) /* If the machine had Data, free it */
		freeData(machine->data);
	machine->data_index = -1; /* The Machine starts at the left of the first cell */
	machine->data = newData(machine); /* Then read Data */
	clearBuffer(); /* Clear buffer */
}

Letter
go(Machine * machine, const Move move)
{
	if (move == RIGHT) /* Go right */
		++(machine->data_index);
	else if (move == LEFT) /* Go left */
		--(machine->data_index);
	/* We don't move with NONE */
	/* Return the Letter we're up to */
	return getLetter(machine);
}

static Transition *
_getTransition(Machine * machine, State current_state, Letter current_letter)
{
	/* Get the transition we must now apply */
	Transition * current_transition;
	unsigned int i;
	/* Go through all available transitions */
	for (i = 0 ; i < machine->transitions_length ; ++i)
	{
		current_transition = &(machine->transitions[i]); /* Get a new Transition */
		/* If the conditions of the Transition are satisfied by the current State and Letter, return it */
		if ((strcmp(current_transition->start_state, current_state) == 0) &&
			(strcmp(current_transition->cond, current_letter) == 0))
				return current_transition;
	}
	return NULL; /* No good Transition has been found */
}

void
execute(Machine * machine, bool step_by_step)
{
	Move move = RIGHT; /* First move */
	/* Initializations */
	Transition * current_transition;
	State current_state = machine->initial_state;
	Letter current_letter = NULL;
	unsigned int steps=0;

	/* Print the start state (Data) */
	printf("\n===========================================\nInitial Data:\n");
	printData(machine->data, machine->data_index);
	printf("\n================== Start ==================\nExecuting your Machine:\n");

	printf("%s:", current_state);
	printData(machine->data, machine->data_index);
	if (step_by_step)
		clearBuffer();
	while (strcmp(current_state, machine->final_state) != 0) /* Loop until we reach the final state */
	{
		current_letter = go(machine, move); /* Move and get the new Letter */
		/* Get the good Transition to apply, if it's NULL, abort */
		if ((current_transition = _getTransition(machine, current_state, current_letter)) == NULL)
			NoSuchTransitionException(machine, current_state, current_letter);
		/* Apply Transition */
		current_state = current_transition->next_state;
		move = current_transition->move;
		setLetter(machine->data, machine->data_index, current_transition->subst);

		/* Print the current Sate */
		if (! step_by_step) /* Rewrite over previous state */
			printf("\r");
		printf("%s:", current_state);
		printData(machine->data, machine->data_index);

		if (++steps >= MAX_STEPS) /* Avoid infinite loop */
			TooMuchStepsException(machine);
		if (step_by_step) /* Wait until user type enter */
			clearBuffer();
	}

	/* Print the result */
	printf("\n=================== Done ==================\nResult after %d steps:\n", steps);
	printData(machine->data, -machine->data->extra_data_length-1);
	printf("\n===========================================\n\n");
}
