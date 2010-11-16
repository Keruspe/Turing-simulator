/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"
#include "utils.h"
#include <string.h>

void
_Exception(const String prefix, String reason)
{
	/* The mother of each exception, prints the error, free the memory and exit */
	printf("\n===========================================\n\n");
	fprintf(stderr, "%s: %s\nExiting...\n", prefix, reason);
	free(reason);
	exit(EXIT_FAILURE);
}

void
NoSuchFileException(const String filename)
{
	/* Gives a precise reason to _Exception */
	String full_reason = (String) malloc((22 + strlen(filename)) * sizeof(char));
	sprintf(full_reason, "failed to read file: %s", filename);
	_Exception("Exception", full_reason);
}

void
_MalformedFileException(Machine * machine, FILE * file, const String reason, unsigned int line_number, bool to_free)
{
	if (machine != NULL) /* If we've a machine to free, free it */
		freeMachine(machine);
	if (file != NULL) /* If we've a file to close, close it */
		fclose(file);
	/* Gives a precise reason to _Exception */
	String full_reason = (String) malloc ((25 + getUnsignedIntegerLength(line_number) + strlen(reason)) * sizeof(char));
	sprintf(full_reason, "malformed file, line %d: %s", line_number, reason);
	if (to_free)
		free(reason);
	_Exception("Exception", full_reason);
}

void
MalformedFileException(Machine * machine, FILE * file, const String reason, unsigned int line_number)
{
	_MalformedFileException(machine, file, reason, line_number, false);
}

void
_BadTransitionException(Machine * machine, FILE * file, const Element * malformed, unsigned int line_number)
{
	/* Gives a precise generic reason to MalformedFileException */
	String reason = (String) malloc((145 + strlen(*malformed)) * sizeof(char));
	sprintf(reason, "a malformed transition has been found.\n%s\n%s %s",
		"Expected: State Letter Letter State Move (Must be values recognized by the Machine)",
		"Did not understand:", *malformed);
	_MalformedFileException(machine, file, reason, line_number, true);
}

void
BadTransitionException(Machine * machine, FILE * file, const String reason, const Element * malformed, unsigned int line_number)
{
	/* Gives a generic or a custom reason to MalformedFileException */
	if (reason != NULL)
		MalformedFileException(machine, file, reason, line_number);
	else
		_BadTransitionException(machine, file, malformed, line_number);
}

void
RuntimeException(Machine * machine, String reason)
{
	freeMachine(machine);
	String full_reason = (String) malloc((18 + strlen(reason)) * sizeof(char));
	sprintf(full_reason, "machine failure: %s", reason);
	free(reason);
	_Exception("Runtime exception", full_reason);
}

void
NoSuchTransitionException(Machine * machine, const State state, const Letter letter)
{
	String reason = (String) malloc((62 + strlen(letter) + strlen(state)) * sizeof(char));
	sprintf(reason, "no matching transition found, was in state: \"%s\", found letter: \"%s\"", state, letter);
	RuntimeException(machine, reason);
}

void
TooMuchStepsException(Machine * machine)
{
	/* Machine was probably in an infinite loop, abort */
	String reason = (String) malloc(100 * sizeof(char));
	sprintf(reason, "your Machine needed too much steps to execute (max allowed: %d).", MAX_STEPS);
	RuntimeException(machine, reason);
}

void
DefaultLetterException(Machine * machine)
{
	/* Give an exception sepcifying the default Letter */
	String reason = (String) malloc((115 + strlen(DEFAULT_LETTER)) * sizeof(char));
	sprintf(reason,
		"the alphabet of your Machine didn't contain the default Letter (%s) and it was needed to get out of the data range.",
		DEFAULT_LETTER);
	RuntimeException(machine, reason);
}

void
ValidationException(Machine * machine, const String what, const String reason)
{
	freeMachine(machine);
	String full_reason = (String) malloc((strlen(reason) + 37) * sizeof(char));
	sprintf(full_reason, "your %s failed the validation: %s", what, reason);
	_Exception("Validation exception", full_reason);
}
