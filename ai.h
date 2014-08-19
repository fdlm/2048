#ifndef AI_H
#define AI_H

#include "board.h"

#define N_AIS 2

typedef int (*move_fun_ptr)(struct board *);
extern move_fun_ptr compute_next_move[N_AIS];

#endif //AI_H
