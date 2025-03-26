CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image

SRC=src/main.c
OBJ=$(SRC:.c=.o)
EXEC=snake

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
