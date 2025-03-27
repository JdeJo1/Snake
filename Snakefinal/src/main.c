#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "image.h"
#include "snake.h"
#include "param.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Fonction pour afficher l'écran d'accueil
void show_main_menu() {
    SDL_Event event;
    bool menu_running = true;

    // Charger l'image du logo
    load_logo_image();

    // Affichage de l'écran d'accueil
    while (menu_running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        // Afficher l'image du logo (par exemple, au centre de l'écran)
        if (logoTexture) {
            int logoWidth, logoHeight;
            SDL_QueryTexture(logoTexture, NULL, NULL, &logoWidth, &logoHeight); // Obtenir les dimensions de l'image

            SDL_Rect logoRect = {WIDTH / 2 - logoWidth / 2, HEIGHT / 4 - logoHeight / 2, logoWidth, logoHeight};
            SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
        }

        // Afficher le message "Appuyez sur ENTER pour commencer"
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Appuyez sur ENTER pour commencer", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {WIDTH / 2 - textSurface->w / 2, HEIGHT / 2 - textSurface->h / 2, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Afficher l'écran
        SDL_RenderPresent(renderer);

        // Gérer les événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                menu_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) {
                    // L'utilisateur appuie sur Enter ou Espace pour commencer
                    menu_running = false;
                }
            }
        }
    }

    // Libérer la texture du logo après utilisation
    if (logoTexture) {
        SDL_DestroyTexture(logoTexture);
    }
}



// Initialisation de SDL et SDL_ttf
void init_SDL() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init(); // Initialisation de SDL_ttf
    IMG_Init(IMG_INIT_PNG); // Initialisation de SDL_image

    window = SDL_CreateWindow("Snake SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police
    font = TTF_OpenFont("Snakefinal/cosmetiques/Arial.ttf", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        running = false;
    }
}



// Gère les entrées clavier et événements
void handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:    
                    if (direction != 'd') direction = 'u'; 
                    break;
                case SDLK_DOWN:  
                    if (direction != 'u') direction = 'd'; 
                    break;
                case SDLK_LEFT:  
                    if (direction != 'r') direction = 'l'; 
                    break;
                case SDLK_RIGHT: 
                    if (direction != 'l') direction = 'r'; 
                    break;
            }
        }
    }
}


int main() {
    init_SDL();

    // Charger les images
    load_fruit_image();
    load_obstacle_image();

    // Afficher l'écran d'accueil avant de commencer le jeu
    show_main_menu();  // L'écran d'accueil

    // Position initiale du serpent
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = (WIDTH / TILE_SIZE) / 2 - i;
        snake[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    // Position du fruit
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);

    update_score_texture(); // Initialiser le score affiché

    while (running) {
        handle_input();
        update_game();
        draw_game();
        SDL_Delay(100);  // Temps entre les mises à jour (100ms)
    }

    // Nettoyage SDL
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (fruitTexture) SDL_DestroyTexture(fruitTexture);
    if (obstacleTexture) SDL_DestroyTexture(obstacleTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
