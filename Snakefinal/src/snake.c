#include "snake.h"

int snake_length = 3;

Point snake[SNAKE_MAX_LENGTH]={};
char direction = 'd';  // Départ vers la droite

int score = 0;
// Met à jour le jeu
void update_game() {
    // Déplacer le corps du serpent
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Déplacer la tête
    if (direction == 'u') snake[0].y--;
    if (direction == 'd') snake[0].y++;
    if (direction == 'l') snake[0].x--;
    if (direction == 'r') snake[0].x++;

    // Vérifier collisions avec les murs
    if (snake[0].x < 0 || snake[0].x >= WIDTH / TILE_SIZE ||
        snake[0].y < 0 || snake[0].y >= HEIGHT / TILE_SIZE) {
        running = false; // Game over
    }

    // Vérifier collision avec soi-même
    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            running = false;
        }
    }

    // Manger le fruit
    if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
        snake_length++;
        snake[snake_length - 1] = snake[snake_length - 2];  // Correction ici

        score += 1;
        fruit.x = rand() % (WIDTH / TILE_SIZE);
        fruit.y = rand() % (HEIGHT / TILE_SIZE);

        update_score_texture();
    }

    // Ajouter des obstacles toutes les 5 secondes
    if (SDL_GetTicks() - last_obstacle_time > OBSTACLE_INTERVAL) {
        if (num_obstacles < MAX_OBSTACLES) {
            obstacles[num_obstacles].x = rand() % (WIDTH / TILE_SIZE);
            obstacles[num_obstacles].y = rand() % (HEIGHT / TILE_SIZE);
            num_obstacles++;
        }
        last_obstacle_time = SDL_GetTicks();
    }
}