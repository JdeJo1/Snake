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

void update_score_texture() {
    char scoreText[100]; // pour éviter tout dépassement dz mémoire
    
    if (num_players ==2){
        sprintf(scoreText, "Joueur 1: %d pts - Vies: %d | Joueur 2: %d pts - Vies: %d",
            score1, lives1, num_players == 2 ? score2 : 0, num_players == 2 ? lives2 : 0);
        }
    else{
    sprintf(scoreText, "Joueur 1: %d pts - Vies: %d", score1, lives1);
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    if (textSurface) {
        if (scoreTexture) SDL_DestroyTexture(scoreTexture); // Supprime l'ancienne texture
        scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        scoreRect = (SDL_Rect){20, 20, textSurface->w, textSurface->h}; // Position en haut à gauche
        SDL_FreeSurface(textSurface);
    }
}
