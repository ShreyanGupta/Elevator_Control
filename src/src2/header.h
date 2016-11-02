#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <random>
#include <queue>
#include <unordered_set>
#include <string>
#include <assert.h>

using namespace std;

extern int N, K;
extern float p, q, r, t;

enum Lift_State{
	Up, Down, Vella
};

bool lift_is_empty(int lift_num);
bool button_above_pos(int lift_num);
int upper_floor(int lift_num);
int lower_floor(int lift_num);

void action_lift_up(int lift_num);
void action_lift_down(int lift_num);
void action_lift_open_up(int lift_num);
void action_lift_open_down(int lift_num);
void observation_floor_up(int flr);
void observation_floor_down(int flr);
void observation_lift_press(int lift_num, int flr);

void update_state(string s);
void update_i_state(string s);
void update_lift_state(int lift_num);

string get_action();
string get_lift_action(int lift_num);


#endif






