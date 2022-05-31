#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 3
#define WIDTH 1000
#define HEIGHT 1500

SDL_Point points[N];

int Round(float x){
    return (int)(x + 0.5);
}

void dessin(int x0, int y0, int dx, int dy, SDL_Renderer *renderer){
    int x = x0;
    float y = y0;
    float m = ((float)dy) / ((float)dx);
    int x1 = x0 + dx;
    for (x = x0; x < x1; x++)
    {
        SDL_RenderDrawPoint(renderer, x, Round(y));
        y += m;
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
    points[1].x = 400;
    points[1].y = 200;

    int dx = points[1].x - points[0].x;
    int dy = points[1].y - points[0].y;
    dessin(points[0].x, points[0].y, dx, dy, renderer);
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
