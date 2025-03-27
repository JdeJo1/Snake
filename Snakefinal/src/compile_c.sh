#!/bin/bash


gcc -Wall -c main.c -lSDL2 -lSDL2_image -lSDL2_ttf
gcc -Wall -c world.c -lSDL2 -lSDL2_image -lSDL2_ttf
gcc -Wall -c snake.c -lSDL2 -lSDL2_image -lSDL2_ttf
gcc -Wall -c image.c -lSDL2 -lSDL2_image -lSDL2_ttf

gcc main.o world.o snake.o image.o -o prog -lSDL2 -lSDL2_image -lSDL2_ttf