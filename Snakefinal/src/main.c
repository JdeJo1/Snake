#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 20
#define SNAKE_MAX_LENGTH 100
#define OBSTACLE_INTERVAL 5000
#define MAX_OBSTACLES 20

// Structure des points
typedef struct {
    int x, y;
} Point;

Point snake[SNAKE_MAX_LENGTH];
int snake_length = 3;
Point fruit;
Point obstacles[MAX_OBSTACLES];
int num_obstacles = 0;
char direction = 'd';  // Départ vers la droite
bool running = true;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int WIDTH = 920;
int HEIGHT = 920;

// Variables pour l'affichage du score
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255, 255}; // Blanc
SDL_Texture* scoreTexture = NULL;
SDL_Rect scoreRect;
int score = 0;

SDL_Texture* fruitTexture = NULL;
SDL_Texture* obstacleTexture = NULL;
Uint32 last_obstacle_time = 0;

// Variable pour l'image du logo
SDL_Texture* logoTexture = NULL;

// Fonction pour charger l'image du logo
void load_logo_image() {
    logoTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/logosnake.png");
    if (!logoTexture) {
        printf("Erreur chargement image logo : %s\n", IMG_GetError());
    }
}
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

// Charger l'image du fruit
void load_fruit_image() {
    fruitTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/fruit.png");
    if (!fruitTexture) {
        printf("Erreur chargement image fruit : %s\n", IMG_GetError());
    }
}

// Charger l'image de l'obstacle
void load_obstacle_image() {
    obstacleTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/obstacle.png");
    if (!obstacleTexture) {
        printf("Erreur chargement image obstacle : %s\n", IMG_GetError());
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

    // Dessiner le serpent
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
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
