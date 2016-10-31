#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "state_vars.h"

using namespace std;

typedef int state;

extern const float INVALID;
extern int N, K;
extern float p, q, r, t;
extern unordered_map<state, state_vars> m;

state_vars &map(state &s);

#endif