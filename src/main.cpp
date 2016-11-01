#include "state.h"

int N, K;
float p, q, r, t;
unordered_map<state, state_vars> m;

int main(int argc, char const *argv[])
{
	// cin >> N >> K >> p >> q >> r >> t;
	
	state start_state = 0;
	print_state(start_state);
	string x = "B_1_2";
	auto pos1 = x.find('_') + 1;
	auto pos2 = x.find('_', pos1);
	cout << (x.substr(pos2+1)) << endl;
	cout << (x.substr(pos1, pos2-pos1)) << endl;

	auto temp = map(start_state);

	return 0;
}


// Construct simulator
// Call that function D times
// Last mein get that vector of costs
// And then update them