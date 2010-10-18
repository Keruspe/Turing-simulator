#include "utils.h"

void
skipLine(FILE * file)
{
	while (fgetc(file) != '\n');
}

void
clearBuffer()
{
	while (getchar() != '\n');
}
