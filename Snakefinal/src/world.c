#include "world.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

int WIDTH = 920;
int HEIGHT = 920;
int HEADER_HEIGHT=40;

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

void draw_snake_rect(snake_t *s){
    if(s->lives>0){
        for (int i = 0; i < s->length; i++) {
            if(s->body_texture){
                SDL_Rect snakeRect = {s->points[i].x * TILE_SIZE-10, HEADER_HEIGHT+s->points[i].y * TILE_SIZE-10, TILE_SIZE*2, TILE_SIZE*2};
                SDL_RenderCopy(renderer,s->body_texture,NULL,&snakeRect);
            }
            else{
                SDL_Rect snakeRect = {s->points[i].x * TILE_SIZE, HEADER_HEIGHT+s->points[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(renderer, &snakeRect);
            }
            
        }
    }
}

// Dessine le jeu
void draw_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
    SDL_RenderClear(renderer);

    SDL_Rect header_rect=(SDL_Rect){0,0,WIDTH,HEADER_HEIGHT};
    SDL_SetRenderDrawColor(renderer,127,127,127,255);
    SDL_RenderFillRect(renderer,&header_rect);

    // Dessiner le fruit
    if (fruitTexture) {
        SDL_Rect fruitRect = {fruit.x * TILE_SIZE, HEADER_HEIGHT+fruit.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);
    }

    // Dessiner les obstacles
    if (obstacleTexture) {
        for (int i = 0; i < num_obstacles; i++) {
            SDL_Rect obstacleRect = {obstacles[i].x * TILE_SIZE, HEADER_HEIGHT+obstacles[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_RenderCopy(renderer, obstacleTexture, NULL, &obstacleRect);
        }
    }

    // Dessiner le Snake 1 (vert)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    if(snakes[0].lives>0){
        draw_snake_rect(&snakes[0]);
    }
    

    // Dessiner le Snake 2 (rouge) seulement si on joue à 2 joueurs
    if (sel_num_players == 2) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        draw_snake_rect(&snakes[1]);
    }

    // Afficher le score
    if (playing&&scoreTexture){
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        
    }

    SDL_RenderPresent(renderer);
}

void fruit_renew_coord(){
    bool conflict=false;
    
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);
    for(int i=0; i<num_obstacles; i++){
        if(points_have_same_coord(&fruit,&obstacles[i])){
            conflict=true;
            break;
        }
    }
    if(conflict){
        fruit_renew_coord();
    }
}

void world_add_new_obstacle(){
    bool conflict=false;
    obstacles[num_obstacles].x = rand() % (WIDTH / TILE_SIZE);
    obstacles[num_obstacles].y = rand() % (HEIGHT / TILE_SIZE);
    if(points_have_same_coord(&fruit,&obstacles[num_obstacles])){
        conflict=true;
    }
    else{
        Point spawn_points[]={(Point){(WIDTH / TILE_SIZE) / 4,(HEIGHT / TILE_SIZE) / 2},(Point){(WIDTH / TILE_SIZE) * 3 / 4,(HEIGHT / TILE_SIZE) / 2}};
        Point test_point;
        
        for(int i=0; i<2; i++){
            for(int j=-2; j<=2; j++){
                test_point=(Point){spawn_points[i].x+j,spawn_points[0].y};
                if(points_have_same_coord(&obstacles[num_obstacles],&test_point)){
                    conflict=true;
                    break;
                }
                test_point=(Point){spawn_points[i].x,spawn_points[0].y+j};
                if(points_have_same_coord(&obstacles[num_obstacles],&test_point)){
                    conflict=true;
                    break;
                }
            }
        }
    }
    
    if(conflict){
        world_add_new_obstacle();
    }
    else{
        num_obstacles++;
    }
    
    
}