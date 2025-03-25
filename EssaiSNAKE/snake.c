#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 640
#define HEIGHT 480
#define TILE_SIZE 20
#define SNAKE_MAX_LENGTH 100

typedef struct {
    int x, y;
} Point;

Point snake[SNAKE_MAX_LENGTH];
int snake_length = 3;
Point fruit;
char direction = 'd';  // Départ vers la droite
bool running = true;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Initialise SDL2
void init_SDL() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

// Dessine le jeu
void draw_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
    SDL_RenderClear(renderer);

    // Dessiner le fruit (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect fruitRect = {fruit.x * TILE_SIZE, fruit.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &fruitRect);

    // Dessiner le serpent (vert)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snake_length; i++) {
        SDL_Rect snakeRect = {snake[i].x * TILE_SIZE, snake[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    SDL_RenderPresent(renderer);
}

// Gère les entrées clavier
void handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_z: if (direction != 's') direction = 'z'; break;
                case SDLK_s: if (direction != 'z') direction = 's'; break;
                case SDLK_q: if (direction != 'd') direction = 'q'; break;
                case SDLK_d: if (direction != 'q') direction = 'd'; break;
            }
        }
    }
}

// Met à jour le jeu
void update_game() {
    // Déplacer le corps du serpent
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Déplacer la tête
    if (direction == 'z') snake[0].y--;
    if (direction == 's') snake[0].y++;
    if (direction == 'q') snake[0].x--;
    if (direction == 'd') snake[0].x++;

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
        fruit.x = rand() % (WIDTH / TILE_SIZE);
        fruit.y = rand() % (HEIGHT / TILE_SIZE);
    }
}

int main() {
    init_SDL();

    // Position initiale du serpent
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = (WIDTH / TILE_SIZE) / 2 - i;
        snake[i].y = (HEIGHT / TILE_SIZE) / 2;
    }

    // Position du fruit
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);

    while (running) {
        handle_input();
        update_game();
        draw_game();
        SDL_Delay(100);  // Temps entre les mises à jour (100ms)
    }

    // Nettoyage SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
