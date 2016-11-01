#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "header.h"

float random_fraction();
float random_int(int n);

class person
{
	public:	
		// int userid = -1;
		int elevator = -1;
		int entry = -1;
		int exit_flr = -1;
		bool up = false;

		person(/*int id, */int n, float q, float r)
		{
			// userid = id;

			if (random_fraction() < q)
			{
				entry = 0;
			}
			else
			{
				entry = random_int(n-1);
			}

			if (entry == 0)
			{
				exit_flr = random_int(n-1);
			}
			else
			{
				if (random_fraction() < r)
					exit_flr = 0;
				else
				{
					int temp = random_int(n-2);

					if (temp < entry)
						exit_flr = temp;
					else
						exit_flr = temp+1;
				}
			}

			if (exit_flr > entry)
				up = true;

			// cout<<"********************************** "<<n<<" EXIT: "<<exit_flr<<" ENTRY: "<<entry<<endl;
		}

		// ~person();
		
		void update_elevator(int i);
};

class lift
{
	public:

		int id = 0; // id starts from 1
		int floor = 0;

		vector<bool> buttons; //which floor buttons pressed
		bool is_up = false;

		queue<person> people;

		lift(int l_id, int n)
		{
			id = l_id;

			for(int i = 0; i<n; i++)
			{
				buttons.push_back(false);
			}
		}

		// ~lift();
		
		void change_direction(bool up)
		{
			is_up = up;
		}

		void add_person(person& p)
		{
			people.push(p);
		}

		void remove_persons_with_dest();
		void press_button(int f_no) // f_no starts form 0
		{
			buttons[f_no] = true;
		}

		void unpress_button(int f_no) // f_no starts form 0
		{
			buttons[f_no] = false;
		}

		int get_person_count()
		{
			return people.size();
		}
};

class simulator
{

	public:

		int n = -1;
		int k = -1;
		float p = 0;
		float q = 0;
		float r = 0;

		unordered_map< int,queue<person> > building;
		vector<int> cost;

		int curr_cost = 0;

		vector<lift> elevators;

		vector<bool> press_up;
		vector<bool> press_down;

		simulator(int n, int k, float p, float q, float r)
		{
			this -> n = n;
			this -> p = p;
			this -> q = q;
			this -> r = r;
			this -> k = k;

			for(int i = 0; i<k; i++)
			{
				lift el(i+1,n);
				elevators.push_back(el);
			}

			for(int i = 0; i<n; i++)
			{
				press_up.push_back(false);
				press_down.push_back(false);
			}
		}

		string parse_and_take_action(string act);
		vector<int> get_cost_vector()
		{
			return this -> cost;
		}

		void get_state_details();

		// ~simulator();
	
};

#endif