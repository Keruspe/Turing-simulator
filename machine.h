/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "types.h"
#include "utils.h"

/* Structure to store a Machine */
struct _Machine {
	/* Array containing the alphabet recognized by the Machine */
	LettersCollection alphabet;
	/* Array containing the states in which the Machine can be */
	StatesCollection states;
	/* Array containing pointers to the transitions the Machine can use */
	TransitionsCollection transitions;
	/* The State in which the Machine is at the beginning */
	State initial_state;
	/* The State in which the Machine is at the end */
	State final_state;
	/* Data used by the Machine */
	Data * data;
	/* Index we're up to in the data */
	int data_index;

	/**
	 * Store arrays length too
	 */

	/* Number of Letter in the alphabet */
	unsigned int alphabet_length;
	/* Number of states recognized */
	unsigned int states_length;
	/* Number of available transitions */
	unsigned int transitions_length;
};

/* Instantiate a new Machine */
Machine * newMachine();

/* Free the memory, we don't want any leak */
void freeMachine(Machine *);

/* Load new Data for the Machine */
void reloadData(Machine *);

/* Move the Mahcine and return the next value */
Letter go(Machine *, const Move);

/* Execute a Machine */
void execute(Machine *, bool step_by_step);

#endif /* __MACHINE_H__ */
