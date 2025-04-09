#ifndef _ABOUT_H
#define _ABOUT_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "image.h"

extern SDL_Texture *aboutButtonNormalTexture;
extern SDL_Texture *aboutButtonSelectedTexture;

extern SDL_Texture *returnButtonTexture;

void draw_about_screen();

#endif