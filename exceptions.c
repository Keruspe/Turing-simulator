/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"
#include "utils.h"
#include <string.h>

void
_Exception(String reason)
{
	/* The mother of each exception, prints the error, free the memory and exit */
	fprintf(stderr, "Exception: %s\nExiting...\n", reason);
	free(reason);
	exit(EXIT_FAILURE);
}

void
NoSuchFileException(const String filename)
{
	/* Gives a precise reason to _Exception */
	String full_reason = (String) malloc((22 + strlen(filename)) * sizeof(char));
	sprintf(full_reason, "failed to read file: %s", filename);
	_Exception(full_reason);
}

void
MalformedFileException(Machine * machine, FILE * file, const String reason)
{
	if (machine) /* If we've a machine to free, free it */
		freeMachine(machine);
	if (file) /* If we've a file to close, close it */
		fclose(file);
	/* Gives a precise reason to _Exception */
	String full_reason = (String) malloc ((17 + strlen(reason)) * sizeof(char));
	sprintf(full_reason, "malformed file, %s", reason);
	_Exception(full_reason);
}

void
_BadTransitionException(Machine * machine, FILE * file)
{
	/* Gives a precise reason to MalformedFileException */
	MalformedFileException(machine, file, "a malformed transition has been found.\nExpected: State Letter Letter State Move");
}

void
BadTransitionException(Machine * machine, FILE * file, const String reason)
{
	/* Gives a generic or a custom reason to MalformedFileException */
	if (reason == NULL)
		_BadTransitionException(machine, file);
	else
		MalformedFileException(machine, file, reason);
}

void
TooMuchStepsException(Machine * machine)
{
	/* Machine was probably in an infinite loop, abort */
	free(machine);
	String reason = (String) malloc(100 * sizeof(char));
	sprintf(reason, "your Machine needed too much steps to execute (max allowed: %d), considerating it as broken.", MAX_STEPS);
	_Exception(reason);
}
