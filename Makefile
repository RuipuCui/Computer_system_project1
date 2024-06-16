SRC = main.c queue.c process.c memory.c lru.c eviction.c task4.c page.c
OBJ = $(SRC:.c=.o)

EXE=allocate
CC = cc
CFLAGS = -Wall

$(EXE): $(OBJ)
	cc -Wall -o $(EXE) $(OBJ) -lm

format:
	clang-format -style=file -i *.c

clean:
	rm -f $(OBJ) $(EXE)