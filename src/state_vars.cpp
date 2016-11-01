#include "state_vars.h"
#include "state.h"
#include "simulator"

const float INVALID = +10000000;

state_vars::state_vars(int s){
	Q_avg = 0;
	num_total_actions = 0;
	for(int i=0; i<4; ++i){
		for(int j=0; j<4; ++j){
			Q[i][j] = 0;
			num_actions[i][j] = 0;
		}
	}
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

// verify this function
pair<int,int> state_vars::best_actions(){
	pair<int, int> action;
	float best_val = INVALID;
	for(int i=0; i<4; ++i){
		for(int j=0; j<4; ++j){
			if(Q[i][j] == INVALID) continue;
			float temp = Q[i][j] - Q_avg * sqrt(2*log(num_total_actions) / num_actions[i][j]);
			if(temp < best_val){
				best_val = temp;
				action.first = i;
				action.second = j;
			}
		}
	}
	return action;
}

// verify this function
void state_vars::update_action(pair<int, int> &a, float cost){
	++num_actions[a.first][a.second];
	++num_total_actions;
	Q_avg += cost / num_total_actions;
	Q[a.first][a.second] += cost / num_actions[a.first][a.second];
}

// *************************** NON State vars functions

void simulate(int depth){
	simulator sim(N, K, p, q, r);
	vector <pair<state, pair<int, int> > > action_list;
	state s = 0;
	parse_response(parse_and_take_action("0"), s);
	for(int i=0; i<depth; ++i){
		auto action = map(s).best_actions();
		action_list.push_back(make_pair(s, action));
		string str_action = action_to_string(action);
		string response = sim.parse_and_take_action(str_action);
		parse_response(response, s);
	}
	auto cost_list = sim.get_cost_vector();

	for(int i=0; i<depth; ++i){
		map(action_list[i].first).update_action(action_list[i].second, cost_list[i]);
	}
}

// Yahaan kya karna hai???
void manage_simulation(){
	time_t start = time(0);
	int depth = 100;
	while(time(0) - start < 30*60*60){
		simulate(depth);
	}
}






