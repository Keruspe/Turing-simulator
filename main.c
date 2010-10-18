#include "machine.h"

int
main()
{
	Machine * machine = NULL;

	do /* Main loop */
	{
		machine = newMachine();
		do /* Loop for using the same machine with various data */
		{
			reloadData(machine);

			/* TODO */
int tester;
printf("alphabet: ");
for (tester = 0 ; tester < machine->alphabet_length ; ++tester)
	printf("%s ", machine->alphabet[tester]);
printf("\n");
printf("states: ");
for (tester = 0 ; tester < machine->states_length ; ++tester)
	printf("%s ", machine->states[tester]);
printf("\n");
printf("data: ");
for (tester = 0 ; tester < machine->data->data_length ; ++tester)
	printf("%s ", machine->data->data[tester]);
printf("\n");

			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		clearBuffer(); /* Clear buffer */
		freeMachine(machine);
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
