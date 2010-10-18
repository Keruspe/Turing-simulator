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

			printf("Do you want to continue with this machine ? [Y/n]\n");
		} while (getchar() != 'n');

		clearBuffer(); /* Clear buffer */
		freeMachine(machine);
		machine = NULL;
		printf("Do you want to continue with another machine ? [Y/n]\n");
	} while (getchar() != 'n'); /* Only exit on user demand */

	return EXIT_SUCCESS;
}
