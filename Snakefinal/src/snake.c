#include "snake.h"
#include <stdlib.h>

snake_t snakes[2];

void reset_snake(int player) {
    if (player == 1) {
        snakes[0].length = 3;
        for (int i = 0; i < snakes[0].length; i++) {
            snakes[0].points[i].x = (WIDTH / TILE_SIZE) / 4 - i;
            snakes[0].points[i].y = (HEIGHT / TILE_SIZE) / 2;
        }
        
        snakes[0].direction = 'd'; // Vers la droite
        
    } else if (player == 2) {
        snakes[1].length = 3;
        for (int i = 0; i < snakes[1].length; i++) {
            snakes[1].points[i].x = (WIDTH / TILE_SIZE) * 3 / 4 + i; // À droite de l'écran
            snakes[1].points[i].y = (HEIGHT / TILE_SIZE) / 2;
        }
        
        snakes[1].direction = 'q'; // Vers la gauche
    }
}

void reset_completely_both_snakes(){
    reset_snake(1);
    reset_snake(2);

    snakes[0].score=0;
    snakes[0].lives=3;
    snakes[0].direction='d';

    snakes[1].score=0;
    snakes[1].lives=3;
    snakes[1].direction='s';
}

void update_game() {
    // Déplacement du Snake 1
    for (int i = snakes[0].length - 1; i > 0; i--) {
        snakes[0].points[i] = snakes[0].points[i-1];
    }

    if (snakes[0].direction == 'u') snakes[0].points[0].y--;
    if (snakes[0].direction == 'd') snakes[0].points[0].y++;
    if (snakes[0].direction == 'l') snakes[0].points[0].x--;
    if (snakes[0].direction == 'r') snakes[0].points[0].x++;

    // Déplacement du Snake 2 (si actif)
    if (num_players == 2) {
        for (int i = snakes[1].length - 1; i > 0; i--) {
            snakes[1].points[i] = snakes[1].points[i - 1];
        }

        if (snakes[1].direction == 'z') snakes[1].points[0].y--;
        if (snakes[1].direction == 's') snakes[1].points[0].y++;
        if (snakes[1].direction == 'q') snakes[1].points[0].x--;
        if (snakes[1].direction == 'd') snakes[1].points[0].x++;
    }

    // Vérification des collisions pour Snake 1
    bool snake1_dead = false;

    if (snakes[0].points[0].x < 0 || snakes[0].points[0].x >= WIDTH / TILE_SIZE || 
        snakes[0].points[0].y < 0 || snakes[0].points[0].y >= HEIGHT / TILE_SIZE) {
        snake1_dead = true;
    }

    for (int i = 1; i < snakes[1].length; i++) {
        if (snakes[0].points[0].x == snakes[0].points[i].x && snakes[0].points[0].y == snakes[0].points[i].y) {
            snake1_dead = true;
        }
    }

    for (int i = 0; i < num_obstacles; i++) {
        if (snakes[0].points[0].x == obstacles[i].x && snakes[0].points[0].y == obstacles[i].y) {
            snake1_dead = true;
        }
    }

    if (snake1_dead) {
        snakes[0].lives--;
        if (snakes[0].lives > 0) {
            reset_snake(1);
        } else {
            playing = false; // Fin du jeu si le joueur 1 n'a plus de vies
            screenshot=get_screenshot_texture();
        }
    }

    // Vérification des collisions pour Snake 2 (si actif)
    if (num_players == 2) {
        bool snake2_dead = false;

        if (snakes[1].points[0].x < 0 || snakes[1].points[0].x >= WIDTH / TILE_SIZE || 
            snakes[1].points[0].y < 0 || snakes[1].points[0].y >= HEIGHT / TILE_SIZE) {
            snake2_dead = true;
        }

        for (int i = 1; i < snakes[1].length; i++) {
            if (snakes[1].points[0].x == snakes[1].points[i].x && snakes[1].points[0].y == snakes[1].points[i].y) {
                snake2_dead = true;
            }
        }

        for (int i = 0; i < num_obstacles; i++) {
            if (snakes[1].points[0].x == obstacles[i].x && snakes[1].points[0].y == obstacles[i].y) {
                snake2_dead = true;
            }
        }

        if (snake2_dead) {
            snakes[1].lives--;
            if (snakes[1].lives > 0) {
                reset_snake(2);
            } else {
                num_players = 1; // Désactive le joueur 2
            }
        }
    }

    // Collision entre les deux serpents
    if (num_players == 2) {
        for (int i = 0; i < snakes[0].length; i++) {
            if (snakes[0].points[i].x == snakes[1].points[0].x && snakes[0].points[i].y == snakes[1].points[0].y) {
                snakes[1].lives--;
                if (snakes[1].lives > 0) reset_snake(2);
            }
        }

        for (int i = 0; i < snakes[1].length; i++) {
            if (snakes[1].points[i].x == snakes[0].points[0].x && snakes[1].points[i].y == snakes[0].points[0].y) {
                snakes[0].lives--;
                if (snakes[0].lives > 0) reset_snake(1);
            }
        }
    }

    // Manger un fruit (Snake 1)
if (snakes[0].points[0].x == fruit.x && snakes[0].points[0].y == fruit.y) {
    snakes[0].points[snakes[0].length] = snakes[0].points[snakes[0].length - 1]; // Prend la position de l'ancien dernier segment
    snakes[0].length++;
    snakes[0].score++;
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);
    update_score_texture();
}

// Manger un fruit (Snake 2)
if (num_players == 2 && snakes[1].points[0].x == fruit.x && snakes[1].points[0].y == fruit.y) {
    snakes[1].points[snakes[1].length] = snakes[1].points[snakes[1].length - 1]; // Même correction ici
    snakes[1].length++;
    snakes[1].score++;
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

