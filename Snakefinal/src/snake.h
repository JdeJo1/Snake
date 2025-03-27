#ifndef _SNAKE_H
#define _SNAKE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "param.h"
#include "world.h"
#include "point.h"

extern Point snake[];
extern int snake_length;
extern char direction;

extern Point snake2[];
extern int snake_length2;
extern char direction2;

extern int score1, score2;
extern int lives1, lives2;


void update_game();

#endif