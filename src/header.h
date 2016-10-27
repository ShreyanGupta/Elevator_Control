#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef int state;

extern int top_floor;
extern const float INVALID;

state lift_up(int lift_no, state s);
state lift_down(int lift_no, state s);
state floor_up(int flr, state s);
state floor_down(int flr, state s);
state lift_press(int lift_no, int flr);

#endif