#include "world.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

int WIDTH = 920;
int HEIGHT = 920;

int num_obstacles = 0;

bool running = true;
bool playing = true;
int num_players = 1;

SDL_Rect scoreRect;

Uint32 last_obstacle_time=0;

Point fruit;
Point obstacles[MAX_OBSTACLES];

Uint32 last_update_time = 0;
const Uint32 MOVE_DELAY = 100; // Délai entre les déplacements en millisecondes (100ms = 0.1 seconde)

// Dessine le jeu
void draw_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
    SDL_RenderClear(renderer);

    // Dessiner le fruit
    if (fruitTexture) {
        SDL_Rect fruitRect = {fruit.x * TILE_SIZE, fruit.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);
    }

    // Dessiner les obstacles
    if (obstacleTexture) {
        for (int i = 0; i < num_obstacles; i++) {
            SDL_Rect obstacleRect = {obstacles[i].x * TILE_SIZE, obstacles[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(renderer, obstacleTexture, NULL, &obstacleRect);
        }
    }

    // Dessiner le Snake 1 (vert)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snakes[0].length; i++) {
        SDL_Rect snakeRect = {snakes[0].points[i].x * TILE_SIZE, snakes[0].points[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    // Dessiner le Snake 2 (rouge) seulement si on joue à 2 joueurs
    if (num_players == 2) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0; i < snakes[1].length; i++) {
            SDL_Rect snake2Rect = {snakes[1].points[i].x * TILE_SIZE, snakes[1].points[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &snake2Rect);
        }
    }

    // Afficher le score
    if (scoreTexture) SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_RenderPresent(renderer);
}