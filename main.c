#define MAX_FILENAME_LENGTH 100

#include <stdio.h>
#include <stdlib.h>

int
main()
{
	char machineFilename[MAX_FILENAME_LENGTH];
	char dataFilename[MAX_FILENAME_LENGTH];
	FILE * machineFile = NULL;
	FILE * dataFile = NULL;

	do /* Main loop */
	{
		printf("Where is the file describing your turing machine ?\n");
		if (!scanf("%s", machineFilename) || (machineFile = fopen(machineFilename, "r")) == NULL)
		{
			printf("Failed to read file: %s, exiting...\n", machineFilename);
			return EXIT_FAILURE;
		}
		printf("Now using machine: %s\n", machineFilename);

		do /* Loop for using the same machine with various data */
		{
			printf("Where is the file containing the data for your machine ?\n");
			if (!scanf("%s", dataFilename) || (dataFile = fopen(machineFilename, "r")) == NULL)
			{
				printf("Failed to read file: %s, exiting...\n", dataFilename);
				return EXIT_FAILURE;
			}
			printf("Now using data from: %s\n", dataFilename);
			getchar(); /* Clear buffer */
			fclose(dataFile);
			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		getchar(); /* Clear buffer */
		fclose(machineFile);
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
