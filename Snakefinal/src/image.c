#include "image.h"

SDL_Texture* fruitTexture = NULL;
SDL_Texture* obstacleTexture = NULL;

// Variable pour l'image du logo
SDL_Texture* logoTexture = NULL;
SDL_Texture* scoreTexture = NULL;

// Variables pour l'affichage du score
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255, 255}; // Blanc


// Fonction pour charger l'image du logo
void load_logo_image() {
    logoTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/logosnake.png");
    if (!logoTexture) {
        printf("Erreur chargement image logo : %s\n", IMG_GetError());
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