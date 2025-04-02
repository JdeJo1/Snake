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

extern SDL_Texture *screenshot;

extern int WIDTH;
extern int HEIGHT;

extern int num_obstacles;
extern bool running;
extern bool playing;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_Rect scoreRect;

extern Uint32 last_obstacle_time;

extern Point fruit;
extern Point obstacles[];

extern int num_players;
extern int sel_num_players;

extern Uint32 last_update_time;
extern const Uint32 MOVE_DELAY; // Délai entre les déplacements en millisecondes (100ms = 0.1 seconde)

void draw_game();

#endif