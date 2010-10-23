/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include "types.h"
#include <stdio.h>

/* We couldn't read a file */
void NoSuchFileException(const String filename);
/*
 * There was a syntax error in the file (machine and file are optional)
 * machine and file are optional and can be NULL
 */
void MalformedFileException(Machine *, FILE *, const String reason);
/*
 * There was a malformed transition in the file
 * machine and file are optional and can be NULL
 * reason can be NULL for a generic one
 */
void BadTransitionException(Machine *, FILE *, const String reason);

/* We reached the max steps number,consider the Machine as broken */
void TooMuchStepsException(Machine *);

#endif /* __EXCEPTIONS_H__ */
