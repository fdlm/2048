#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#define BOARD_SIZE 4

struct board {
    uint8_t field[BOARD_SIZE][BOARD_SIZE];
    uint8_t n_zeros;
    unsigned int score;
};

//shamelessly taken from stackoverflow
inline unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

inline void add_new_element(struct board *b)
{
    uint8_t new_val = (rand_interval(0, 10) == 0) ? 2 : 1;
    uint8_t elem_pos = rand_interval(0, b->n_zeros);
    --b->n_zeros;

    for(uint8_t i=0; i < BOARD_SIZE; ++i) {
        for(uint8_t j=0; j < BOARD_SIZE; ++j) {
            if (b->field[i][j] == 0) {
                if (elem_pos == 0) {
                    b->field[i][j] = new_val;
                    return;
                }
                --elem_pos;
            }
        }
    }
}

inline void copy_board(struct board *from, struct board *to) {
    to->n_zeros = from->n_zeros;
    to->score = from->score;
    for(uint8_t i=0; i < BOARD_SIZE; ++i) {
        for(uint8_t j=0; j < BOARD_SIZE; ++j) {
            to->field[i][j] = from->field[i][j];
        }
    }
}

inline void init_board(struct board *b)
{
    b->n_zeros = BOARD_SIZE * BOARD_SIZE;
    b->score = 0;
    for(uint8_t i=0; i < BOARD_SIZE; ++i) {
        for(uint8_t j=0; j < BOARD_SIZE; ++j) {
            b->field[i][j] = 0;
        }
    }
    add_new_element(b);
}

inline void print_board(struct board *b)
{
    printw("%d points\n\n", b->score);
    for(uint8_t i=0; i < BOARD_SIZE; ++i) {
        for(uint8_t j=0; j < BOARD_SIZE; ++j) {
            if (b->field[i][j] == 0) {
                printw(".\t");
            } else {
                printw("%d\t", (unsigned int) pow(2, b->field[i][j]));
            }
        }
        printw("\n");
    }
    printw("\n");
}

#define move_to_inner(x) --x
#define move_to_outer(x) ++x
#define start_at_inner(cplace, cpos) int8_t cplace = 0; \
                                     int8_t cpos = cplace + 1

#define start_at_outer(cplace, cpos) int8_t cplace = BOARD_SIZE - 1; \
                                     int8_t cpos = cplace - 1

#define not_reached_inner(x) x >= 0
#define not_reached_outer(x) x < BOARD_SIZE

#define horizontal_pos(board, i, j) board->field[i][j]
#define vertical_pos(board, i, j) board->field[j][i]

#define move_func(dir, start, move, not_reached_final, pos) \
inline int move_##dir(struct board *b) \
{ \
    int score = 0; \
    bool moved = false; \
    for (uint8_t i=0; i < BOARD_SIZE; ++i) { \
        start(cur_place, cur_pos); \
        uint8_t place_val = pos(b, i, cur_place); \
 \
        while (not_reached_final(cur_pos)) { \
            uint8_t pos_val = pos(b, i, cur_pos); \
 \
            if (pos_val == 0) { \
                move(cur_pos); \
                continue; \
            } \
 \
            if (pos_val == place_val) { \
                moved = true; \
                score += (int) pow(2, ++pos(b, i, cur_place)); \
                ++b->n_zeros; \
                move(cur_place); \
                pos(b, i, cur_pos) = 0; \
                place_val = pos(b, i, cur_place); \
                continue; \
            } \
 \
            if (place_val != 0) { \
                move(cur_place); \
            } \
 \
            if (cur_place != cur_pos) { \
                moved = true; \
                pos(b, i, cur_place) = pos(b, i, cur_pos); \
                pos(b, i, cur_pos) = 0; \
            } \
 \
            place_val = pos(b, i, cur_place); \
            move(cur_pos); \
        } \
    } \
    b->score += score; \
    if (moved) { \
        add_new_element(b); \
    } \
    return moved ? score : -1; \
}

move_func(right, start_at_outer, move_to_inner, not_reached_inner, horizontal_pos)
move_func(left, start_at_inner, move_to_outer, not_reached_outer, horizontal_pos)
move_func(up, start_at_inner, move_to_outer, not_reached_outer, vertical_pos)
move_func(down, start_at_outer, move_to_inner, not_reached_inner, vertical_pos)

#endif //BOARD_H
