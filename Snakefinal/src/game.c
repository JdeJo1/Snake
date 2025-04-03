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
            
            rectBlock_t go_rb, rep_rb, quit_rb;
            SDL_Rect rect;

            go_rb.fillColor=(SDL_Color){0,0,0,127};
            go_rb.drawColor=(SDL_Color){255,255,255,255};
            go_rb.rect=&rect;
            rect.w=200;
            rect.h=40;
            rect.x=(WIDTH-rect.w)/2;
            rect.y=(HEIGHT-rect.h)/2;
            rectblock_draw_with_text(&go_rb,"GAME OVER");
            
            rep_rb.fillColor=(SDL_Color){0,(selection==1)?255:0,0,(selection==1)?255:127};
            rep_rb.drawColor=(SDL_Color){(selection==1)?255:0,255,(selection==1)?255:0,255};
            rep_rb.rect=&rect;
            rect.x=(3*WIDTH-2*rect.w)/4;
            rect.y=3*HEIGHT/4;
            rectblock_draw_with_text(&rep_rb,"REPLAY");

            quit_rb.fillColor=(SDL_Color){(selection==0)?255:0,0,0,(selection==0)?255:127};
            quit_rb.drawColor=(SDL_Color){255,(selection==0)?255:0,(selection==0)?255:0,255};
            quit_rb.rect=&rect;
            rect.x=(WIDTH-2*rect.w)/4;
            rect.y=3*HEIGHT/4;
            rectblock_draw_with_text(&quit_rb,"QUIT");

            display_scores();

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
void display_scores(){
    bool blk_2h=((SDL_GetTicks()%500)<250);
    bool disp_p1=false, disp_p2=false;
    if(sel_num_players==2){
        disp_p1=((snakes[0].score<snakes[1].score)||blk_2h);
        disp_p2=((snakes[0].score>snakes[1].score)||blk_2h);
    }
    else{
        disp_p1=true;
    }
    rectBlock_t rb;
    SDL_Rect rect, txt_rect;
    rb.rect=&rect;
    rb.fillColor=(SDL_Color){127,127,127,127};
    rb.drawColor=(SDL_Color){255,255,255,255};
    rect.w=200;
    rect.h=80;
    rect.x=(WIDTH/2-rect.w)/2;
    rect.y=(HEIGHT/2-rect.h)/2;
    rectblock_draw(&rb);
    char score_txt[20];
    sprintf(score_txt,"Score : %02d",snakes[0].score);
    txt_rect.x=rect.x;
    txt_rect.y=rect.y;
    txt_rect.w=rect.w;
    txt_rect.h=rect.h/2;
    if(disp_p1) renderer_print_text(&txt_rect,(SDL_Color){0,255,0,255},score_txt);

    sprintf(score_txt,"Score : %02d",snakes[1].score);
    txt_rect.y+=txt_rect.h;
    if(disp_p2) renderer_print_text(&txt_rect,(SDL_Color){255,0,0,255},score_txt);
    

}