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

bool
_validateLetter(Letter letter, Machine * machine)
{
	int count;
	for (count = 0 ; count < machine->alphabet_length ; ++count)
	{
		if (strcmp(letter, machine->alphabet[count]) == 0)
			return true;
	}
	return false;
}

bool
_validateState(Letter letter, Machine * machine)
{
	int count;
	for (count = 0 ; count < machine->states_length ; ++count)
	{
		if (strcmp(letter, machine->states[count]) == 0)
			return true;
	}
	return false;
}

bool
_validateTransition(Transition transition, Machine * machine)
{
	return (_validateLetter(transition.cond, machine)
		&& _validateLetter(transition.subst, machine)
		&& _validateState(transition.start_state, machine)
		&& _validateState(transition.next_state, machine));
}

bool
_checkTransitions(Machine * machine)
{
	int count;
	for (count = 0 ; count < machine->transitions_length ; ++count)
	{
		if (!_validateTransition(machine->transitions[count], machine))
			return false;
	}
	return true;
}

bool
_checkInitialStateDeparture(Machine * machine)
{
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
	if (!_validateState(machine->initial_state, machine))
		return false;
	if (!_checkInitialStateDeparture(machine))
		MalformedFileException(machine, NULL, "cannot leave the initial state.");
	return true;
}

bool
_checkFinalStateReachability(Machine * machine)
{
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
	if (!_validateState(machine->final_state, machine))
		return false;
	if (!_checkFinalStateReachability(machine))
		MalformedFileException(machine, NULL, "the final state is unreachable.");
	return true;
}

void
validate(Machine * machine)
{
	if (!_checkTransitions(machine))
		MalformedFileException(machine, NULL, "a bad transition has been found.");
	if (!_checkInitialState(machine))
		MalformedFileException(machine, NULL, "the initial state is not part of the alphabet.");
	if (!_checkFinalState(machine))
		MalformedFileException(machine, NULL, "the final state is not part of the alphabet.");
}

void
validateData(Machine * machine)
{
	int count;
	Letter current_letter = NULL;
	for (count = 0 ; count < machine->data->data_length ; ++count)
	{
		current_letter = machine->data->data[count];
		if (!_validateLetter(current_letter, machine))
		{
			String reason = (String) malloc((38 + strlen(current_letter)) * sizeof(char));
			sprintf(reason, "%s is not part of the Machine alphabet.", current_letter);
			MalformedFileException(machine, NULL, reason);
		}
	}
}
