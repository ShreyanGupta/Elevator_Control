#include "header.h"

int N, K;
float p, q, r, t;

bool lift_is_empty(int lift_num){
	bool ans = false;
	for(auto b : button_lift[lift_num])
		ans = ans || b;
	return !ans
}

bool button_above_pos(int lift_num){
	bool ans = false;
	for(int i = lift_pos[lift_num]+1; i<N; ++i){
		ans = ans || button_lift[lift_num][i];
	}
	return ans;
}

// bool button_below_pos(int lift_num){
// 	bool ans = false;
// 	for(int i=0; i<lift_pos[lift_num]; ++i){
// 		ans = ans || button_lift[lift_num][i];
// 	}
// 	return ans;
// }

// bool floor_up_above_pos(int pos){
// 	bool ans = false;
// 	for(int i = pos+1; i<N; ++i){
// 		ans = ans || floor_up[i];
// 	}
// 	return ans;
// }

// bool floor_up_below_pos(int pos){
// 	bool ans = false;
// 	for(int i=0; i<pos; ++i){
// 		ans = ans || floor_up[i];
// 	}
// 	return ans;
// }

// bool floor_down_above_pos(int pos){
// 	bool ans = false;
// 	for(int i = pos+1; i<N; ++i){
// 		ans = ans || floor_down[i];
// 	}
// 	return ans;
// }

// bool floor_down_below_pos(int pos){
// 	bool ans = false;
// 	for(int i=0; i<pos; ++i){
// 		ans = ans || floor_down[i];
// 	}
// 	return ans;
// }

// int upper_floor_up(int lift_num){
// 	int ans = -1;
// 	for(int i=N-1; i>lift_pos[lift_num]; --i){
// 		if(floor_up[i]) ans = i;
// 	}
// 	return ans;
// }

int upper_floor(int lift_num){
	int ans = -1;
	int pos = lift_pos[lift_num];
	for(int i=N-1; i>pos; --i){
		if(floor_up[i]) ans = i;
		if(floor_down[i]) ans = i;
	}
	return ans;
}

int lower_floor(int lift_num){
	int ans = -1;
	int pos = lift_pos[lift_num];
	for(int i=0; i<pos; ++i){
		if(floor_up[i]) ans = i;
		if(floor_down[i]) ans = i;
	}
	return ans;
}

// int lower_floor_up(int lift_num){
// 	int ans = -1;
// 	for(int i=0; i<lift_pos[lift_num]; ++i){
// 		if(floor_up[i]) ans = i;
// 	}
// 	return ans;
// }

// int upper_floor_down(int lift_num){
// 	int ans = -1;
// 	for(int i=N-1; i>lift_pos[lift_num]; --i){
// 		if(floor_down[i]) ans = i;
// 	}
// 	return ans;
// }

// int lower_floor_down(int lift_num){
// 	int ans = -1;
// 	for(int i=0; i<lift_pos[lift_num]; ++i){
// 		if(floor_down[i]) ans = i;
// 	}
// 	return ans;
// }

// *********************************************

void action_lift_up(int lift_num){
	++lift_pos[lift_num];
}

void action_lift_down(int lift_num){
	--lift_pos[lift_num];
}

void action_lift_open_up(int lift_num){
	floor_up[lift_pos[lift_num]] = false;
	button_lift[lift_num][lift_pos[lift_num]] = false;
}

void action_lift_open_down(int lift_num){
	floor_down[lift_pos[lift_num]] = false;
	button_lift[lift_num][lift_pos[lift_num]] = false;
}

void observation_floor_up(int flr){
	floor_up[flr] = true;
}

void observation_floor_down(int flr){
	floor_down[flr] = true;
}

void observation_lift_press(int lift_num, int flr){
	button_lift[lift_num][flr] = true;
}

// ***************************************************

void update_i_state(string s){
	if(s[0] == '0') return;
	if(s[0] == 'A'){
		switch(s[1]){
			case 'U' : {
				action_lift_up(stoi(s.substr(2)));
				break;
			}
			case 'D' : {
				action_lift_down(stoi(s.substr(2)));
				break;
			}
			case 'O' : {
				if(s[2] == 'U') action_lift_open_up(stoi(s.substr(3)));
				else action_lift_open_down(stoi(s.substr(3)));
				break;
			}
		}
	}
	else if(s[0] == 'B'){
		switch(s[1]){
			case 'U' : {
				observation_floor_up(stoi(s.substr(3)) - 1);
				break;
			}
			case 'D' : {
				observation_floor_down(stoi(s.substr(3)) - 1);
				break;
			}
			default : {
				auto pos1 = s.find('_') + 1;
				auto pos2 = s.find('_', pos1);
				observation_lift_press(stoi(s.substr(pos2+1)), stoi(s.substr(pos1, pos2-pos1)) - 1);
				break;
			}
		}
	}
}

void update_state(string s){
	auto ptr = s.begin();
	while(ptr != s.end()){
		string temp;
		while(ptr != s.end() && *ptr != ' '){
			temp += *ptr;
			++ptr;
		}
		update_i_state(temp);
		if(ptr != s.end()) ++ptr;
	}
	for(int i=0; i<K; ++i) update_lift_state(i);
}

void update_lift_state(int lift_num){
	if(lift_is_empty(lift_num)) lift_state[lift_num] = Vella;
	else if(button_above_pos(lift_num)) lift_state[lift_num] = Up;
	else lift_state[lift_num] = Down;
}

// *******************************************************

string get_lift_action(int lift_num){
	int pos = lift_pos[lift_num];
	string x = to_string(lift_num + 1) + " ";
	switch(lift_state[lift_num]){
		case Vella : {
			// fetch nearest state or open
			if(floor_down[pos]) return "AOD"+x;
			if(floor_up[pos]) return "AOU"+x;
			int u = upper_floor(lift_num);
			int d = lower_floor(lift_num);
			if(u == -1){
				if(d == -1) return "AS"+x;
				else return "AD"+x;
			}
			else{
				if(d == -1) return "AU"+x;
				else if(u < d) return "AU"+x;
				else return "AD"+x;
			}
		}
		case Up : {
			if(floor_up[pos]) return "AOU"+x;
			else if(button_lift[lift_num][pos]) return "AOU"+x;
			else return "AU"+x;
		}
		case Down : {
			if(floor_down[pos]) return "AOD"+x;
			else if(button_lift[lift_num][pos]) return "AOD"+x;
			else return "AD"+x;
		}
	}
}

string get_action(){
	string act;
	for(int i=0; i<K; ++i){
		act += get_lift_action(i);
	}
	return act;
}

// ***************************************************

string get_observation()
{
	char obs[1000];

	cin.getline(obs,1000);
	string observe = "";
	for(int i = 0; obs[i] != '\0'; i++)
	{
		observe+=obs[i];
	}

	return obs;
}

int main(int argc, char const *argv[])
{
	// cin >> N >> K >> p >> q >> r >> t;
	N = 5;
	K = 2;
	p = 0.7;
	q = 0.25;
	r = 0.25;

	vector<int> lift_pos(K, 0);
	vector<Lift_State> lift_state(K, Vella);
	vector<bool> floor_up(N, false);
	vector<bool> floor_down(N, false);
	vector< vector<bool> > button_lift(K, vector<bool>(N, false));

	// take_action("0");
	cout<<"0"<<endl;
	string str = get_observation();
	update_state(str);
	while(ongoing episodes){
		string act = get_action();
		update_state(act);
		cout<<act<<endl;
		string get_obs = get_observation();
		update_state(get_obs);
	}

	return 0;
}