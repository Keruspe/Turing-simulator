/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"
#include <string.h>

int
main()
{
	Machine * machine = NULL;

	do /* Main loop */
	{
		/* Get a new Machine */
		machine = newMachine();

		bool valid = false;
		int i;
		for (i = 0 ; i < machine->transitions_length ; ++i)
		{
			if (strcmp(machine->transitions[i].next_state, machine->final_state) == 0)
			{
				valid = true;
				break;
			}
		}
		if (!valid)
			MalformedFileException(machine, NULL, "the final state is unreachable.");

		do /* Loop for using the same machine with various data */
		{
			/* Get new Data for our Machine */
			reloadData(machine);

			/* Only debug output for developping */
			debug(machine);

			Move move = 'R'; /* First move */
			State current_state = machine->initial_state;
			Letter current_letter = NULL;
			Transition current_transition;
			int transition_iterator, steps=0;

			while (strcmp(current_state, machine->final_state) != 0)
			{
				current_letter = go(machine, move);
				for (transition_iterator = 0 ; transition_iterator < machine->transitions_length ; ++transition_iterator)
				{
					current_transition = machine->transitions[transition_iterator];
					if ((strcmp(current_transition.start_state, current_state) == 0) && (strcmp(current_transition.cond, current_letter) == 0))
					{
						current_state = current_transition.next_state;
						move = current_transition.move;
						setLetter(machine->data, machine->data_index, current_transition.subst);
						break;
					}
				}
				printf("%3d steps\n", ++steps);
			}
			printf("Done\n");

			machine->data_index = machine->data->extra_data_length - 1;
			printf("Result: ");
			while (machine->data_index < machine->data->data_length - 1)
				printf("%s ", go(machine,'R'));
			printf("\n");

			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		clearBuffer(); /* Clear buffer */
		freeMachine(machine); /* No memory leak */
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
