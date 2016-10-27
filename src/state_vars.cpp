#include "state_vars.h"

const float INVALID = -10000000;

state_vars::state_vars(int s){
	valid_actions = (1 << 16 - 1);
	validate_actions(s);
}


inline void state_vars::constraint_base_flr(int s){
	// lift 1 on ground floor
	if(pos_lift1(s) == 0){
		// lift down actions invalid
	}
	// lift 2 on ground floor
	if(pos_lift2(s) == 0){

	}
}

inline void state_vars::constraint_top_flr(int s){
	// lift 1 on top floor
	if(pos_lift1(s) == top_floor){

	}
	// lift 2 on top floor
	if(pos_lift2(s) == top_floor){

	}
}

inline void state_vars::constraint_open_lift(int s){
	// button for floor f pressed in lift 1
	if(button_lift1(s) & (1 << pos_lift1(s)) != 0){
		// Only allow opening lift
	}
	// button for floor f pressed in lift 2
	if(button_lift2(s) & (1 << pos_lift2(s)) != 0){
		// Only allow opening lift
	}
}

inline void state_vars::constraint_move_up_lift(int s){
	// all pressed buttons above current floor of the lift
	if(button_lift1(s) & (1 << pos_lift1(s) - 1) == 0){
		// can't move down, stay still
	}
	if(button_lift2(s) & (1 << pos_lift2(s) - 1) == 0){
		
	}
}

inline void state_vars::constraint_move_down_lift(int s){
	// all pressed buttons below current floor of the lift
	if(button_lift1(s) >> pos_lift1(s) == 0){
		// can't move up
	}
	if(button_lift2(s) >> pos_lift2(s) == 0){
		
	}
}

inline void state_vars::constraint_open_up_lift(int s){
	// all pressed above current floor and button up at current floor
	if(button_lift1(s) & (1 << pos_lift1(s) - 1) == 0 && floor_up(s) & (1 << pos_lift1(s)) != 0){
		// ONLY open up here
	}
	if(button_lift2(s) & (1 << pos_lift2(s) - 1) == 0 && floor_up(s) & (1 << pos_lift2(s)) != 0){
		// ONLY open up here
	}
}

void state_vars::validate_actions(int s, state_vars &val){
	constraint_base_flr(s,val);
	constraint_top_flr(s,val);
	constraint_open_lift(s,val);
}






