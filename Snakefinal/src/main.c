#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "image.h"
#include "snake.h"
#include "param.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Uint32 last_update_time = 0;
const Uint32 MOVE_DELAY = 100; // Délai entre les déplacements en millisecondes (100ms = 0.1 seconde)

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
            SDL_Rect logoRect = {WIDTH / 2 - logoWidth / 2, HEIGHT / 4 - logoHeight / 2, logoWidth, logoHeight};
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

    // Charger la police
    font = TTF_OpenFont("Snakefinal/cosmetiques/Arial.ttf", 24);
    if (!font)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        running = false;
    }
}

// Gère les entrées clavier et événements
void handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            // Contrôles pour le Snake 1
            case SDLK_UP:
                if (direction != 'd')
                    direction = 'u';
                break;
            case SDLK_DOWN:
                if (direction != 'u')
                    direction = 'd';
                break;
            case SDLK_LEFT:
                if (direction != 'r')
                    direction = 'l';
                break;
            case SDLK_RIGHT:
                if (direction != 'l')
                    direction = 'r';
                break;

            // Contrôles pour le Snake 2
            case SDLK_z:
                if (direction2 != 's')
                    direction2 = 'z';
                break;
            case SDLK_s:
                if (direction2 != 'z')
                    direction2 = 's';
                break;
            case SDLK_q:
                if (direction2 != 'd')
                    direction2 = 'q';
                break;
            case SDLK_d:
                if (direction2 != 'q')
                    direction2 = 'd';
                break;
            }
        }
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
    for (int i = 0; i < snake_length; i++)
    {
        snake[i].x = (WIDTH / TILE_SIZE) / 2 - 10;
        snake[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    for (int i = 0; i < snake_length2; i++)
    {
        snake2[i].x = (WIDTH / TILE_SIZE) / 2 + 10;
        snake2[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);

    

    Uint32 start_time = SDL_GetTicks();

    while (running)
    {
        Uint32 current_time = SDL_GetTicks();
        Uint32 delta_time = current_time - start_time;

        // Gestion des événements
        handle_input();

        // Mise à jour du jeu seulement si le délai est passé
        if (current_time - last_update_time >= MOVE_DELAY)
        {
            update_game();
            last_update_time = current_time;
        }

        // Dessiner le jeu à chaque itération
        draw_game();

        // Limiter la boucle à environ 60 FPS pour pouvoir jouer en même temps
        if (delta_time < (100 / 60))
        {
            SDL_Delay((1000 / 60) - delta_time);
        }

        start_time = SDL_GetTicks(); // Réinitialiser le start_time pour maintenir un rythme constant

        update_score_texture();
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
