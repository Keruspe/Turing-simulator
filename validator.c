/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "transition.h"
#include "validator.h"

#include <string.h>

bool
validateElement(const Element element, const ElementsCollection valid_elements, const unsigned int collection_length)
{
	unsigned int i;
	/* Go through the ElementsCollection */
	for (i = 0 ; i < collection_length ; ++i)
	{
		/* If we find the Element, return true */
		if (strcmp(element, valid_elements[i]) == 0)
			return true;
	}
	/* We didn't find it, return false */
	return false;
}

bool
hasDefaultLetter(const Machine * machine)
{
	/* Check if we find the default Letter in the alphabet */
	return validateLetter(DEFAULT_LETTER, machine);
}

bool
validateTransition(const Transition transition, const Machine * machine, String reason, Element * malformed)
{
	/**
	 * Check if all states and letters used by a Transition are known by the Machine
	 * If a wrong Element is found, put it in malformed and return false
	 */
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
	/* Particular case, give a reason instead of a malformed Element */
	if (transition.move != RIGHT && transition.move != LEFT && transition.move != NONE)
	{
		reason = "bad move in Transition, only 'R', 'L' or 'N' are allowed";
		return false;
	}
	/* Everything is fine, return true */
	return true;
}

static bool
_checkInitialStateDeparture(const Machine * machine)
{
	/* Check if there is at least one Transition available to leave the initial state */
	unsigned int i;
	for (i = 0 ; i < machine->transitions_length ; ++i)
	{
		/* Compare the start_state of each Transition with the initial_state of the Machine */
		if (strcmp(machine->transitions[i].start_state, machine->initial_state) == 0)
			return true;
	}
	/* No Transition allows us to leave the initial State */
	return false;
}

static bool
_checkInitialState(Machine * machine, FILE * file, const unsigned int line_number)
{
	/* Check if the initial state is recognized by the Machine */
	if (!validateState(machine->initial_state, machine))
		return false;
	if (!_checkInitialStateDeparture(machine)) /* Check if we can leave it */
		ValidationException(machine, file, "machine", "cannot leave the initial state.", NULL, line_number);
	/* Everything's fine */
	return true;
}

static bool
_checkFinalStateReachability(const Machine * machine)
{
	/* Check if there is at least one Transition available to reach the final state */
	unsigned int i;
	for (i = 0 ; i < machine->transitions_length ; ++i)
	{
		/* Compare the next_state of each Transition with the final_state of the Machine */
		if (strcmp(machine->transitions[i].next_state, machine->final_state) == 0)
			return true;
	}
	/* We cannot reach it */
	return false;
}

static bool
_checkFinalState(Machine * machine, FILE * file, const unsigned int line_number)
{
	/* Check if the final state is recognized by the Machine */
	if (!validateState(machine->final_state, machine))
		return false;
	if (!_checkFinalStateReachability(machine)) /* Check if we can reach it */
		ValidationException(machine, file, "machine", "the final state is unreachable.", NULL, line_number);
	/* Everything's fine */
	return true;
}

static bool
_checkAtLeastOnePathExists(Machine * machine)
{
	/* Check if at least one path exists from initial state to final state, not taking care of data */
	unsigned int states_checked = 0; /* The number of states we have checked ATM */
	unsigned int i;

	/* The states we found */
	StatesCollection states_found = (StatesCollection) malloc(machine->states_length * sizeof(State));
	State current_state = NULL; /* The State we're dealing with */
	Transition current_transition; /* The Transition we're dealing with */

	bool valid = false; /* The value we'll return */
	unsigned int states_found_number = 1; /* The number of states we found ATM (first one is the initial state) */
	states_found[0] = machine->initial_state;

	/* Loop while no transition allowed to reach the final state */
	while (!valid)
	{
		/* Get a new current state and mark it as checked */
		current_state = states_found[states_checked++];
		/* Go through the Machine transitions */
		for (i = 0 ; i < machine->transitions_length ; ++i)
		{
			current_transition = machine->transitions[i]; /* Get a new Transition */
			/**
			 * If the start_state of the Transition is our current State and we didn't already meet
			 * its final State, register this final State.
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
validate(Machine * machine, FILE * file, const unsigned int line_number)
{
	if (!_checkInitialState(machine, file, line_number)) /* Check if the initial state is alright */
		ValidationException(machine, file, "machine", "the initial state is not part of the alphabet.", NULL, line_number);
	if (!_checkFinalState(machine, file, line_number)) /* Check if the final state is alright */
		ValidationException(machine, file, "machine", "the final state is not part of the alphabet.", NULL, line_number);
	if (!_checkAtLeastOnePathExists(machine)) /* Check if the final state has any chance to be reached */
	{
		ValidationException(machine, file, "machine",
			"no matter the Data, there is no path available from the initial State of your Machine to its final State.",
			NULL, line_number);
	}
}

