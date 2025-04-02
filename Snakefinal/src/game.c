#include "game.h"

//Jouer
void play_game(){
    Uint32 start_time = SDL_GetTicks();

    while (running&&playing)
    {
        Uint32 current_time = SDL_GetTicks();
        Uint32 delta_time = current_time - start_time;

        // Gestion des événements
        handle_input();

        // Mise à jour du jeu seulement si le délai est passé
        if (current_time - last_update_time >= MOVE_DELAY)
        {
            update_game();
            last_update_time = current_time;
        }

        // Dessiner le jeu à chaque itération
        draw_game();

        // Limiter la boucle à environ 60 FPS pour pouvoir jouer en même temps
        if (delta_time < (100 / 60))
        {
            SDL_Delay((1000 / 60) - delta_time);
        }

        start_time = SDL_GetTicks(); // Réinitialiser le start_time pour maintenir un rythme constant

        update_score_texture();
    }
    
}

//Afficher l'écran de game over
void show_end_screen(){
        SDL_Event event;
        int selection = 1; // 1 joueur par défaut
    
        load_logo_image();
    
        while (running&&(!playing)) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
    
            // Afficher le logo
            if (screenshot) {
                int logoWidth, logoHeight;
               
                SDL_QueryTexture(screenshot, NULL, NULL, &logoWidth, &logoHeight);
                SDL_Rect logoRect = {0,0, logoWidth, logoHeight};
                SDL_RenderCopy(renderer, screenshot, NULL, &logoRect);

                
            }
            else if(logoTexture){
                int logoWidth, logoHeight;
                SDL_QueryTexture(logoTexture, NULL, NULL, &logoWidth, &logoHeight);
                SDL_Rect logoRect = {0,0, logoWidth, logoHeight};
                SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
            }          

            SDL_Color colorNormal = {255, 255, 255, 255}; // Blanc normal
            
            
            SDL_Rect rect;
            rect.w=200;
            rect.h=40;
            rect.x=(WIDTH-rect.w)/2;
            rect.y=(HEIGHT-rect.h)/2;
            SDL_SetRenderDrawColor(renderer,0,0,0,127);
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderDrawRect(renderer,&rect);
            //Surface du texte "GAME OVER"
            SDL_Surface *go_surf=TTF_RenderText_Solid(font,"GAME OVER",colorNormal);
            //Texture du texte "GAME OVER"
            SDL_Texture* go_texture = SDL_CreateTextureFromSurface(renderer, go_surf);
            SDL_RenderCopy(renderer, go_texture, NULL, &rect);
            SDL_FreeSurface(go_surf);
            SDL_DestroyTexture(go_texture);

            //Couleur de police du bouton "REPLAY"
            SDL_Color rep_txt_color={(selection==1)?255:0,255,(selection==1)?255:0,255};
            rect.x=(3*WIDTH-2*rect.w)/4;
            rect.y=3*HEIGHT/4;
            SDL_SetRenderDrawColor(renderer,0,(selection==1)?255:0,0,(selection==1)?255:127);
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer,(selection==1)?255:0,255,(selection==1)?255:0,255);
            SDL_RenderDrawRect(renderer,&rect);
            //Surface du bouton "REPLAY"
            SDL_Surface *rep_surf=TTF_RenderText_Solid(font,"REPLAY",rep_txt_color);
            //Texture du bouton "REPLAY"
            SDL_Texture* rep_texture = SDL_CreateTextureFromSurface(renderer, rep_surf);
            SDL_RenderCopy(renderer, rep_texture, NULL, &rect);
            SDL_FreeSurface(rep_surf);
            SDL_DestroyTexture(rep_texture);

            //Couleur de police du bouton "QUIT"
            SDL_Color quit_txt_color={255,(selection==0)?255:0,(selection==0)?255:0,255};
            rect.x=(WIDTH-2*rect.w)/4;
            rect.y=3*HEIGHT/4;
            SDL_SetRenderDrawColor(renderer,(selection==0)?255:0,0,0,(selection==0)?255:127);
            SDL_RenderFillRect(renderer,&rect);
            SDL_SetRenderDrawColor(renderer,255,(selection==0)?255:0,(selection==0)?255:0,255);
            SDL_RenderDrawRect(renderer,&rect);
            //Surface du bouton "QUIT"
            SDL_Surface *quit_surf=TTF_RenderText_Solid(font,"QUIT",quit_txt_color);
            //Texture du bouton "QUIT"
            SDL_Texture* quit_texture = SDL_CreateTextureFromSurface(renderer, quit_surf);
            SDL_RenderCopy(renderer, quit_texture, NULL, &rect);
            SDL_FreeSurface(quit_surf);
            SDL_DestroyTexture(quit_texture);

            SDL_RenderPresent(renderer);

    
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_r || event.key.keysym.sym == SDLK_DOWN) {
                        replay();                   
                    }
                    if (event.key.keysym.sym == SDLK_e) {
                        quit();   
                    }
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        if(selection<1){
                            selection++;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        if(selection>0){
                            selection--;
                        }
                    }
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        switch (selection)
                        {
                        case 0:
                            quit();
                            break;
                        case 1:
                            replay();
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    
    
    
        // Libérer la texture du logo après utilisation
        if (logoTexture)
        {
            SDL_DestroyTexture(logoTexture);
        }
}

/*Rejouer :
- Revenir en mode "jeu"
- Reinitialiser les snakes
- Remettre à 0 obstacle
*/
void replay(){
    playing=true;
    reset_completely_both_snakes();
    num_obstacles = 0;
}
//Quitter le jeu
void quit(){
    playing=false;
    running=false;
}

// Gère les entrées clavier et événements
void handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            // Contrôles pour le Snake 1
            case SDLK_UP:
                if (snakes[0].direction != 'd')
                    snakes[0].direction = 'u';
                break;
            case SDLK_DOWN:
                if (snakes[0].direction != 'u')
                    snakes[0].direction = 'd';
                break;
            case SDLK_LEFT:
                if (snakes[0].direction != 'r')
                    snakes[0].direction = 'l';
                break;
            case SDLK_RIGHT:
                if (snakes[0].direction != 'l')
                    snakes[0].direction = 'r';
                break;

            // Contrôles pour le Snake 2
            case SDLK_z:
                if (snakes[1].direction != 'd')
                    snakes[1].direction = 'u';
                break;
            case SDLK_s:
                if (snakes[1].direction != 'u')
                    snakes[1].direction = 'd';
                break;
            case SDLK_q:
                if (snakes[1].direction != 'r')
                    snakes[1].direction = 'l';
                break;
            case SDLK_d:
                if (snakes[1].direction != 'l')
                    snakes[1].direction = 'r';
                break;
            }
        }
    }
}
