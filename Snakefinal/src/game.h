#ifndef _GAME_H
#define _GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "world.h"
#include "snake.h"

void play_game();
void show_end_screen();

void replay();
void quit();

void handle_input();

#endif