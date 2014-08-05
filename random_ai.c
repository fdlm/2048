#include "board.h"


void random_moves_till_death(struct board *b)
{
    int new_score;
    unsigned int move;
    bool move_ok[4] = {true, true, true, true};

    while (true) {
        do {
            move = rand_interval(0, 4);
        } while (!move_ok[move]);

        switch (move) {
            case 0: new_score = move_up(b); break;
            case 1: new_score = move_down(b); break;
            case 2: new_score = move_left(b); break;
            case 3: new_score = move_right(b); break;
        }

        if (new_score == -1) {
            move_ok[move] = false;
            if (!move_ok[0] && !move_ok[1] && !move_ok[2] && !move_ok[3]) {
                // no more moves possible, game over
                return;
            }
        } else {
           move_ok[0] = true; move_ok[1] = true;
           move_ok[2] = true; move_ok[3] = true;
        }
    };
}

int random_sampling_next_move(struct board *b)
{
    unsigned int tries_per_dir = 400;
    struct board sandbox;
    uint64_t max_points = 0, cur_points;
    unsigned int prev_score;
    int best_move = 'q';

    int (*moves[4])(struct board*) = {move_down, move_up, move_left, move_right};

    for (int move = 0; move < 4; ++move) {
        cur_points = 0;
        for (int t = 0; t < tries_per_dir; ++t) {
            copy_board(b, &sandbox);
            prev_score = sandbox.score;
            moves[move](&sandbox);
            random_moves_till_death(&sandbox);
            cur_points += sandbox.score - prev_score;
        }
        if (cur_points > max_points) {
            max_points = cur_points;
            best_move = move + 258;  /* KEY_DOWN is 258 */
        }
    }

    return best_move;
}
