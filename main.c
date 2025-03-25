#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // Bibliothèque pour afficher du texte
#include <stdbool.h>
#include <stdio.h>

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
int WIDTH = 640;
int HEIGHT = 480;

// Variables pour l'affichage du score
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255, 255}; // Blanc
SDL_Texture* scoreTexture = NULL;
SDL_Rect scoreRect;
int score = 0;

// Initialisation de SDL et SDL_ttf
void init_SDL() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init(); // Initialisation de SDL_ttf

    window = SDL_CreateWindow("Snake SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police (modifie "Arial.ttf" selon la police disponible)
    font = TTF_OpenFont("Arial.ttf", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        running = false;
    }
}

// Met à jour la texture du score
void update_score_texture() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture); // Supprimer l'ancienne texture

    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    scoreRect.x = WIDTH - textSurface->w - 10; // En bas à droite
    scoreRect.y = HEIGHT - textSurface->h - 10;
    scoreRect.w = textSurface->w;
    scoreRect.h = textSurface->h;

    SDL_FreeSurface(textSurface);
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

    // Afficher le score
    if (scoreTexture) SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_RenderPresent(renderer);
}

// Gère les entrées clavier et événements
void handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP: if (direction != 'u') direction = 'u'; break;
                case SDLK_DOWN: if (direction != 'd') direction = 'd'; break;
                case SDLK_LEFT: if (direction != 'l') direction = 'l'; break;
                case SDLK_RIGHT: if (direction != 'r') direction = 'r'; break;
            }
        } else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                WIDTH = event.window.data1;
                HEIGHT = event.window.data2;
                update_score_texture(); // Recalculer l'affichage du score
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

    score += 10;
    fruit.x = rand() % (WIDTH / TILE_SIZE);
    fruit.y = rand() % (HEIGHT / TILE_SIZE);

    update_score_texture();
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

    update_score_texture(); // Initialiser le score affiché

    while (running) {
        handle_input();
        update_game();
        draw_game();
        SDL_Delay(100);  // Temps entre les mises à jour (100ms)
    }

    // Nettoyage SDL
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
