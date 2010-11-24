/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include "types.h"

#include <stdio.h>

/* We couldn't read a file */
void NoSuchFileException(const String filename);

/**
 * Validation of "what" failed
 * "what" should be either "machine" or "data"
 */
void ValidationException(Machine *, FILE *, const String what, String reason, const Element malformed, const unsigned int line_number);

/**
 * There was a malformed transition in the file
 * machine and file are optional and can be NULL
 * reason can be NULL for a generic one
 * If reason is NULL, you can still specify which Element was wrong
 * If reason is specified, element is ignored (just put it to NULL)
 */
void BadTransitionException(Machine *, FILE *, const String reason, const Element malformed, const unsigned int line_number);

/**
 * Runtime Exceptions
 */

/* No transition matching our current State was found */
void NoSuchTransitionException(Machine *, const State, const Letter);

/* We reached the max steps number,consider the Machine as broken */
void TooMuchStepsException(Machine *);

/* We needed to use the default Letter but it was not in the Machine's alphabet */
void DefaultLetterException(Machine *);

#endif /* __EXCEPTIONS_H__ */
