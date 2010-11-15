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
 * There was a syntax error in the file (machine and file are optional)
 * machine and file are optional and can be NULL
 */
void MalformedFileException(Machine *, FILE *, const String reason, unsigned int line_number);

/**
 * There was a malformed transition in the file
 * machine and file are optional and can be NULL
 * reason can be NULL for a generic one
 */
void BadTransitionException(Machine *, FILE *, const String reason, unsigned int line_number);

/* Exception encountered during runtime */
void RuntimeException(Machine *, const String reason);

/* Exception encountered during runtime, really should never have been hit */
void UnexpectedRuntimeException(Machine *, const String reason);

/* No transition matching our current State was found */
void NoSuchTransitionException(Machine *, const State, const Letter);

/* We reached the max steps number,consider the Machine as broken */
void TooMuchStepsException(Machine *);

/* We needed to use the default Letter but it was not in the Machine's alphabet */
void DefaultLetterException(Machine *);

/*
 * Validation of "what" failed
 * "what" should be either "machine" or "data"
 */
void ValidationException(Machine *, const String what, const String reason);

#endif /* __EXCEPTIONS_H__ */
