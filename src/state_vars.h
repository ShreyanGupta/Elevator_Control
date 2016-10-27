#ifndef STATE_VARS_H
#define STATE_VARS_H

// 0 -> move up
// 1 -> move down
// 2 -> open up
// 3 -> open down

// 4 -> stay still

#include "header.h"

struct state_vars{
	float Q[4][4];
	int num_actions[4][4];
	
	state_vars(int);
	void validate_actions(int s);
	void constraint_base_flr(int s);
	void constraint_top_flr(int s);
	void constraint_open_lift(int s);
	void constraint_move_up_lift(int s);
	void constraint_move_down_lift(int s);
};

#endif