/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Macros to improve readability */
#define _validateLetter(x,y) _validateElement(x,y->alphabet,y->alphabet_length)
#define _validateState(x,y) _validateElement(x,y->states,y->states_length)

bool
_validateElement(Element element, ElementsCollection valid_elements, int collection_length)
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
	if (!_validateState(machine->initial_state, machine)) /* Check if the initial state is recognized by the Machine */
		return false;
	if (!_checkInitialStateDeparture(machine)) /* Check if we can leave it */
		ValidationException(machine, "machine", "cannot leave the initial state.");
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
	if (!_validateState(machine->final_state, machine)) /* Check if the final state is recognized by the Machine */
		return false;
	if (!_checkFinalStateReachability(machine)) /* Check if we can reach it */
		ValidationException(machine, "machine", "the final state is unreachable.");
	return true;
}

bool
validateTransition(Transition transition, Machine * machine)
{
	/* Check if all states and letters used by a Transition are known by the Machine */
	return (_validateLetter(transition.cond, machine)
		&& _validateLetter(transition.subst, machine)
		&& _validateState(transition.start_state, machine)
		&& _validateState(transition.next_state, machine));
}

void
validate(Machine * machine)
{
	if (!_checkInitialState(machine)) /* Check if the initial state is alright */
		ValidationException(machine, "machine", "the initial state is not part of the alphabet.");
	if (!_checkFinalState(machine)) /* Check if the final state is alright */
		ValidationException(machine, "machine", "the final state is not part of the alphabet.");
}

void
validateData(Machine * machine)
{
	/* Check if every Letter in the Data set is recognized by the Machine */
	int count;
	Letter current_letter = NULL;
	for (count = 0 ; count < machine->data->data_length ; ++count)
	{
		current_letter = machine->data->data[count];
		if (!_validateLetter(current_letter, machine))
		{
			/* Throw an exception if it's not */
			String reason = (String) malloc((38 + strlen(current_letter)) * sizeof(char));
			sprintf(reason, "%s is not part of the Machine alphabet.", current_letter);
			ValidationException(machine, "data", reason);
		}
	}
}

bool
hasDefaultLetter(Machine * machine)
{
	int count;
	for (count = 0 ; count < machine->alphabet_length ; ++count)
	{
		if(strcmp(machine->alphabet[count], DEFAULT_LETTER) == 0)
			return true; /* If we find the default Letter in the alphabet, return true */
	}
	return false; /* We didn't find it */
}
