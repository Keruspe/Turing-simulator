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

void
validate(Machine * machine)
{
	if (!_checkFinalStateReachability(machine))
		MalformedFileException(machine, NULL, "the final state is unreachable.");
}

void
validateData(Machine * machine)
{
	int data_count, alphabet_count;
	bool valid;
	for (data_count = 0 ; data_count < machine->data->data_length ; ++data_count)
	{
		valid = false;
		for (alphabet_count = 0 ; alphabet_count < machine->alphabet_length ; ++alphabet_count)
		{
			if (strcmp(machine->data->data[data_count], machine->alphabet[alphabet_count]) == 0)
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			String reason = (String) malloc((38 + strlen(machine->data->data[data_count])) * sizeof(char));
			sprintf(reason, "%s is not part of the Machine alphabet.", machine->data->data[data_count]);
			MalformedFileException(machine, NULL, reason);
		}
	}
}
