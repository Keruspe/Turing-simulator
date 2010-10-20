/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "types.h"

/* Structure to store a Transition */
struct _Transition {
	/* The State in which the Machine is before the Transition */
	State start_state;
	/* The Letter that must be in the box at the right of the head to apply the Transition */
	Letter cond;
	/* The Letter which will replace the previous one in the box at the right of the head after the Transition */
	Letter subst;
	/* The State in which the Machine is after the Transition */
	State next_state;
	/* Where will we go after the Transition ? R(ight) or L(eft) ? */
	Move move;
};

/* Structure to store a Machine */
struct _Machine {
	/* Array containing the alphabet recognized by the Machine */
	Letter * alphabet;
	/* Array containing the states in which the Machine can be */
	State * states;
	/* Array containing pointers to the transitions the Machine can use */
	Transition * transitions;
	/* The State in which the Machine is at the beginning */
	State initial_state;
	/* The State in which the Machine is at the end */
	State final_state;
	/* Data used by the Machine */
	Data * data;

	/*
	 * Store arrays length too
	 */

	/* Number of Letter in the alphabet */
	int alphabet_length;
	/* Number of states recognized */
	int states_length;
	/* Number of available transitions */
	int transitions_length;
};

/* Instantiate a new Machine */
Machine * newMachine();

/* Free the memory, we don't want any leak */
void freeMachine(Machine *);

/* Load new Data for the Machine */
void reloadData(Machine *);

#endif /* __MACHINE_H__ */
