#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

typedef int state;
struct state_vars;

extern const float INVALID;
extern int N, K;
extern float p, q, r, t;
extern unordered_map<state, state_vars> m;

#endif