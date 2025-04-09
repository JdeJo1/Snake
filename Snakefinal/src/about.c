#include "about.h"

SDL_Texture *aboutButtonNormalTexture=NULL;
SDL_Texture *aboutButtonSelectedTexture=NULL;

SDL_Texture *returnButtonTexture=NULL;

void draw_about_screen(){
    SDL_Event event;
    bool about_running = true;

    load_logo_image();

    while (about_running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        if(aboutTexture){
            SDL_Rect rect1 = {0,0, WIDTH, HEADER_HEIGHT+HEIGHT};
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderFillRect(renderer,&rect1);

            SDL_Point pt;
            SDL_QueryTexture(aboutTexture, NULL,NULL,&pt.x,&pt.y);
            rect1 =(SDL_Rect){0,0, pt.x, pt.y};
            SDL_RenderCopy(renderer, aboutTexture, NULL, &rect1);

            if(returnButtonTexture){
                rect1 =(SDL_Rect){-50+WIDTH/2,HEADER_HEIGHT+HEIGHT-150, 100, 100};
                SDL_RenderCopy(renderer,returnButtonTexture,NULL,&rect1);
            }
            else{
                SDL_Surface *gb_surf=TTF_RenderText_Solid(font,"RETOUR",(SDL_Color){255,255,0,255});
                SDL_Texture *gb_txtr=SDL_CreateTextureFromSurface(renderer,gb_surf);

                rect1 =(SDL_Rect){(WIDTH-gb_surf->w)/2,HEADER_HEIGHT+HEIGHT-gb_surf->h-50, gb_surf->w, gb_surf->h};
                
                SDL_SetRenderDrawColor(renderer,0,0,0,127);
                SDL_RenderFillRect(renderer,&rect1);
                SDL_SetRenderDrawColor(renderer,255,255,0,255);
                SDL_RenderDrawRect(renderer,&rect1);
                SDL_RenderCopy(renderer,gb_txtr,NULL,&rect1);

                SDL_FreeSurface(gb_surf);
                SDL_DestroyTexture(gb_txtr);
            }
            
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                about_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                
                if (event.key.keysym.sym == SDLK_RETURN) {
                    about_running = false;
                }
            }
        }
    }
}
