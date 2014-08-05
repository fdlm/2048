CC=cc
CFLAGS=-std=c99 -O3 -Wall -D_POSIX_C_SOURCE=199309L
LFLAGS=-lm -lncurses -lrt
OBJ=2048.o board.o random_ai.o ai.o

2048: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(LFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	@rm $(OBJ) 2048
