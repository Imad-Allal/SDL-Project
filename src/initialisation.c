#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

/*Initialisation de la SDL*/
int initialisation(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur creation SDL : %s", SDL_GetError());
        return 0;
        SDL_Quit();
    }
    return 1;
}

/*Création de la fenêtre de travail*/
int initWindow(SDL_Window *window, char* nom, int height, int width){
    window = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width, SDL_WINDOW_SHOWN);
    if (window == NULL){
        fprintf(stderr, "Erreur creation fenetre: %s", SDL_GetError());
        return 0;
    }
    return 1;
}

/*Création du rendu du travail*/
int initRenderer(SDL_Renderer *renderer, SDL_Window *window){
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL) {
        fprintf(stderr,"Erreur renderer %s", SDL_GetError());
        return 0;
    }
    return 1;
}