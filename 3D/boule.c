#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 8
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

struct Cercle{
    int cx, cy, cz, r;
};

struct Oeil
{
    int ox,oy,oz;
};

struct Lumiere{
    int lx,ly,lz;
};

struct Equation{
    int x, y, z, e;
    float tq;
};

struct Cercle boule = {1000,1000,400,200};
struct Lumiere l = {1000,1000,500};
struct Oeil o = {250,250,-500};
struct Equation eq[N];
struct Cercle cercle;


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

void verticale (int x0, int y0, int y1){
    int i, x, y;
    int dy = y1 - y0;
    if (dy > 0)
    {
        for (i = y0; i <= y1; i++)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
        }
    }
    else{
        y = y0 + dy;
        for (i = y0; i >= y1; i--)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
        }
    }    
}

void projection(struct Oeil o){
    
    for (int i = 0; i < N; i++)
    {
        eq[i].x = boule.cx - o.ox;
        eq[i].y = boule.cy - o.oy;
        eq[i].z = boule.cz - o.oz;

        eq[i].tq = (float)-o.oz/(float)eq[i].z;

        cercle.cx = eq[i].tq * eq[i].x + o.ox;
        cercle.cy = eq[i].tq * eq[i].y + o.oy;
    }
}

void rayon(){
    float oc1, oc2;
    oc1 = sqrt((boule.cx - o.ox) * (boule.cx - o.ox) + (boule.cy - o.oy) * (boule.cy - o.oy) + (boule.cz - o.oz) * (boule.cz - o.oz));
    oc2 = sqrt((boule.cx - o.ox) * (boule.cx - o.ox) + (boule.cy - o.oy) * (boule.cy - o.oy));

    cercle.r = boule.r * oc2 / oc1;
}

void disque(){
    int x0 = cercle.cx - cercle.r;
    int x1 = cercle.cx + cercle.r;
    int x = x1;
    float y0, y1;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = cercle.cy + sqrt((cercle.r * cercle.r) - (x0 - cercle.cx) * (x0 - cercle.cx));
        y1 = cercle.cy - sqrt((cercle.r * cercle.r) - (x0 - cercle.cx) * (x0 - cercle.cx));

        verticale(x0, y0, y1);
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
        fprintf(stderr,"Color error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    struct Oeil o = {250,250,-300};
        projection(o);
        rayon();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        printf("disque : cx = %d, cy = %d, r = %d\n", cercle.cx, cercle.cy, cercle.r);
        disque();
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_bool quitter = SDL_FALSE;

        while (!quitter)
        {
            while (SDL_WaitEvent(&event))
            {
                switch (event.type)
                {
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
