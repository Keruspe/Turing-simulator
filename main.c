/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "data.h"
#include "exceptions.h"
#include "machine.h"

int
main()
{
	Machine * machine = NULL;

	do /* Main loop */
	{
		/* Get a new Machine */
		machine = newMachine();

		/* Validate the Machine */
		validate(machine);

		do /* Loop for using the same machine with various data */
		{
			/* Get new Data for our Machine */
			reloadData(machine);

			/* Only debug output for developping */
			debug(machine);

			/* Execute the Machine */
			execute(machine);

			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		clearBuffer(); /* Clear buffer */
		freeMachine(machine); /* No memory leak */
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
