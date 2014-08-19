#include "board.h"
#define NO_MOVE_POSSIBLE 4

double expect_dfs(struct board *b, const unsigned int max_depth, uint8_t *best_move)
{
	if (max_depth == 0) {
		return b->score;
	}

	struct board next;
	struct board next_with_add;
	bool moved = false;
	double max_score = 0;
	double cur_score;

	for (uint8_t m = 0; m < 4; ++m) {
		copy_board(b, &next);
		int res;
		switch (m) {
			case 0: res = just_move_down(&next); break;
			case 1: res = just_move_up(&next); break;
			case 2: res = just_move_left(&next); break;
			case 3: res = just_move_right(&next);
		}
		if (res != -1) {
			moved = true;
			cur_score = 0;
			const uint8_t n_nz = next.n_zeros;
			const double two_fact = 1. / (double)n_nz * 0.9;
			const double four_fact = 1. / (double)n_nz * 0.1;

			for (uint8_t i = 0; i < n_nz; ++i) {
				copy_board(&next, &next_with_add);
				place_element(&next_with_add, i, 2);
				cur_score += two_fact * expect_dfs(&next_with_add, max_depth - 1, NULL);

				copy_board(&next, &next_with_add);
				place_element(&next_with_add, i, 4);
				cur_score += four_fact * expect_dfs(&next_with_add, max_depth - 1, NULL);
			}

			if (cur_score >= max_score) {
				if (best_move != NULL) {
					*best_move = m;
				}
				max_score = cur_score;
			}
		}
	}

	/* no move was possible -> game over! */
	if (!moved) {
		if (best_move != NULL) {
			*best_move = NO_MOVE_POSSIBLE;
		}
		return b->score;
	}

	return max_score;
}

int expect_dfs_next_move(struct board *b)
{
	const unsigned int max_depth = 4;
	uint8_t move;

	expect_dfs(b, max_depth, &move);
	return move == NO_MOVE_POSSIBLE ? 'q' : move + 258;
}