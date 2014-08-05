#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

/* this one should be appropriate to avoid errors on multiprocessors systems */
#define CLOCKTYPE CLOCK_MONOTONIC

#include <ncurses.h>

#include "board.h"
#include "ai.h"

int input_next_move(struct board *b) {
    return getch();
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    struct board b;
    init_board(&b);
    int (*get_next_move)(struct board*);

    if (argc > 1 && strcmp(argv[1], "ai") == 0) {
        int ai_id = 0;
        if (argc > 2) {
            ai_id = atoi(argv[2]);
        }
        if (ai_id > N_AIS) {
            ai_id = 0;
        }
        get_next_move = compute_next_move[ai_id];
    } else {
        get_next_move = &input_next_move;
    }

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    int new_score = 0;
    int c = 0;
    do {
        switch (c) {
            case KEY_UP: new_score = move_up(&b); break;
            case KEY_LEFT: new_score = move_left(&b); break;
            case KEY_DOWN: new_score = move_down(&b); break;
            case KEY_RIGHT: new_score = move_right(&b); break;
            default:;
        }
        if (new_score >= 0) {
            clear();
            print_board(&b);
            refresh();
        }
    } while ((c = get_next_move(&b)) != 'q');

    printw("GAME OVER!");
    getch();

    endwin();
    printf("\nEnd score: %d\n\n", b.score);
}
