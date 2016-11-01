#ifndef STATE_H
#define STATE_H

// 32 bits
//    G     F     E     D     C    B   A
// ------|-----|-----|-----|-----|---|---
// 31  26 25 21 20 16 15 11 10  6 5 3 2 0

// A (3) -> Lift 1 pos
// B (3) -> Lift 2 pos
// C (5) -> Buttons pressed on Lift 1
// D (5) -> Buttons pressed on Lift 2
// E (5) -> Up button pressed on Floors
// F (5) -> Down button pressed on Floors

#include "header.h"

void action_lift_up(int lift_no, state &s);
void action_lift_down(int lift_no, state &s);
void action_lift_open_up(int lift_no, state &s);
void action_lift_open_down(int lift_no, state &s);
void observation_floor_up(int flr, state &s);
void observation_floor_down(int flr, state &s);
void observation_lift_press(int lift_no, int flr, state &s);

int pos_lift1(state s);
int floor_lift1(state s);
int pos_lift2(state s);
int floor_lift2(state s);
int button_lift1(state s);
int button_lift2(state s);
int floor_up(state s);
int floor_down(state s);
int floor_up_down(state s);

void print(state s);
void update_state(string s, state &st);
void parse_response(string s, state &st);
string action_to_string(pair<int, int> &act);
state_vars &map(state &s);

// ******************** ACTIONS ON STATES

inline void action_lift_up(int lift_no, state &s){
	if(lift_no == 1) s += 1;
	else s += 8;
}

inline void action_lift_down(int lift_no, state &s){
	if(lift_no == 1) s -= 1;
	else s -= 8;
}

inline void action_lift_open_up(int lift_no, state &s){
	if(lift_no == 1){
		s &= ~(floor_lift1(s) << 16);
		s &= ~(floor_lift1(s) << 6);
	}else{
		s &= ~(floor_lift2(s) << 16);
		s &= ~(floor_lift2(s) << 11);
	}
}

inline void action_lift_open_down(int lift_no, state &s){
	if(lift_no == 1){
		s &= ~(floor_lift1(s) << 21);
		s &= ~(floor_lift1(s) << 6);
	}else{
		s &= ~(floor_lift2(s) << 21);
		s &= ~(floor_lift2(s) << 11);
	}
}

inline void observation_floor_up(int flr, state &s){
	s |= (1 << (16 + (1 << flr)));
}

inline void observation_floor_down(int flr, state &s){
	s |= (1 << (21 + (1 << flr)));
}

inline void observation_lift_press(int lift_no, int flr, state &s){
	if(lift_no == 1) s |= (1 << (6 + (1 << flr)));
	else s |= (1 << (11 + (1 << flr)));
}

// ********************* ACCESSING DATA

inline int pos_lift1(state s){
	return (s & ((1 << 3) - 1));
}

inline int floor_lift1(state s){
	return (1 << pos_lift1(s));
}

inline int pos_lift2(state s){
	return ((s >> 3) & ((1 << 3) - 1));
}

inline int floor_lift2(state s){
	return (1 << pos_lift2(s));
}

inline int button_lift1(state s){
	return ((s >> 6) & ((1 << 5) - 1));
}

inline int button_lift2(state s){
	return ((s >> 11) & ((1 << 5) - 1));
}

inline int floor_up(state s){
	return ((s >> 16) & ((1 << 5) - 1));
}

inline int floor_down(state s){
	return ((s >> 21) & ((1 << 5) - 1));
}

inline int floor_up_down(state s){
	return (floor_up(s) | floor_down(s));
}

#endif