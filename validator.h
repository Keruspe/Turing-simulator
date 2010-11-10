/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

/* Check if the Machine allows the use of the default Letter */
bool hasDefaultLetter(Machine *);

/* Validate a transition */
bool validateTransition(Transition, Machine *);

/* Validate a Machine */
void validate(Machine *);

/* Validate a Data set for a Machine */
void validateData(Machine *);

#endif /* __VALIDATOR_H__ */
