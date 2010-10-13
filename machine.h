#ifndef __MACHINE_H__
#define __MACHINE_H__

#include "types.h"

struct _Transition {
	State start_state;
	Letter cond;
	Letter subst;
	State next_state;
	Move move;
};

struct _Machine {
	Letter * alphabet;
	State * states;
	Transition * transitions;
	State initial_state;
	State final_state;
	// Store arrays length too
	int alphabet_length;
	int states_length;
	int transitions_length;
};

Machine * newMachine(char *);
void freeMachine(Machine *);

#endif /* __MACHINE_H__ */
