#define MAX_FILENAME_LENGTH 100

#include "machine.h"
#include "data.h"
#include <stdlib.h>
#include <stdio.h>

int
main()
{
	char machineFilename[MAX_FILENAME_LENGTH];
	char dataFilename[MAX_FILENAME_LENGTH];
	Machine * machine = NULL;
	Data * data = NULL;

	do /* Main loop */
	{
		printf("Where is the file describing your turing machine ?\n");
		if (!scanf("%s", machineFilename) || (machine = newMachine(machineFilename)) == NULL)
		{
			printf("Failed to read file: %s, exiting...\n", machineFilename);
			return EXIT_FAILURE;
		}
		printf("Now using machine: %s\n", machineFilename);

		do /* Loop for using the same machine with various data */
		{
			printf("Where is the file containing the data for your machine ?\n");
			if (!scanf("%s", dataFilename) || (data = newData(dataFilename)) == NULL)
			{
				printf("Failed to read file: %s, exiting...\n", dataFilename);
				return EXIT_FAILURE;
			}
			printf("Now using data from: %s\n", dataFilename);
			getchar(); /* Clear buffer */
			free(data);
			data = NULL;
			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		getchar(); /* Clear buffer */
		free(machine);
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
