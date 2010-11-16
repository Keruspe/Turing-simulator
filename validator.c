/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"
#include "validator.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool
validateElement(Element element, ElementsCollection valid_elements, int collection_length)
{
	/* Check if element is in valid_elements */
	int count;
	for (count = 0 ; count < collection_length ; ++count)
	{
		if (strcmp(element, valid_elements[count]) == 0)
			return true;
	}
	return false;
}

bool
hasDefaultLetter(Machine * machine)
{
	/* Check if we find the default Letter in the alphabet */
	return validateLetter(DEFAULT_LETTER, machine);
}

bool
validateTransition(Transition transition, Machine * machine, Element * malformed)
{
	/* Check if all states and letters used by a Transition are known by the Machine */
	if (!validateLetter(transition.cond, machine))
	{
		*malformed = transition.cond;
		return false;
	}
	if (!validateLetter(transition.subst, machine))
	{
		*malformed = transition.subst;
		return false;
	}
	if (!validateState(transition.start_state, machine))
	{
		*malformed = transition.start_state;
		return false;
	}
	if (!validateState(transition.next_state, machine))
	{
		*malformed = transition.next_state;
		return false;
	}
	return true;
}

bool
_checkInitialStateDeparture(Machine * machine)
{
	/* Check if there is at least one Transition available to leave the initial state */
	int count;
	for (count = 0 ; count < machine->transitions_length ; ++count)
	{
		if (strcmp(machine->transitions[count].start_state, machine->initial_state) == 0)
			return true;
	}
	return false;
}

bool
_checkInitialState(Machine * machine)
{
	/* Check if the initial state is recognized by the Machine */
	if (!validateState(machine->initial_state, machine))
		return false;
	if (!_checkInitialStateDeparture(machine)) /* Check if we can leave it */
		ValidationException(machine, NULL, "machine", "cannot leave the initial state.", NULL, 0);
	return true;
}

bool
_checkFinalStateReachability(Machine * machine)
{
	/* Check if there is at least one Transition available to reach the final state */
	int count;
	for (count = 0 ; count < machine->transitions_length ; ++count)
	{
		if (strcmp(machine->transitions[count].next_state, machine->final_state) == 0)
			return true;
	}
	return false;
}

bool
_checkFinalState(Machine * machine)
{
	if (!validateState(machine->final_state, machine)) /* Check if the final state is recognized by the Machine */
		return false;
	if (!_checkFinalStateReachability(machine)) /* Check if we can reach it */
		ValidationException(machine, NULL, "machine", "the final state is unreachable.", NULL, 0);
	return true;
}

bool
_checkAtLeastOnePathExists(Machine * machine)
{
	/* Check if at least one path exists from initial state to final state, not taking care of data */
	int states_checked = 0; /* The number of states we have checked ATM */
	int count; /* A counter */

	/* The states we found */
	StatesCollection states_found = (StatesCollection) malloc(machine->states_length * sizeof(State));
	State current_state = NULL; /* The State we're dealing with */
	Transition current_transition; /* The Transition we're dealing with */

	bool valid = false; /* The value we'll return */
	int states_found_number = 1; /* The number of states we found ATM (first one is the initial state) */
	states_found[0] = machine->initial_state;

	/* Loop while no transition allowed to reach the final state */
	while (!valid)
	{
		current_state = states_found[states_checked++]; /* Get a new current state and mark it as checked */
		/* Go through the Machine transitions */
		for (count = 0 ; count < machine->transitions_length ; ++count)
		{
			current_transition = machine->transitions[count]; /* Get a new Transition */
			/**
			 * If the start State of the Transition is our current State and we didn't already meet its final State,
			 * register this final State.
			 */
			if ((strcmp(current_transition.start_state, current_state) == 0)
				&& (!validateElement(current_transition.next_state, states_found, states_found_number)))
					states_found[states_found_number++] = current_transition.next_state;
		}
		/* Check if the final State is now in the list of states we found */
		valid = validateElement(machine->final_state, states_found, states_found_number);
		if (states_checked == states_found_number) /* If we checked all the states we found, stop */
			break;
	}
	/* Free the memory and return the result */
	free(states_found);
	return valid;
}

void
validate(Machine * machine)
{
	if (!_checkInitialState(machine)) /* Check if the initial state is alright */
		ValidationException(machine, NULL, "machine", "the initial state is not part of the alphabet.", NULL, 0);
	if (!_checkFinalState(machine)) /* Check if the final state is alright */
		ValidationException(machine, NULL, "machine", "the final state is not part of the alphabet.", NULL, 0);
	if (!_checkAtLeastOnePathExists(machine)) /* Check if the final state has any chance to be reached */
	{
		ValidationException(machine, NULL, "machine",
			"no matter the Data, there is no path available from the initial State of your Machine to its final State.", NULL, 0);
	}
}
