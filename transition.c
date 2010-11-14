/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/**
 * Turing-simulator
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 */

 #include "transition.h"

void
initTransition(Transition * transition)
{
	/* Basic initialisation of a Transition */
	transition->start_state = NULL;
	transition->cond = NULL;
	transition->subst = NULL;
	transition->next_state = NULL;
	transition->move = '\0';
}

void
freeTransition(Transition * transition)
{
	/* Free the memory used by a Transition */
	free(transition->start_state);
	if (transition->cond)
		free(transition->cond);
	if (transition->subst)
		free(transition->subst);
	if (transition->next_state)
		free(transition->next_state);
}
