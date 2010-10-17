#include "machine.h"
#include <stdlib.h>
#include <stdio.h>

int
main()
{
	Machine * machine = NULL;

	do /* Main loop */
	{
		if((machine = newMachine()) == NULL)
			return EXIT_FAILURE;

		do /* Loop for using the same machine with various data */
		{
			reloadData(machine);
			if(machine->data == NULL)
				return EXIT_FAILURE;
			/* TODO */

int tester;
for (tester = 0 ; tester < machine->data->data_length ; ++tester)
	printf("%s ", machine->data->data[tester]);
printf("\n");

			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		while (getchar() != '\n'); /* Clear buffer */
		freeMachine(machine);
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
