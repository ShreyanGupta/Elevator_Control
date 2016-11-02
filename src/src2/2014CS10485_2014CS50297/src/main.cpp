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

	return 0;
}