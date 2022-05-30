#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

#define N 3
#define WIDTH 1000
#define HEIGHT 1500

SDL_Point points[N];

int Round(float x){
    return (int)(x + 0.5);
}

void dessin(int x0, int y0, int x1, int y1, SDL_Renderer *renderer){
    int pas = 0;
    float x, y;
    int dx = x1 - x0, dy = y1 - y0;
    float m = (float)dy / (float)dx;
    if (abs(dx) > abs(dy))
        pas = abs(dx);
    else
        pas = abs(dy);

    x = dx / pas;
    y = dy / pas;
    for (int i = 0; i <= pas; i++){
        SDL_RenderDrawPoint(renderer, x0, y0);
        x0 += x;
        y0 += y;
        if (m<1)
            round(y0);
        else if (m>1)
            round(x0);
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur creation SDL : %s", SDL_GetError());
        return EXIT_FAILURE;
        SDL_Quit();
        return EXIT_SUCCESS;
    }

    window = SDL_CreateWindow("NomFenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, HEIGHT,WIDTH, SDL_WINDOW_SHOWN);
    if (window == NULL){
        fprintf(stderr, "Erreur creation fenetre: %s", SDL_GetError());
        goto quit;
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL) {
        fprintf(stderr,"Erreur renderer %s", SDL_GetError());
        goto quit;
    }

    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {
        fprintf(stderr,"Erreur renderer %s", SDL_GetError());
        goto quit;
    }

    points[0].x = 900;
    points[0].y = 500;
    points[1].x = 1000;
    points[1].y = 1200;

    dessin(points[0].x, points[0].y, points[1].x, points[1].y, renderer);

    SDL_RenderPresent(renderer);
    
    SDL_Delay(3000);

quit:
    if (renderer != NULL)
    SDL_DestroyRenderer(renderer);
    if (window != NULL)
    SDL_DestroyWindow(window);

    SDL_Quit();
    EXIT_SUCCESS;
}
