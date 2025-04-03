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
        
        snakes[0].direction = 'd';
        
    } else if (player == 2) {
        snakes[1].length = 3;
        for (int i = 0; i < snakes[1].length; i++) {
            snakes[1].points[i].x = (WIDTH / TILE_SIZE) * 3 / 4 + i; // À droite de l'écran
            snakes[1].points[i].y = (HEIGHT / TILE_SIZE) / 2;
        }
        
        snakes[1].direction = 'u';
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
    snakes[1].direction='u';

    num_players=sel_num_players;
}

//Mettre à jour les coordonnées du snake "s"
void update_snake_coord(snake_t *s){
    for (int i = s->length - 1; i > 0; i--) {
        s->points[i] = s->points[i-1];
    }
    if (s->direction == 'u') s->points[0].y--;
    if (s->direction == 'd') s->points[0].y++;
    if (s->direction == 'l') s->points[0].x--;
    if (s->direction == 'r') s->points[0].x++;
}

//Retourne "vrai" si le snake "s" à touché un mur
bool snake_collides_wall(snake_t *s){
    bool coll=false;
    if (s->points[0].x < 0 || s->points[0].x >= WIDTH / TILE_SIZE || 
        s->points[0].y < 0 || s->points[0].y >= HEIGHT / TILE_SIZE) {
        coll=true;
        
    }
    return coll;
}

//Retourne "vrai" si le snake "s" a touché un obstacle
bool snake_collides_obstacle(snake_t *s){
    bool coll=false;
    for (int i = 0; i < num_obstacles; i++) {
        if (points_have_same_coord(&s->points[0],&obstacles[i])) {
            coll = true;
        }
    }
    return coll;
}

//Retourne "vrai" si le snake "s" a fait collision avec lui-même
bool snake_autocollides(snake_t *s){
    bool coll=false;
    for (int i = 1; i < s->length; i++) {
        if (points_have_same_coord(&s->points[0],&s->points[i])) {
            coll = true;
        }
    }
    return coll;
}

/*Si le snake "s" mange un fruit
- Incrémenter sa longueur
- Incrémenter son score
- Réaffecter de nouvelles coordonnées au fruit
*/
void snake_refresh_eating(snake_t *s){
    if (points_have_same_coord(&s->points[0],&fruit)) {
        s->points[s->length] = s->points[s->length - 1]; // Prend la position de l'ancien dernier segment
        s->length++;
        s->score++;
        fruit_renew_coord();
        update_score_texture();
    }
}

void update_game() {
    if(snakes[0].lives>0){
        update_snake_coord(&snakes[0]);    

        // Vérification des collisions pour Snake 1
        bool snake1_dead = false;

        if (snake_collides_wall(&snakes[0])) {
            snake1_dead = true;
        }

        if(snake_autocollides(&snakes[0])){
            snake1_dead = true;
        }

        if(snake_collides_obstacle(&snakes[0])){
            snake1_dead = true;
        }

        if (snake1_dead) {
            snakes[0].lives--;
            if (snakes[0].lives > 0) {
                reset_snake(1);
            } else {
                num_players=1;
            }
        }
    }

    // Vérification des collisions pour Snake 2 (si actif)
    if (sel_num_players == 2) {
        if(snakes[1].lives>0){
            // Déplacement du Snake 2 (si actif)
            update_snake_coord(&snakes[1]);

            bool snake2_dead = false;

            if (snake_collides_wall(&snakes[1])) {
                snake2_dead = true;
            }

            if(snake_autocollides(&snakes[1])){
                snake2_dead = true;
            }

            if(snake_collides_obstacle(&snakes[1])){
                snake2_dead = true;
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
        
    }

    if((snakes[0].lives>0)&&(snakes[1].lives>0)&&(sel_num_players == 2)){
        // Collision entre les deux serpents
        for (int i = 0; i < snakes[0].length; i++) {
            if (points_have_same_coord(&snakes[0].points[i],&snakes[1].points[0])) {
                snakes[1].lives--;
                if (snakes[1].lives > 0) reset_snake(2);
            }
        }

        for (int i = 0; i < snakes[1].length; i++) {
            if (points_have_same_coord(&snakes[1].points[i],&snakes[0].points[0])) {
                snakes[0].lives--;
                if (snakes[0].lives > 0) reset_snake(1);
            }
        }
    }
    

    // Manger un fruit (Snake 1)
    if (snakes[0].lives > 0) snake_refresh_eating(&snakes[0]);

    // Manger un fruit (Snake 2)
    if (sel_num_players == 2) {
        if (snakes[1].lives > 0) snake_refresh_eating(&snakes[1]);
    }

    bool end=true;
    for(int i=0; i<sel_num_players; i++){
        if(snakes[i].lives>0){
            end=false;
        }
    }
    if(end){
        playing=false;
        draw_game();
        screenshot=get_screenshot_texture();
    }

    // Ajout d'obstacles toutes les 5 secondes
    if (SDL_GetTicks() - last_obstacle_time > OBSTACLE_INTERVAL) {
        if (num_obstacles < MAX_OBSTACLES) {
            world_add_new_obstacle();
        }
        last_obstacle_time = SDL_GetTicks();
    }
}


