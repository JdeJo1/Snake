#ifndef _IMAGE_H
#define _IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "world.h"

extern SDL_Texture* fruitTexture;
extern SDL_Texture* obstacleTexture;

// Variable pour l'image du logo
extern SDL_Texture* logoTexture;
extern SDL_Texture* scoreTexture;

// Variables pour l'affichage du score
extern TTF_Font* font;
extern SDL_Color textColor; // Blanc

void load_fruit_image();
void load_obstacle_image();
void load_logo_image();
void update_score_texture();
SDL_Texture *get_screenshot_texture();

#endif