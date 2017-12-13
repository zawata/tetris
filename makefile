CC = gcc
OBJ = term.o tetris.o
INC_FILES = -I./
CFLAGS = -Wall $(INC_FILES)

.PHONY: tetris

new: clean tetris

debug: CFLAGS += -g
debug: clean
debug: tetris

tetris: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@rm -f *.o
	@rm -f tetris
