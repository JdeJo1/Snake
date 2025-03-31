#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "game.h"
#include "image.h"
#include "snake.h"
#include "param.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture *screenshot=NULL;

void show_main_menu() {
    SDL_Event event;
    bool menu_running = true;
    int selection = 1; // 1 joueur par défaut

    load_logo_image();

    while (menu_running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Afficher le logo
        if (logoTexture) {
            int logoWidth, logoHeight;
            SDL_QueryTexture(logoTexture, NULL, NULL, &logoWidth, &logoHeight);
            SDL_Rect logoRect = {0,0, logoWidth, logoHeight};
            SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        }

        // Afficher le choix du mode de jeu
        char choix1[50] = "1 Joueur";
        char choix2[50] = "2 Joueurs";

        SDL_Color colorSelected = {255, 255, 0, 255}; // Jaune si sélectionné
        SDL_Color colorNormal = {255, 255, 255, 255}; // Blanc normal

        SDL_Surface* surface1 = TTF_RenderText_Solid(font, choix1, (selection == 1) ? colorSelected : colorNormal);
        SDL_Surface* surface2 = TTF_RenderText_Solid(font, choix2, (selection == 2) ? colorSelected : colorNormal);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

        SDL_Rect rect1 = {WIDTH / 2 - surface1->w / 2, HEIGHT / 2 - surface1->h, surface1->w, surface1->h};
        SDL_Rect rect2 = {WIDTH / 2 - surface2->w / 2, HEIGHT / 2 + surface2->h, surface2->w, surface2->h};

        SDL_SetRenderDrawColor(renderer,0,0,0,127);
        SDL_RenderFillRect(renderer,&rect1);
        SDL_RenderFillRect(renderer,&rect2);
        SDL_SetRenderDrawColor(renderer,255,255,(selection==1)?0:255,255);
        SDL_RenderDrawRect(renderer,&rect1);
        SDL_SetRenderDrawColor(renderer,255,255,(selection==2)?0:255,255);
        SDL_RenderDrawRect(renderer,&rect2);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(texture1);
        SDL_DestroyTexture(texture2);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                menu_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
                    selection = (selection == 1) ? 2 : 1;
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    num_players = selection;
                    menu_running = false;
                }
            }
        }
    }



    // Libérer la texture du logo après utilisation
    if (logoTexture)
    {
        SDL_DestroyTexture(logoTexture);
    }
}

// Initialisation de SDL et SDL_ttf
void init_SDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();             // Initialisation de SDL_ttf
    IMG_Init(IMG_INIT_PNG); // Initialisation de SDL_image

    window = SDL_CreateWindow("Snake SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    // Charger la police
    font = TTF_OpenFont("Snakefinal/cosmetiques/Arial.ttf", 24);
    if (!font)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        running = false;
    }
}

int main()
{
    init_SDL();
    load_fruit_image();
    load_obstacle_image();
    show_main_menu();

    srand(time(NULL));

    // Initialisation des positions des serpents
    for (int i = 0; i < snakes[0].length; i++)
    {
        snakes[0].points[i].x = (WIDTH / TILE_SIZE) / 2 - 10;
        snakes[0].points[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    for (int i = 0; i < snakes[1].length; i++)
    {
        snakes[1].points[i].x = (WIDTH / TILE_SIZE) / 2 + 10;
        snakes[1].points[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);
    reset_completely_both_snakes();
    while (running)
    {
        play_game();
        show_end_screen();
    }
    
    
    

    // Nettoyage SDL
    if (scoreTexture)
        SDL_DestroyTexture(scoreTexture);
    if (fruitTexture)
        SDL_DestroyTexture(fruitTexture);
    if (obstacleTexture)
        SDL_DestroyTexture(obstacleTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
