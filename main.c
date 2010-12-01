/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"

int
main()
{
	Machine * machine = NULL;
	bool step_by_step;

	do /* Main loop */
	{
		machine = newMachine(); /* Get a new Machine */

		do /* Loop for using the same Machine with various Data sets */
		{
			reloadData(machine); /* Get new Data for our Machine */
			printf("Do you want to execute the machine step by step ? [y/N]\n");
			step_by_step = !getChoice('y');
			execute(machine, step_by_step); /* Execute the Machine */
			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getChoice('n'));

		freeMachine(machine); /* No memory leak */
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getChoice('n')); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
