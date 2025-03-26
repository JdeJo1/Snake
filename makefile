EXE=snake
LIB_FLAGS=-lSDL2 -lSDL2_ttf -lSDL2_image

all :
	gcc -o $(EXE) main.c $(LIB_FLAGS)