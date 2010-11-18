/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__

/* Macros to improve readability */
#define validateLetter(x,y) validateElement(x,y->alphabet,y->alphabet_length)
#define validateState(x,y) validateElement(x,y->states,y->states_length)

bool validateElement(const Element, const ElementsCollection valid_elements, const unsigned int collection_length);

/* Check if the Machine allows the use of the default Letter */
bool hasDefaultLetter(const Machine *);

/* Validate a transition, store the bad Element if malformed */
bool validateTransition(const Transition, const Machine *, String reason, Element * malformed);

/* Validate a Machine */
void validate(Machine *, FILE *, const unsigned int line_number);

#endif /* __VALIDATOR_H__ */
