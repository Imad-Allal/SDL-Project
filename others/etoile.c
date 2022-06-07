#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#define N 4
#define WIDTH 1500
#define HEIGHT 1000
#define PI 3.141592653589793

SDL_Window *window;
SDL_Renderer *renderer;
int n = 0;

void quit(){
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Point points[N*2];

int Round(float x){
    return (int)(x + 0.5);
}

void verticale (int x0, int y0, int dy){
    int i, x, y;
    if (dy > 0){
        y = dy + y0;
        for (i = y0; i <= y; i++)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
        }
    }
    else{
        y = y0 + dy;
        for (i = y0; i >= y; i--)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
        }
    }    
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

void algoNaif(int x0, int y0, int x1, int y1){
    int i, x, y;
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx == 0)
    {
        verticale(x0, y0, dy);
    }

    else if (dy == 0){
        horizontale(x0, y0, dx);
    }
    else{
        if (dx > 0){
            float m = ((float)dy) / ((float)dx);
            for (int i = 0; i < dx; i++){
                x = x0 + i;
                y = Round(y0 + m * i);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        else{
            float m = ((float)dy) / ((float)dx);
            for (int i = 0; i > dx; i--){
                x = x0 + i;
                y = Round(y0 + m * i);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void etoile(int cx, int cy, int r){
    float angle = 0;
    int r2 = r / 2;
    for (int i = 0; i < (N * 2); i++)
    {
        if (i % 2 == 0)
        {
            points[i].x = cx + r * cos((2 * PI / N) + angle);
            points[i].y = cy + r * sin((2 * PI / N) + angle);
        }

        else
        {
            points[i].x = cx + r2 * cos((2 * PI / 2 * N) + (2 * PI / N) + angle);
            points[i].y = cy + r2 * sin((2 * PI / 2 * N) + (2 * PI / N) + angle);
        }
        angle += PI / N;
    }
}

void rotate (int x, int y, int cx, int cy, double angle){
    int x1, y1;
    points[n].x = cx + (x - cx) * cos(angle) - (y - cy) * sin(angle);
    points[n].y = cy + (x - cy) * sin(angle) + (y - cy) * cos(angle);
}

int main(int argc, char *argv[])
{

    int angle = 0.1;
    int cx, cy;

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

    etoile(400, 400, 100);

    for (int i = 0; i < N*2; i++){
        if (i == (N*2)-1)
            algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
        else
            algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(100);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    cx = 400;
    cy = 400;

    while(angle < 0.5){
        for (int i = 0; i < N*2; i++){
            if (i == (N*2)-1)
                algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
            else
                algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(15);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);



        for (int i = 0; i<(N*2); i++){
            rotate(points[i].x, points[i].y, cx, cy, angle);
            n++;
        }
        n = 0;
        angle += 0.1;
    }
    for (int i = 0; i < N*2; i++){
        if (i == (N*2)-1)
            algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
        else
            algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }  

    SDL_RenderPresent(renderer);

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
