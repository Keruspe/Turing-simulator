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
_validateLetter(Letter letter, LettersCollection valid_letters, int valid_letters_length)
{
	int count;
	for (count = 0 ; count < valid_letters_length ; ++count)
	{
		if (strcmp(letter, valid_letters[count]) == 0)
			return true;
	}
	return false;
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
	if (!_validateLetter(machine->final_state, machine->alphabet, machine->alphabet_length))
		return false;
	if (!_checkFinalStateReachability(machine))
		MalformedFileException(machine, NULL, "the final state is unreachable.");
	return true;
}

void
validate(Machine * machine)
{
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
		if (!_validateLetter(current_letter, machine->alphabet, machine->alphabet_length))
		{
			String reason = (String) malloc((38 + strlen(current_letter)) * sizeof(char));
			sprintf(reason, "%s is not part of the Machine alphabet.", current_letter);
			MalformedFileException(machine, NULL, reason);
		}
	}
}
