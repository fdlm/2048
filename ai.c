#include "ai.h"
#include "board.h"

int random_sampling_next_move(struct board *b);
int expect_dfs_next_move(struct board *b);

move_fun_ptr compute_next_move[N_AIS] = { random_sampling_next_move,
										  expect_dfs_next_move };
