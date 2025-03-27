#ifndef _WORLD_H
#define _WORLD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "point.h"
#include "param.h"
#include "snake.h"
#include "image.h"

extern int WIDTH;
extern int HEIGHT;

extern int num_obstacles;
extern bool running;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_Rect scoreRect;

extern Uint32 last_obstacle_time;

extern Point fruit;
extern Point obstacles[];

extern int num_players;


void draw_game();

#endif