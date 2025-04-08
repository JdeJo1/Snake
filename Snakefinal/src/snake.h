#ifndef _SNAKE_H
#define _SNAKE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "param.h"
#include "world.h"
#include "point.h"

typedef struct snake_s
{
    char direction;
    Point points[SNAKE_MAX_LENGTH];
    int length, lives, score;
    SDL_Texture *body_texture;
}snake_t;

extern snake_t snakes[];

void reset_snake(int player);
void reset_completely_both_snakes();

bool snake_collides_wall(snake_t *s);
bool snake_collides_obstacle(snake_t *s);
bool snake_autocollides(snake_t *s);
void snake_refresh_eating(snake_t *s);

void update_game();

#endif
