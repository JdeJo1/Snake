#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // Bibliothèque pour afficher du texte
#include <SDL2/SDL_image.h> // Bibliothèque pour charger des images
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 20
#define SNAKE_MAX_LENGTH 100
#define MAX_OBSTACLES 10 // Nombre maximum d'obstacles à l'écran
#define OBSTACLE_INTERVAL 5000 // Temps en millisecondes entre l'ajout des obstacles (5 secondes)

// Structure pour les points
typedef struct {
    int x, y;
} Point;

// Variables du serpent
Point snake[SNAKE_MAX_LENGTH];
int snake_length = 3;
Point fruit;
char direction = 'd';  // Départ vers la droite
bool running = true;

// Variables de la fenêtre SDL
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

// Variables pour les obstacles
Point obstacles[MAX_OBSTACLES];
int num_obstacles = 0;
Uint32 last_obstacle_time = 0;

// Variables pour les images
SDL_Texture* fruitTexture = NULL;
SDL_Texture* obstacleTexture = NULL;

// Fonction pour initialiser SDL
void init_SDL() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init(); // Initialisation de SDL_ttf
    IMG_Init(IMG_INIT_PNG);  // Initialisation de SDL_image pour charger des PNG

    window = SDL_CreateWindow("Snake SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger la police
    font = TTF_OpenFont("Arial.ttf", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        running = false;
    }
}

// Fonction pour charger l'image du fruit
void load_fruit_image() {
    fruitTexture = IMG_LoadTexture(renderer, "fruit.png");  // Remplacer "fruit.png" par le chemin vers votre image
    if (!fruitTexture) {
        printf("Erreur de chargement de l'image du fruit : %s\n", SDL_GetError());
        running = false;
    }
}

// Fonction pour charger l'image de l'obstacle
void load_obstacle_image() {
    obstacleTexture = IMG_LoadTexture(renderer, "obstacle.png");  // Remplacer "obstacle.png" par le chemin vers votre image
    if (!obstacleTexture) {
        printf("Erreur de chargement de l'image de l'obstacle : %s\n", SDL_GetError());
        running = false;
    }
}

// Fonction pour initialiser les obstacles
void init_obstacles() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = rand() % (WIDTH / TILE_SIZE);  // Coordonnée X aléatoire
        obstacles[i].y = rand() % (HEIGHT / TILE_SIZE); // Coordonnée Y aléatoire
    }
}

// Fonction pour mettre à jour la texture du score
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

// Fonction pour dessiner le jeu
void draw_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
    SDL_RenderClear(renderer);

    // Dessiner le fruit (avec l'image)
    if (fruitTexture) {
        SDL_Rect fruitRect = {fruit.x * TILE_SIZE, fruit.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);
    }

    // Dessiner le serpent (vert)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snake_length; i++) {
        SDL_Rect snakeRect = {snake[i].x * TILE_SIZE, snake[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    // Dessiner les obstacles (avec l'image)
    if (obstacleTexture) {
        for (int i = 0; i < num_obstacles; i++) {
            SDL_Rect obstacleRect = {obstacles[i].x * TILE_SIZE, obstacles[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(renderer, obstacleTexture, NULL, &obstacleRect);
        }
    }

    // Afficher le score
    if (scoreTexture) SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_RenderPresent(renderer);
}

// Fonction pour gérer les entrées clavier et événements
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

// Fonction pour mettre à jour le jeu
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

    // Vérifier collision avec les obstacles
    for (int i = 0; i < num_obstacles; i++) {
        if (snake[0].x == obstacles[i].x && snake[0].y == obstacles[i].y) {
            running = false; // Game over si collision avec obstacle
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

int main() {
    init_SDL();

    // Charger les images
    load_fruit_image();
    load_obstacle_image();

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
