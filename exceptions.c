/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#include "machine.h"

#include <string.h>

/**
 * Generic Exceptions
 */

static void
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
	/* Tell which file we were not able to read to _Exception */
	String full_reason = (String) malloc((22 + strlen(filename)) * sizeof(char));
	sprintf(full_reason, "failed to read file: %s", filename);
	_Exception("Exception", full_reason);
}

/**
 * Validation Exceptions
 */

static void
_PreciseValidationException(const String what, const String reason, const Element malformed, const unsigned int line_number)
{
	/* Tell which precise Element at which line failed the validation */
	String full_reason = (String) malloc((57 + strlen(what) + strlen(reason) + strlen(malformed) +
										getUnsignedIntegerLength(line_number)) * sizeof(char));
	sprintf(full_reason, "your %s failed the validation line %d: %s\n%s %s",
			what, line_number, reason, "Did not understand:", malformed);
	_Exception("Validation exception", full_reason);
}

static void
_ValidationException(Machine * machine, FILE * file, const String what, String reason, const Element malformed, const unsigned int line_number, bool to_free)
{
	freeMachine(machine);
	fclose(file);
	if (malformed != NULL) /* If we're given a malformed Element, throw a precise Exception */
		_PreciseValidationException(what, reason, malformed, line_number);
	/* Else tell the reason why validation failed, and at which line */
	String full_reason = (String) malloc((36 + strlen(what) + getUnsignedIntegerLength(line_number) +
										strlen(reason)) * sizeof(char));
	sprintf(full_reason, "your %s failed the validation line %d: %s", what, line_number, reason);
	if (to_free)
		free(reason);
	_Exception("Validation exception", full_reason);
}

void
ValidationException(Machine * machine, FILE * file, const String what, String reason, const Element malformed, const unsigned int line_number)
{
	/* This is just a wrapper, a basic ValidationException doesn't need its reason to be free'd */
	_ValidationException(machine, file, what, reason, malformed, line_number, false);
}

static void
_BadTransitionException(Machine * machine, FILE * file, const Element malformed, const unsigned int line_number)
{
	/* Build a precise reason */
	String reason = (String) malloc((144 + strlen(malformed)) * sizeof(char));
	sprintf(reason, "a malformed transition has been found.\n%s\n%s %s",
		"Expected: State Letter Letter State Move (Must be values recognized by the Machine)",
		"Did not understand:", malformed);
	/* Let _ValidationException forward it to the precise one, and tell it reason needs to be free'd */
	_ValidationException(machine, file, "machine", reason, NULL, line_number, true);
}

void
BadTransitionException(Machine * machine, FILE * file, const String reason, const Element malformed, const unsigned int line_number)
{
	/* If we're not given any reason, it's just a basic ValidationException, won't be a precise one */
	if (reason != NULL)
		ValidationException(machine, file, "machine", reason, NULL, line_number);
	else /* Build a precise Exception that will be redirected to _PreciseValidationException */
		_BadTransitionException(machine, file, malformed, line_number);
}

/**
 * Runtime Exceptions
 */

static void
_RuntimeException(Machine * machine, String reason)
{
	/* Mother of all runtime exceptions, tells _Exception that the one we're dealing with occured during runtime */
	freeMachine(machine);
	String full_reason = (String) malloc((18 + strlen(reason)) * sizeof(char));
	sprintf(full_reason, "machine failure: %s", reason);
	free(reason);
	_Exception("Runtime exception", full_reason);
}

void
NoSuchTransitionException(Machine * machine, const State state, const Letter letter)
{
	/* We were stuck in a State with no Transition to leave it */
	String reason = (String) malloc((65 + strlen(state) + strlen(letter)) * sizeof(char));
	sprintf(reason, "no matching transition found, was in state: \"%s\", found letter: \"%s\"", state, letter);
	_RuntimeException(machine, reason);
}

void
TooMuchStepsException(Machine * machine)
{
	/* Machine was probably in an infinite loop, abort */
	String reason = (String) malloc((62 + getUnsignedIntegerLength(MAX_STEPS)) * sizeof(char));
	sprintf(reason, "your Machine needed too much steps to execute (max allowed: %d)", MAX_STEPS);
	_RuntimeException(machine, reason);
}

void
DefaultLetterException(Machine * machine)
{
	/* Give an exception specifying the default Letter since we needed it and it was not supported by the Machine */
	String reason = (String) malloc((113 + strlen(DEFAULT_LETTER)) * sizeof(char));
	sprintf(reason, "the alphabet of your Machine didn't contain the default Letter (%s) %s",
			DEFAULT_LETTER, "and it was needed to get out of the data range");
	_RuntimeException(machine, reason);
}
