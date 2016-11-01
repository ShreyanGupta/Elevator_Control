#include "state.h"
#include "state_vars.h"

string to_bin(int n){
	string r;
	for(int i=0; i<5; ++i){
		if(n%2 == 1) r += "1";
		else r += "0";
		n/=2;
	}
	return r;
}
void print(state s){
	cout << "L1: " << pos_lift1(s);
	cout << " L2: " << pos_lift2(s);
	cout << " BL1: " << to_bin(button_lift1(s));
	cout << " BL2: " << to_bin(button_lift2(s));
	cout << " FU: " << to_bin(floor_up(s));
	cout << " FD: " << to_bin(floor_down(s)) << endl;
}

void update_state(string s, state &st){
	if(s[0] == '0') return;
	if(s[0] == 'A'){
		switch(s[1]){
			case 'U' : {
				action_lift_up(stoi(s.substr(2)), st);
				break;
			}
			case 'D' : {
				action_lift_down(stoi(s.substr(2)), st);
				break;
			}
			case 'O' : {
				if(s[2] == 'U') action_lift_open_up(stoi(s.substr(3)), st);
				else action_lift_open_down(stoi(s.substr(3)), st);
				break;
			}
		}
	}
	else if(s[0] == 'B'){
		switch(s[1]){
			case 'U' : {
				observation_floor_up(stoi(s.substr(3)) - 1, st);
				break;
			}
			case 'D' : {
				observation_floor_down(stoi(s.substr(3)) - 1, st);
				break;
			}
			default : {
				auto pos1 = s.find('_') + 1;
				auto pos2 = s.find('_', pos1);
				observation_lift_press(stoi(s.substr(pos2+1)), stoi(s.substr(pos1, pos2-pos1)) - 1, st);
				break;
			}
		}
	}
}

void parse_response(string s, state &st){
	auto ptr = s.begin();
	while(ptr != s.end()){
		string temp;
		while(ptr != s.end() && *ptr != ' '){
			temp += *ptr;
			++ptr;
		}
		update_state(temp, st);
		if(ptr != s.end()) ++ptr;
	}
}

string action_to_string(pair<int, int> &act){
	string str_action;
	switch(act.first){
		case 0 : str_action += "AU1 "; break;
		case 1 : str_action += "AD1 "; break;
		case 2 : str_action += "AOU1 "; break;
		case 3 : str_action += "AOD1 "; break;
	}
	switch(act.second){
		case 0 : str_action += "AU2"; break;
		case 1 : str_action += "AD2"; break;
		case 2 : str_action += "AOU2"; break;
		case 3 : str_action += "AOD2"; break;
	}
	return str_action;
}

state_vars &map(state &s){
	auto itr = m.find(s);
	if(itr == m.end()){
		itr = m.insert(make_pair(s, state_vars(s))).first;
		// ++total_explored_states;
		// cout << total_explored_states << endl;
	}
	return itr->second;
}
