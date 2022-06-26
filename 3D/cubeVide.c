#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 8
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

struct Oeil{
    int ox,oy,oz;
};

struct Lumiere{
    int lx,ly,lz;
};


struct Equation{
    int x, y, z, e;
    float tq;
};

struct Prl{
    int px,py,pz;
};

struct Prl p1[N] = {
    {400, 400, 400},
    {800, 400, 400},
    {800, 800, 400},
    {400, 800, 400},

    {400, 400, 800},
    {800, 400, 800},
    {800, 800, 800},
    {400, 800, 800}
};

struct Equation eq[N];
struct Prl q[N];

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
    float i, x, y;
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
    float i, x, y;
    if (dx > 0){
        x = x0 + dx;
        for (i = x0; i <= x; i++)
        {
            SDL_RenderDrawPoint(renderer, i, y0);
        }
    }
    else {
        x = x0 + dx;
        for (i = x0; i >= x; i--)
        {
            SDL_RenderDrawPoint(renderer, i, y0);
        }
    }  
}

void algoNaif(int x0, int y0, int x1, int y1){
    float i, x, y;
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
            for (i = 0; i < dx; i++){
                x = x0 + i;
                y = Round(y0 + m * i);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
        else{
            float m = ((float)dy) / ((float)dx);
            for (i = 0; i > dx; i--){
                x = x0 + i;
                y = Round(y0 + m * i);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

void projection(struct Oeil o){
    
    for (int i = 0; i < N; i++)
    {
        eq[i].x = p1[i].px - o.ox;
        eq[i].y = p1[i].py - o.oy;
        eq[i].z = p1[i].pz - o.oz;

        eq[i].tq = (float)-o.oz/(float)eq[i].z;

        q[i].px = eq[i].tq * eq[i].x + o.ox;
        q[i].py = eq[i].tq * eq[i].y + o.oy;
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
        fprintf(stderr,"Color error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    struct Oeil o = {100,250,-300};
        projection(o);
        algoNaif(q[0].px, q[0].py, q[4].px, q[4].py);
        algoNaif(q[1].px, q[1].py, q[5].px, q[5].py);
        algoNaif(q[2].px, q[2].py, q[6].px, q[6].py);
        algoNaif(q[3].px, q[3].py, q[7].px, q[7].py);
        algoNaif(q[0].px, q[0].py, q[1].px, q[1].py);
        algoNaif(q[1].px, q[1].py, q[2].px, q[2].py);
        algoNaif(q[2].px, q[2].py, q[3].px, q[3].py);
        algoNaif(q[3].px, q[3].py, q[0].px, q[0].py);
        algoNaif(q[4].px, q[4].py, q[5].px, q[5].py);
        algoNaif(q[5].px, q[5].py, q[6].px, q[6].py);
        algoNaif(q[6].px, q[6].py, q[7].px, q[7].py);
        algoNaif(q[7].px, q[7].py, q[4].px, q[4].py);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

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
