#ifndef _IMAGE_H
#define _IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "world.h"

typedef struct rectBlock_s{
    SDL_Rect *rect;
    SDL_Color fillColor, drawColor;
}rectBlock_t;

extern SDL_Texture* fruitTexture;
extern SDL_Texture* obstacleTexture;

// Variable pour l'image du logo
extern SDL_Texture* logoTexture;
extern SDL_Texture* scoreTexture;

extern SDL_Texture* aboutTexture;

extern SDL_Texture* bodyTexture;

extern SDL_Texture* controllerTexture;

// Variables pour l'affichage du score
extern TTF_Font* font;
extern TTF_Font* emojiFont;
extern SDL_Color textColor; // Blanc

void load_fruit_image();
void load_controller_image();
void load_obstacle_image();
void load_logo_image();
void load_body_image();
void load_about_image();

void update_score_texture();
SDL_Texture *get_screenshot_texture();

void rectblock_draw(rectBlock_t *rb);
void rectblock_draw_with_text(rectBlock_t *rb,char *txt);
void renderer_print_text(SDL_Rect *r,SDL_Color c, char *txt);

void rect_resize(SDL_Rect *r, int w, int h);
#endif