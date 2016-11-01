#include "simulator.h"
#include "header.h"

// using namespace std;

void person::update_elevator(int i)
{
	elevator = i;
}

void lift::remove_persons_with_dest()
{
	int temp = people.size();

	for(int i = 0; i<temp; i++)
	{
		person p = people.front();

		people.pop();

		if (p.exit_flr != floor)
			people.push(p);
	}
}

string simulator::parse_and_take_action(string act)
{
	if (act[0] == '0') // starting command
	{
		// return an episode

		string final_obs = "";

		if (random_fraction() < this -> p)
		{
			person new_per(this->n,this->q,this->r);

			building[new_per.entry].push(new_per);

			if (new_per.up)
			{
				if (!press_up[new_per.entry])
				{
					press_up[new_per.entry] = true;
					final_obs += "BU_";
					final_obs += to_string(new_per.entry+1); // start from 1
					final_obs += " ";
				}
				else
				{
					final_obs+="0 ";
				}
			}
			else
			{
				if (!press_down[new_per.entry])
				{
					press_down[new_per.entry] = true;
					final_obs += "BD_";
					final_obs += to_string(new_per.entry+1); // start from 1
					final_obs += " ";
				}
				else
				{
					final_obs+="0 ";
				}
			}
		}
		else
		{
			final_obs+="0 ";
		}

		if (final_obs.back() == ' ')
			final_obs.pop_back();

		return final_obs;
	}
	else
	{
		// parse an action and return an episode
		vector<string> actions;

		string str = "";

		// parse into separate actions
		for(int i = 0; i<act.size(); i++)
		{
			if (i == act.size() - 1)
			{
				str+=act[i];
				actions.push_back(str);
			}

			if (act[i] == ' ')
			{
				actions.push_back(str);
				str = "";
			}
			else
			{
				str+=act[i];
			}
		}

		//compute cost due to number of people in the system
		int count_ppl = 0;

		for(int i = 0; i<this->k; i++)
		{
			count_ppl+= elevators[i].get_person_count();
		}

		for(auto it = building.begin(); it != building.end(); it++)
		{
			count_ppl+=(it->second).size();
		}

		curr_cost += 2*count_ppl; // update waiting cost

		string observations = "";

		// take actions and change state
		for(int i = 0; i<actions.size(); i++)
		{
			if (actions[i].compare(0,2,"AU") == 0) // move up
			{
				int liftid = stoi(actions[i].substr(2)) - 1;
				elevators[liftid].floor++;
				curr_cost +=1;
			}
			else if (actions[i].compare(0,2,"AD") == 0) // move up
			{
				int liftid = stoi(actions[i].substr(2)) - 1;
				elevators[liftid].floor--;
				curr_cost +=1;
			}
			if (actions[i].compare(0,3,"AOU") == 0) // move up
			{
				int liftid = stoi(actions[i].substr(3)) - 1;
				elevators[liftid].remove_persons_with_dest();
				elevators[liftid].unpress_button(elevators[liftid].floor);

				int temp = building[elevators[liftid].floor].size();
				press_up[elevators[liftid].floor] = false;

				for(int j = 0; j<temp; j++)
				{
					person per = building[elevators[liftid].floor].front();
					building[elevators[liftid].floor].pop();

					if (per.entry == elevators[liftid].floor && per.up)
					{
						per.elevator = liftid;
						elevators[liftid].add_person(per);
						if (!elevators[liftid].buttons[per.exit_flr])
						{
							elevators[liftid].buttons[per.exit_flr] = true;
							observations += "B_";
							observations +=	to_string(per.exit_flr+1); // starting from 1
							observations += "_";
							observations += to_string(liftid+1);
							observations += " ";
						}
					}
					else
					{
						building[elevators[liftid].floor].push(per);
					}
				}
			}
			else if (actions[i].compare(0,3,"AOD") == 0) // move up
			{
				int liftid = stoi(actions[i].substr(3)) - 1;
				elevators[liftid].remove_persons_with_dest();
				elevators[liftid].unpress_button(elevators[liftid].floor);

				int temp = building[elevators[liftid].floor].size();
				press_down[elevators[liftid].floor] = false;

				for(int j = 0; j<temp; j++)
				{
					person per = building[elevators[liftid].floor].front();
					building[elevators[liftid].floor].pop();

					if (per.entry == elevators[liftid].floor && !per.up)
					{
						per.elevator = liftid;
						elevators[liftid].add_person(per);
						if (!elevators[liftid].buttons[per.exit_flr])
						{
							elevators[liftid].buttons[per.exit_flr] = true;
							observations += "B_";
							observations +=	to_string(per.exit_flr+1); // starting from 1
							observations += "_";
							observations += to_string(liftid+1);
							observations += " ";
						}
					}
					else
					{
						building[elevators[liftid].floor].push(per);
					}
				}
			}
		}

		//store cost of episode
		cost.push_back(curr_cost);

		// incoming person
		string final_obs = "";

		if (random_fraction() < this -> p)
		{
			person new_per(this->n,this->q,this->r);

			building[new_per.entry].push(new_per);

			if (new_per.up)
			{
				if (!press_up[new_per.entry])
				{
					press_up[new_per.entry] = true;
					final_obs += "BU_";
					final_obs += to_string(new_per.entry+1); // start from 1
					final_obs += " ";
				}
				else
				{
					final_obs+="0 ";
				}
			}
			else
			{
				if (!press_down[new_per.entry])
				{
					press_down[new_per.entry] = true;
					final_obs += "BD_";
					final_obs += to_string(new_per.entry+1); // start from 1
					final_obs += " ";
				}
				else
				{
					final_obs+="0 ";
				}
			}
		}
		else
		{
			final_obs+="0 ";
		}

		final_obs+=observations;

		if (final_obs.back() == ' ')
			final_obs.pop_back();

		return final_obs;
	}
}

void simulator::get_state_details()
{
	cout<<"COST: "<<curr_cost<<endl;
	cout<<"PERSON INFORMATION:"<<endl;

	for(int i = 0; i<n; i++)
	{
		cout<<"FLOOR "<<i+1<<": "<<building[i].size()<<endl;
	}

	cout<<"LIFT INFORMATION: "<<endl;

	for(int i = 0; i<k; i++)
	{
		cout<<"LIFT "<<i+1<<": "<<elevators[i].get_person_count()<<" people at "<<elevators[i].floor<<endl;
	}
}

float random_fraction()
{
	static std::default_random_engine generator (std::time(NULL));	
  	static std::uniform_real_distribution<float> distribution(0.0,1.0);

  	float num = distribution(generator);

  	// return num/INT_MAX;
  	return (float)num/*/(float)INT_MAX*/;
}

float random_int(int a)
{
	static std::default_random_engine generator (std::time(NULL));	
  	static std::uniform_int_distribution<int> distribution(1,a);

  	int num = (distribution(generator) % a) + 1;

  	assert(num<=a);
  	// return num/INT_MAX;
  	return num/*/(float)INT_MAX*/;
}

// int main(int argc, char const *argv[])
// {
// 	// cout<<"yo"<<endl;

// 	simulator sim(5,2,0.7,0.25,0.25);

// 	// string s = "";
// 	char inp[100];
// 	string s = "";
// 	while(cin.getline(inp,100))
// 	{
// 		// cin>>s;
// 		// cout<<s<<endl;
// 		for(int i = 0; inp[i] != '\0'; i++)
// 		{
// 			s+=inp[i];
// 		}

// 		cout<<sim.parse_and_take_action(s)<<endl;
// 		sim.get_state_details();
// 		s.clear();
// 	}

// 	return 0;
// }