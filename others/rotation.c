#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#define N 4
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Point points[N];
SDL_Point points2[N];
int n = 0;

void quit(){
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}


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

void rotate (int x, int y, int cx, int cy, double angle){
    points[n].x = cx + (x - cx) * cos(angle) - (y - cy) * sin(angle);
    points[n].y = cy + (x - cy) * sin(angle) + (y - cy) * cos(angle);
}

int main(int argc, char *argv[])
{
    int cx, cy;
    double angle = 0.1;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL creation error : %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Rotation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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

    points[0].x = 300;
    points[0].y = 400;
    points[1].x = 500;
    points[1].y = 400;
    points[2].x = 500;
    points[2].y = 300;
    points[3].x = 300;
    points[3].y = 300;

    for (int i = 0; i<N; i++){
    if(i == N-1){ 
        algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
    }
    else
        algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    cx = ((points[1].x - points[0].x) / 2) + points[0].x;
    cy = ((points[2].y - points[1].y) / 2) + points[1].y;

    while(angle < 0.175){
        for (int i = 0; i<N; i++){
            if(i == N-1){ 
                algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
            }
            else
                algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(15);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);



        for (int i = 0; i<N; i++){
            rotate(points[i].x, points[i].y, cx, cy, angle);
            n++;
        }
        n = 0;
        angle += 0.001;
    }
    for (int i = 0; i<N; i++){
        if(i == N-1){ 
            algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
        }
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
