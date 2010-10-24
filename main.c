/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"
#include "utils.h"
#include "validator.h"

int
main()
{
	Machine * machine = NULL;

	do /* Main loop */
	{
		machine = newMachine(); /* Get a new Machine */
		validate(machine); /* Validate the Machine */

		do /* Loop for using the same Machine with various Data sets */
		{
			reloadData(machine); /* Get new Data for our Machine */
			validateData(machine); /* Validate the new Data set */
			execute(machine); /* Execute the Machine */
			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		clearBuffer(); /* Clear buffer */
		freeMachine(machine); /* No memory leak */
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
