#include "snake.h"



Point snake[SNAKE_MAX_LENGTH]={};
int snake_length = 3;
char direction = 'd'; 

Point snake2[SNAKE_MAX_LENGTH] = {}; 
int snake_length2 = 3;
char direction2 = 's';

int score1 = 0;
int score2 = 0;  
int lives1 = 3;
int lives2 = 3;


void reset_snake(int player) {
    if (player == 1) {
        snake_length = 3;
        for (int i = 0; i < snake_length; i++) {
            snake[i].x = (WIDTH / TILE_SIZE) / 4 - i;
            snake[i].y = (HEIGHT / TILE_SIZE) / 2;
        }
        direction = 'd'; // Vers la droite
    } else if (player == 2) {
        snake_length2 = 3;
        for (int i = 0; i < snake_length2; i++) {
            snake2[i].x = (WIDTH / TILE_SIZE) * 3 / 4 + i; // À droite de l'écran
            snake2[i].y = (HEIGHT / TILE_SIZE) / 2;
        }
        direction2 = 'q'; // Vers la gauche
    }
}


#include "snake.h"
#include <stdlib.h>

void update_game() {
    // Déplacement du Snake 1
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    if (direction == 'u') snake[0].y--;
    if (direction == 'd') snake[0].y++;
    if (direction == 'l') snake[0].x--;
    if (direction == 'r') snake[0].x++;

    // Déplacement du Snake 2 (si actif)
    if (num_players == 2) {
        for (int i = snake_length2 - 1; i > 0; i--) {
            snake2[i] = snake2[i - 1];
        }

        if (direction2 == 'z') snake2[0].y--;
        if (direction2 == 's') snake2[0].y++;
        if (direction2 == 'q') snake2[0].x--;
        if (direction2 == 'd') snake2[0].x++;
    }

    // Vérification des collisions pour Snake 1
    bool snake1_dead = false;

    if (snake[0].x < 0 || snake[0].x >= WIDTH / TILE_SIZE || 
        snake[0].y < 0 || snake[0].y >= HEIGHT / TILE_SIZE) {
        snake1_dead = true;
    }

    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            snake1_dead = true;
        }
    }

    for (int i = 0; i < num_obstacles; i++) {
        if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
            snake1_dead = true;
        }
    }

    if (snake1_dead) {
        lives1--;
        if (lives1 > 0) {
            reset_snake(1);
        } else {
            running = false; // Fin du jeu si le joueur 1 n'a plus de vies
        }
    }

    // Vérification des collisions pour Snake 2 (si actif)
    if (num_players == 2) {
        bool snake2_dead = false;

        if (snake2[0].x < 0 || snake2[0].x >= WIDTH / TILE_SIZE || 
            snake2[0].y < 0 || snake2[0].y >= HEIGHT / TILE_SIZE) {
            snake2_dead = true;
        }

        for (int i = 1; i < snake_length2; i++) {
            if (snake2[0].x == snake2[i].x && snake2[0].y == snake2[i].y) {
                snake2_dead = true;
            }
        }

        for (int i = 0; i < num_obstacles; i++) {
            if (snake2[0].x == obstacles[i].x && snake2[0].y == obstacles[i].y) {
                snake2_dead = true;
            }
        }

        if (snake2_dead) {
            lives2--;
            if (lives2 > 0) {
                reset_snake(2);
            } else {
                num_players = 1; // Désactive le joueur 2
            }
        }
    }

    // Collision entre les deux serpents
    if (num_players == 2) {
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == snake2[0].x && snake[i].y == snake2[0].y) {
                lives2--;
                if (lives2 > 0) reset_snake(2);
            }
        }

        for (int i = 0; i < snake_length2; i++) {
            if (snake2[i].x == snake[0].x && snake2[i].y == snake[0].y) {
                lives1--;
                if (lives1 > 0) reset_snake(1);
            }
        }
    }

    // Manger un fruit (Snake 1)
if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
    snake[snake_length] = snake[snake_length - 1]; // Prend la position de l'ancien dernier segment
    snake_length++;
    score1++;
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);
    update_score_texture();
}

// Manger un fruit (Snake 2)
if (num_players == 2 && snake2[0].x == fruit.x && snake2[0].y == fruit.y) {
    snake2[snake_length2] = snake2[snake_length2 - 1]; // Même correction ici
    snake_length2++;
    score2++;
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);
    update_score_texture();
}


    // Ajout d'obstacles toutes les 5 secondes
    if (SDL_GetTicks() - last_obstacle_time > OBSTACLE_INTERVAL) {
        if (num_obstacles < MAX_OBSTACLES) {
            obstacles[num_obstacles].x = rand() % (WIDTH / TILE_SIZE);
            obstacles[num_obstacles].y = rand() % (HEIGHT / TILE_SIZE);
            num_obstacles++;
        }
        last_obstacle_time = SDL_GetTicks();
    }
}

