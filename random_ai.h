
int compute_next_move(struct board *b);

void random_moves_till_death(struct board *b)
{
    int new_score;
    do {
        unsigned int move = rand_interval(0, 4);
        switch (move) {
            case 0: new_score = move_up(b); if (new_score != -1) break;
            case 1: new_score = move_down(b); if (new_score != -1) break;
            case 2: new_score = move_left(b); if (new_score != -1) break;
            case 3: new_score = move_right(b); break;
            default: new_score = -1;
        }
    } while (new_score >= 0);

    return new_score == -1 ? -1 : b->score - old_board_score;
}

int compute_next_move(struct board *b)
{
    unsigned int tries_per_dir = 1;
    struct board sandbox;
    uint64_t max_points = 0, cur_points, gained_points;
    int best_move = 'q';

    /* struct timespec tsi, tsf; */
    /* clock_gettime(CLOCKTYPE, &tsi); */

    int (*moves[4])(struct board*) = {move_down, move_up, move_left, move_right};

    for (int move = 0; move < 4; ++move) {
        cur_points = 0;
        for (int t = 0; t < tries_per_dir; ++t) {
            copy_board(b, &sandbox);
            moves[move](&sandbox);
            gained_points = random_moves_till_death(&sandbox);
        }
        if (cur_points > max_points) {
            max_points = cur_points;
            best_move = move + 258;  /* KEY_DOWN is 258 */
        }
    }

    /* clock_gettime(CLOCKTYPE, &tsf); */
    /* double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec); */
    /* long elaps_ns = tsf.tv_nsec - tsi.tv_nsec; */
    /* elaps_s += ((double)elaps_ns) / 1.0e9; */

    /* printw("%f games per second\n", ((double)tries_per_dir*4) / elaps_s); */
    /* printw("best move = %d\n", best_move); */
    /* printw("tries = %d\n", tries_per_dir); */
    /* getch(); */

    return best_move;
}
