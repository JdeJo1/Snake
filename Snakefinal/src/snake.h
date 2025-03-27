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
extern char direction;  // Départ vers la droite
extern int score;

void update_game();

#endif