#include "board.h"

extern inline void copy_board(struct board *from, struct board *to);
extern inline void init_board(struct board *b);
extern inline void print_board(struct board *b);

extern inline int move_down(struct board *b);
extern inline int move_up(struct board *b);
extern inline int move_left(struct board *b);
extern inline int move_right(struct board *b);

extern inline unsigned int rand_interval(unsigned int min, unsigned int max);
