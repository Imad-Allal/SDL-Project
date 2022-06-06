#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#define N 3
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

void quit(){
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Point points[N];

int Round(float x){
    return (int)(x + 0.5);
}

void horizontale (int x0, int y0, int dx){
    int i, x, y;
    if (dx > 0){
        x = x0 + dx;
        for (int i = x0; i <= x; i++)
        {
            SDL_RenderDrawPoint(renderer, i, y0);
        }
    }
    else {
        x = x0 + dx;
        for (int i = x0; i >= x; i--)
        {
            SDL_RenderDrawPoint(renderer, i, y0);
        }
    }  
}

void cercle(int cx, int cy , int r){
    int x0 = cx - r;
    int x1 = cx + r;
    int y0, y1;

    while(x0 <= x1){
        y0 = cy + sqrt(pow(r, 2) - pow((x0 - cx), 2));
        SDL_RenderDrawPoint(renderer, x0, round(y0));

        y1 = cy - sqrt(pow(r, 2) - pow((x0 - cx), 2));
        SDL_RenderDrawPoint(renderer, x0, round(y1));

        x0++;
    }
}

void disque(int cx, int cy , int r){
    int x0 = cx - r;
    int x1 = cx + r;
    int x = x1;
    int y0, y1;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = cy + sqrt(pow(r, 2) - pow((x0 - cx), 2));
        horizontale(x0, y0, dx);
        SDL_RenderPresent(renderer);

        y1 = cy - sqrt(pow(r, 2) - pow((x0 - cx), 2));
        horizontale(x0, y1, dx);
        SDL_RenderPresent(renderer);
        x0++;
        x--;
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL creation error : %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("not1txf", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        fprintf(stderr, "Window error: %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL) {
        fprintf(stderr,"Renderer error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {    
        fprintf(stderr,"Collor error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    points[0].x = 400;
    points[0].y = 400;

    cercle(points[0].x, points[0].y, 200);
    disque(points[0].x+200, points[0].y+200, 100);

    SDL_Event event;
    SDL_bool quitter = SDL_FALSE;

    while(!quitter){
        while (SDL_WaitEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        printf("Vous avez demandé à quitter le programme\n");
                        quitter = SDL_TRUE;
                        quit();
                        break;
                    default:
                        printf("Vous avez appuyé sur la touche : %c\n", event.key.keysym.sym);
                        continue;
                    }
                    break;
                case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    default:
                        printf("touche : %c relaché\n",event.key.keysym.sym);
                        continue;
                    }
                case SDL_QUIT: quitter = SDL_TRUE;
                    quit();
                    break;
                }
            }
    }

    quit();
    return EXIT_SUCCESS;
}
