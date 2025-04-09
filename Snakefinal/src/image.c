#include "image.h"

SDL_Texture* fruitTexture = NULL;
SDL_Texture* obstacleTexture = NULL;

SDL_Texture* controllerTexture=NULL;

// Variable pour l'image du logo
SDL_Texture* logoTexture = NULL;
SDL_Texture* scoreTexture = NULL;

SDL_Texture* aboutTexture = NULL;

SDL_Texture* bodyTexture = NULL;

// Variables pour l'affichage du score
TTF_Font* font = NULL;
TTF_Font* emojiFont = NULL;
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
void load_controller_image(){
    controllerTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/Commandes.png");
    if (!controllerTexture) {
        printf("Erreur chargement image obstacle : %s\n", IMG_GetError());
    }
}
void load_about_image(){
    aboutTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/about.jpeg");
    if (!aboutTexture) {
        printf("Erreur chargement image obstacle : %s\n", IMG_GetError());
    }
}

void update_score_texture() {
    char scoreText[100]; // pour éviter tout dépassement dz mémoire
    
    if (sel_num_players ==2){
        sprintf(scoreText, "Joueur 1: %d pts - Vies: %d | Joueur 2: %d pts - Vies: %d",
            snakes[0].score, snakes[0].lives, sel_num_players == 2 ? snakes[1].score : 0, sel_num_players == 2 ? snakes[1].lives : 0);
        }
    else{
    sprintf(scoreText, "Joueur 1: %d pts - Vies: %d", snakes[0].score, snakes[0].lives);
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    if (textSurface) {
        if (scoreTexture) SDL_DestroyTexture(scoreTexture); // Supprime l'ancienne texture
        scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        scoreRect = (SDL_Rect){20,0, textSurface->w, textSurface->h}; // Position en haut à gauche
        rect_resize(&scoreRect,0,HEADER_HEIGHT);
        SDL_FreeSurface(textSurface);
    }
}

void load_body_image(){
    snakes[0].body_texture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/snake_body.png");
    if (!snakes[0].body_texture) {
        printf("Erreur chargement texture snake 0 %s\n", IMG_GetError());
    }
    snakes[1].body_texture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/snake_body2.png");
    if (!snakes[1].body_texture) {
        printf("Erreur chargement texture snake 1 : %s\n", IMG_GetError());
    }
}

void load_button_texture(){
    aboutButtonNormalTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/ABOUT/normal.png");
    if (!aboutButtonNormalTexture) {
        printf("Erreur chargement texture bouton \"A propos\" à l'état normal : %s\n", IMG_GetError());
    }
    aboutButtonSelectedTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/ABOUT/selected.png");
    if (!aboutButtonSelectedTexture) {
        printf("Erreur chargement texture bouton \"A propos\" à l'état sélectionné : %s\n", IMG_GetError());
    }
    returnButtonTexture = IMG_LoadTexture(renderer, "Snakefinal/cosmetiques/RETURN/selected.png");
    if (!returnButtonTexture) {
        printf("Erreur chargement texture bouton de retour : %s\n", IMG_GetError());
    }    
}

//Retourne un pointeur vers une texture issue d'une capture de la fenêtre
SDL_Texture *get_screenshot_texture(){
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, WIDTH, HEIGHT+HEADER_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_Texture *sshotTxtr=SDL_CreateTextureFromSurface(renderer,sshot);
    return sshotTxtr;
}

void rectblock_draw(rectBlock_t *rb){
    SDL_SetRenderDrawColor(renderer,rb->fillColor.r,rb->fillColor.g,rb->fillColor.b,rb->fillColor.a);
    SDL_RenderFillRect(renderer,rb->rect);
    SDL_SetRenderDrawColor(renderer,rb->drawColor.r,rb->drawColor.g,rb->drawColor.b,rb->drawColor.a);
    SDL_RenderDrawRect(renderer,rb->rect);
}
void rectblock_draw_with_text(rectBlock_t *rb,char *txt){
    rectblock_draw(rb);
    renderer_print_text(rb->rect,rb->drawColor,txt);
}
void renderer_print_text(SDL_Rect *r,SDL_Color c, char *txt){
    SDL_Surface *rb_surf=TTF_RenderText_Solid(font,txt,c);
    //Texture du bouton "QUIT"
    SDL_Texture* rb_texture = SDL_CreateTextureFromSurface(renderer, rb_surf);
    SDL_RenderCopy(renderer, rb_texture, NULL, r);
    SDL_FreeSurface(rb_surf);
    SDL_DestroyTexture(rb_texture);
}

void rect_resize(SDL_Rect *r, int w, int h){
    int nw=0;
    int nh=0;
    if((w>0)||(h>0)){
        if(w==0){
            nw=r->w*h/r->h;
            nh=h;
        }
        else if(h==0){
            nh=r->h*w/r->w;
            nw=w;
        }
        else{
            nh=h;
            nw=w;
        }
        r->w=nw;
        r->h=nh;
    }

}
