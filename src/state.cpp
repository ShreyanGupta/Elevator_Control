#include "header.h"

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

// ******************** ACTIONS ON STATES

inline state lift_up(int lift_no, state s){
	if(lift_no == 1) return s+1;
	else return s+8;
}

inline state lift_down(int lift_no, state s){
	if(lift_no == 1) return s-1;
	else return s-8;
}

inline state floor_up(int flr, state s){
	return s | (1 << (16 + (1 << flr)));
}

inline state floor_down(int flr, state s){
	return s | (1 << (21 + (1 << flr)));
}

inline state lift_press(int lift_no, int flr){
	if(lift_no == 1) return s | (1 << (6 + (1 << flr)));
	else return s | (1 << (11 + (1 << flr)));
}

// ********************* ACCESSING DATA

inline int pos_lift1(state s){
	return (s & (1 << 3 - 1));
}

inline int floor_lift1(state s){
	return (1 << pos_lift1(s));
}

inline int pos_lift2(state s){
	return ((s >> 3) & (1 << 3 - 1));
}

inline int floor_lift2(state s){
	return (1 << pos_lift2(s));
}

inline int button_lift1(state s){
	return ((s >> 6) & (1 << 5 - 1));
}

inline int button_lift2(state s){
	return ((s >> 11) & (1 << 5 - 1));
}

inline int floor_up(state s){
	return ((s >> 16) & (1 << 5 - 1));
}

inline int floor_down(state s){
	return ((s >> 21) & (1 << 5 - 1));
}

inline int floor_up_down(state s){
	return (floor_up(s) | floor_down(s));
}


