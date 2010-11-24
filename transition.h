/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

#ifndef __TRANSITION_H__
#define __TRANSITION_H__

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

/* Basic initialisation of a Transition */
void initTransition(Transition *);

/* Free the memory used by a Transition */
void freeTransition(Transition *);

#endif /* __TRANSITION_H__ */
