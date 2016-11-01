#include "state.h"
#include "state_vars.h"

int N, K;
float p, q, r, t;
unordered_map<state, state_vars> m;
int total_explored_states = 0;

int main(int argc, char const *argv[])
{
	// cin >> N >> K >> p >> q >> r >> t;
	N = 5;
	K = 2;
	p = 0.7;
	q = 0.25;
	r = 0.25;
	
	state start_state = 0;
	print(start_state);
	manage_simulation();

	// observation_floor_up(1, start_state);
	// observation_floor_down(1,start_state);
	// // observation_floor_down(3, start_state);
	// // observation_floor_down(3, start_state);
	// // observation_lift_press(2, 2, start_state);
	// action_lift_up(2, start_state);
	// // action_lift_up(2, start_state);
	// // action_lift_down(2, start_state);
	// action_lift_open_up(2,start_state);
	// observation_lift_press(2,2,start_state);
	// action_lift_up(2,start_state);
	// // action_lift_open_up(2,start_state);
	// print(start_state);

	return 0;
}


// Construct simulator
// Call that function D times
// Last mein get that vector of costs
// And then update them