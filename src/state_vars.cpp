#include "state_vars.h"
#include "state.h"

const float INVALID = -10000000;

state_vars::state_vars(int s){
	validate_actions(s);
}

void state_vars::constraint_base_flr(int s){
	// lift 1 on ground floor
	if(pos_lift1(s) == 0){
		// lift down actions invalid
		for(int i=0; i<4; ++i){
			Q[1][i] = INVALID;
			Q[3][i] = INVALID;
		}
	}
	// lift 2 on ground floor
	if(pos_lift2(s) == 0){
		for(int i=0; i<4; ++i){
			Q[i][1] = INVALID;
			Q[i][3] = INVALID;
		}
	}
}

void state_vars::constraint_top_flr(int s){
	// lift 1 on top floor
	if(pos_lift1(s) == N){
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[2][i] = INVALID;
		}
	}
	// lift 2 on top floor
	if(pos_lift2(s) == N){
		for(int i=0; i<4; ++i){
			Q[i][0] = INVALID;
			Q[i][2] = INVALID;
		}
	}
}

void state_vars::constraint_open_lift(int s){
	// button for floor f pressed in lift 1
	if((button_lift1(s) & floor_lift1(s)) != 0){
		// Only allow opening lift
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[1][i] = INVALID;
		}
		// Floor not pressed => only one of open up or open down
		if((floor_up_down(s) & floor_lift1(s)) == 0){
			if(pos_lift1(s) == 0)
			for(int i=0; i<4; ++i){
				Q[3][i] = INVALID;
			}
			else for(int i=0; i<4; ++i){
				Q[2][i] = INVALID;
			}
		}
	}
	// button for floor f pressed in lift 2
	if((button_lift2(s) & floor_lift2(s)) != 0){
		// Only allow opening lift
		for(int i=0; i<4; ++i){
			Q[i][0] = INVALID;
			Q[i][1] = INVALID;
		}
		if((floor_up_down(s) & floor_lift2(s)) == 0){
			if(pos_lift2(s) == 0)
			for(int i=0; i<4; ++i){
				Q[i][3] = INVALID;
			}
			else for(int i=0; i<4; ++i){
				Q[i][2] = INVALID;
			}
		}
	}
}

void state_vars::constraint_move_up_lift(int s){
	// no button pressed below and at current floor of the lift
	// floor up or down or button lift 1 are not pressed at current floor
	if((button_lift1(s) & ((floor_lift1(s) << 1) - 1)) == 0 &&
		(floor_up_down(s) & floor_lift1(s)) == 0)
	{
		// only up is valid
		for(int i=0; i<4; ++i){
			Q[1][i] = INVALID;
			Q[2][i] = INVALID;
			Q[3][i] = INVALID;
		}	
	}
	if((button_lift2(s) & ((floor_lift2(s) << 1) - 1)) == 0 && 
		(floor_up_down(s) & floor_lift2(s)) == 0)
	{
		for(int i=0; i<4; ++i){
			Q[i][1] = INVALID;
			Q[i][2] = INVALID;
			Q[i][3] = INVALID;
		}
	}
}

void state_vars::constraint_move_down_lift(int s){
	// all pressed buttons below current floor of the lift
	if((button_lift1(s) >> pos_lift1(s)) == 0 && 
		(floor_up_down(s) & floor_lift1(s)) == 0)
	{
		// only down is valid
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[2][i] = INVALID;
			Q[3][i] = INVALID;
		}	
	}
	if((button_lift2(s) >> pos_lift2(s)) == 0 &&
		(floor_up_down(s) & floor_lift2(s)) == 0)
	{
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[2][i] = INVALID;
			Q[3][i] = INVALID;
		}	
	}
}

// void state_vars::constraint_open_up_lift(int s){
// 	// all pressed above current floor and button up at current floor
// 	if(button_lift1(s) & (1 << pos_lift1(s) - 1) == 0 && floor_up(s) & (1 << pos_lift1(s)) != 0){
// 		// ONLY open up here
// 	}
// 	if(button_lift2(s) & (1 << pos_lift2(s) - 1) == 0 && floor_up(s) & (1 << pos_lift2(s)) != 0){
// 		// ONLY open up here
// 	}
// }

void state_vars::validate_actions(int s){
	constraint_base_flr(s);
	constraint_top_flr(s);
	constraint_open_lift(s);
	constraint_move_up_lift(s);
	constraint_move_down_lift(s);
}
