EXE=main
LIB_FLAGS=-lSDL2 -lSDL2_ttf

all :
	gcc -o $(EXE) main.c $(LIB_FLAGS)

