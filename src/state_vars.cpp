#include "state_vars.h"
#include "state.h"
#include "simulator.h"

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
	if(pos_lift1(s) == N-1){
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[2][i] = INVALID;
		}
	}
	// lift 2 on top floor
	if(pos_lift2(s) == N-1){
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
	// if((button_lift1(s) & ((floor_lift1(s) << 1) - 1)) == 0 &&
	// 	(button_lift1(s) >> pos_lift1(s) != 0) &&
	// 	(floor_up_down(s) & floor_lift1(s)) == 0)
	if(	((button_lift1(s) | floor_up_down(s)) & 
		((floor_lift1(s) << 1) - 1)) == 0 &&
		((button_lift1(s) | floor_up_down(s)) >> (pos_lift1(s) + 1)) != 0
	)
	{
		// only up is valid
		for(int i=0; i<4; ++i){
			Q[1][i] = INVALID;
			Q[2][i] = INVALID;
			Q[3][i] = INVALID;
		}	
	}
	// if((button_lift2(s) & ((floor_lift2(s) << 1) - 1)) == 0 && 
	// 	(button_lift2(s) >> pos_lift2(s) != 0) &&
	// 	(floor_up_down(s) & floor_lift2(s)) == 0)
	if(	((button_lift2(s) | floor_up_down(s)) & 
		((floor_lift2(s) << 1) - 1)) == 0 &&
		((button_lift2(s) | floor_up_down(s)) >> (pos_lift2(s) + 1)) != 0
	)
	{
		for(int i=0; i<4; ++i){
			Q[i][1] = INVALID;
			Q[i][2] = INVALID;
			Q[i][3] = INVALID;
		}
	}
}

// if( 
// 	(button_lift1(s) | floor_up_down(s)) & (floor_lift1(s) - 1) != 0 &&
// 	(button_lift1(s) | floor_up_down(s)) >> pos_lift1(s) == 0
//   )

void state_vars::constraint_move_down_lift(int s){
	// all pressed buttons below current floor of the lift
	// if((button_lift1(s) >> pos_lift1(s)) == 0 && 
	// 	(floor_up_down(s) & floor_lift1(s)) == 0)
	if(	((button_lift1(s) | floor_up_down(s)) & (floor_lift2(s) - 1)) != 0 &&
		((button_lift1(s) | floor_up_down(s)) >> pos_lift2(s)) == 0
	)
	{
		// only down is valid
		for(int i=0; i<4; ++i){
			Q[0][i] = INVALID;
			Q[2][i] = INVALID;
			Q[3][i] = INVALID;
		}	
	}
	if(	((button_lift2(s) | floor_up_down(s)) & (floor_lift2(s) - 1)) != 0 &&
		((button_lift2(s) | floor_up_down(s)) >> pos_lift2(s)) == 0
	)
	// if((button_lift2(s) >> pos_lift2(s)) == 0 &&
	// 	(floor_up_down(s) & floor_lift2(s)) == 0)
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
	// constraint_move_up_lift(s);
	// constraint_move_down_lift(s);
}

// verify this function
pair<int,int> state_vars::best_actions(){
	pair<int, int> action;
	float best_val = INVALID;
	for(int i=0; i<4; ++i){
		for(int j=0; j<4; ++j){
			if(Q[i][j] == INVALID) continue;
			if(num_actions[i][j] == 0){
				action.first = i;
				action.second = j;
				return action;
			}
			float temp = Q[i][j] - Q_avg * sqrt(2*log(num_total_actions) / num_actions[i][j]);
			// float temp = Q[i][j]*(1 - sqrt(2*log(num_total_actions) / num_actions[i][j]));
			// float temp = Q[i][j] - Q_avg * sqrt(2*sqrt(num_total_actions) / num_actions[i][j]);
			if(temp < best_val){
				best_val = temp;
				action.first = i;
				action.second = j;
			}
		}
	}
	return action;
}

pair<int,int> state_vars::the_best_action(){
	pair<int, int> action = make_pair(-1,-1);
	float best_val = INVALID;
	for(int i=0; i<4; ++i){
		for(int j=0; j<4; ++j){
			if(Q[i][j] == INVALID) continue;
			if(num_actions[i][j] == 0){
				cout << "UNEXPLORED!!!!\n";
				action.first = i;
				action.second = j;
				return action;
			}
			if(Q[i][j] < best_val){
				best_val = Q[i][j];
				action.first = i;
				action.second = j;
			}
		}
	}
	if(action.first == -1) cout << "EEEEEEEEEEERRRRRRRRRROOOOOOOOOOORRRRRRRRRRRRRRR!!!!!!!!!!!!!\n";
	return action;
}

// verify this function
void state_vars::update_action(pair<int, int> &a, float cost){
	++num_actions[a.first][a.second];
	++num_total_actions;
	Q_avg += (cost - Q_avg) / num_total_actions;
	Q[a.first][a.second] += (cost - Q[a.first][a.second]) / num_actions[a.first][a.second];
	// printf("Action %d %d Q_av %f N_t %d Q %f N %d\n", a.first, a.second, Q_avg, num_total_actions, Q[a.first][a.second], num_actions[a.first][a.second]);
}

// *************************** NON State vars functions

void simulate(int depth, bool actual){
	simulator sim(N, K, p, q, r);
	vector <pair<state, pair<int, int> > > action_list;
	state s = 0;
	string response = sim.parse_and_take_action("0");
	parse_response(response, s);
		if(actual) sim.get_state_details();
		if(actual) cout << "!!!!!!!!!!" << response << endl;
	for(int i=0; i<depth; ++i){
			if(actual) print(s);
		pair<int,int> action = map(s).best_actions(); 
		string str_action = action_to_string(action);
		action_list.push_back(make_pair(s, action));
		
		parse_response(str_action, s);
			if(actual) sim.get_state_details();
			if(actual) cout << str_action << endl;
		string response = sim.parse_and_take_action(str_action);
			if(actual) cout << "!!!!!!!!!!" << response << endl;
		parse_response(response, s);
	}
	auto cost_list = sim.get_cost_vector();

	for(int i=0; i<depth; ++i){
		// map(action_list[i].first).update_action(action_list[i].second, cost_list[i] / (i+1));
		map(action_list[i].first).update_action(action_list[i].second, cost_list[depth-1] / depth);
	}
	cout << (cost_list[depth - 1] / depth) << endl;
}

void manage_simulation(){
	time_t start = time(0);
	int depth = 500;
	while(time(0) - start < 60)
	{
		simulate(depth, false);
	}
	cout << "ACTUAL\n";
	for(int i=0; i<1; ++i){
		simulate(30, true);
	}
}






