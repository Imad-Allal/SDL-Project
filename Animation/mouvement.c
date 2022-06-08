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

int xmin, xmax, ymin, ymax;
int x, y;
int ux = 1, uy = 1;

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

        y1 = cy - sqrt(pow(r, 2) - pow((x0 - cx), 2));
        horizontale(x0, y1, dx);
        x0++;
        x--;
    }
}

void mouvement(int r){
    int stop = 0;

    while (stop<3){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        xmin = x - r;
        ymin = y - r;
        xmax = x + r;
        ymax = y + r;

        if (xmax == WIDTH)
            ux = -ux;
        if (ymax == HEIGHT){
            uy = -uy;
            stop++;
        }
        if (xmin == 0)
            ux = -ux;
        if (ymin == 0)
            uy = -uy;

        x = x + ux;
        y = y + uy;

        disque(x, y, r);
        SDL_RenderPresent(renderer);
        SDL_Delay(0.1);
    }
}

int main(int argc, char *argv[])
{
    int r = 100;

    printf("C: Reprendre le mouvement\nA: Quitter\n");

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
        fprintf(stderr,"Color error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    points[0].x = 400;
    points[0].y = 400;

    xmin = points[0].x - r;
    ymin = points[0].y - r;
    xmax = points[0].x + r;
    ymax = points[0].y + r;

    disque(points[0].x, points[0].y, r);

    SDL_RenderPresent(renderer);

    x = points[0].x;
    y = points[0].y;

    mouvement(r);

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
                    case SDLK_c:
                        printf("Reprise du mouvement\n");
                        mouvement(r);
                        break;
                    default:
                        printf("Vous avez appuyé sur la touche : %c\n", event.key.keysym.sym);
                        continue;
                    }
                    break;
                case SDL_QUIT: quitter = SDL_TRUE;
                    quit();
                    break;
                }
            }
    }

    quit();
    return EXIT_SUCCESS;
}
