#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>

#define N 8
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

struct Cercle{
    int cx, cy, cz, r;
};

struct Point
{
    int x,y,z;
};

struct Equation{
    int x, y, z, e;
    float tq;
};

struct Cercle boule = {1000,1000,400,200};
struct Point l = {1000,1000,500};
struct Point o = {250,250,-500};
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

void projection(struct Point p[2]){
    
    for (int i = 0; i < 2; i++)
    {
        eq[i].x = p[i].x - o.x;
        eq[i].y = p[i].y - o.y;
        eq[i].z = p[i].z - o.z;

        eq[i].tq = (float)-o.z/(float)eq[i].z;

        p[i].x = eq[i].tq * eq[i].x + o.x;
        p[i].y = eq[i].tq * eq[i].y + o.y;

        SDL_RenderDrawPoint(renderer, p[i].x, p[i].y);
    }
}

void intersection(int px, int py, int pz){
    float a, b, c, t0, t1;
    struct Point e;
    struct Point p[2];
    e.x = px - o.x;
    e.y = py - o.y;
    e.z = pz - o.z; // a verifier
    //printf("e : %d, %d, %d\n", e.x, e.y, e.z);

    a = e.x*e.x + e.y*e.y + e.z*e.z;
    b = 2 * e.x * (o.x - boule.cx) + 2 * e.y * (o.y - boule.cy) + 2 * e.z * (o.z - boule.cz);
    c = (o.x - boule.cx) * (o.x - boule.cx) + (o.y - boule.cy) * (o.y - boule.cy) + (o.z - boule.cz) * (o.z - boule.cz) - (boule.r * boule.r);

    float rac = sqrt((b * b) - (4 * a * c));
    //printf("a : %f, b : %f, c : %f, rac : %f\n", a, b, c, rac);

    t0 = (-b+rac)/(2 * a);
    t1 = (-b-rac)/(2 * a);

    //printf("t0 = %f, t1 = %f\n", t0, t1);

    p[0].x = t0 * e.x + o.x;
    p[0].y = t0 * e.y + o.y;
    p[0].z = t0 * e.z + o.z;

    p[1].x = t1 * e.x + o.x;
    p[1].y = t1 * e.y + o.y;
    p[1].z = t1 * e.z + o.z;

    //printf("p0 : %d, %d, %d\n", p[0].x, p[0].y, p[0].z);
    //printf("p1 : %d, %d, %d\n", p[1].x, p[1].y, p[1].z);

    projection(p);
}

void verticale (int x0, int y0, int y1, int z){
    //printf("VERTICAAAALE\n");
    int i, x, y;
    int dy = y1 - y0;
    if (dy > 0)
    {
        for (i = y0; i <= y1; i++)
        {
            //printf("x0 : %d, y0 : %d\n", x0, i);
            SDL_RenderDrawPoint(renderer, x0, i);
            SDL_RenderPresent(renderer);
            intersection(x0, i, z);
        }
    }
    else{
        y = y0 + dy;
        for (i = y0; i >= y1; i--)
        {
            //printf("x0 : %d, y0 : %d\n", x0, i);
            intersection(x0, i, z);
        }
    }    
}
/*
void projection(){
    
    for (int i = 0; i < N; i++)
    {
        eq[i].x = boule.cx - o.x;
        eq[i].y = boule.cy - o.y;
        eq[i].z = boule.cz - o.z;

        eq[i].tq = (float)-o.z/(float)eq[i].z;

        cercle.cx = eq[i].tq * eq[i].x + o.x;
        cercle.cy = eq[i].tq * eq[i].y + o.y;
    }
}
*/

void rayon(){
    float oc1, oc2;
    oc1 = sqrt((boule.cx - o.x) * (boule.cx - o.x) + (boule.cy - o.y) * (boule.cy - o.y) + (boule.cz - o.z) * (boule.cz - o.z));
    oc2 = sqrt((boule.cx - o.x) * (boule.cx - o.x) + (boule.cy - o.y) * (boule.cy - o.y));

    cercle.r = boule.r * oc2 / oc1;
}

void disque(struct Cercle cercle){
    int x0 = cercle.cx - cercle.r;
    int x1 = cercle.cx + cercle.r;
    int x = x1;
    float y0, y1;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = cercle.cy + sqrt((cercle.r * cercle.r) - (x0 - cercle.cx) * (x0 - cercle.cx));
        y1 = cercle.cy - sqrt((cercle.r * cercle.r) - (x0 - cercle.cx) * (x0 - cercle.cx));
        //printf("x0 = %d, y0 = %f, y1 = %f\n", x0, y0, y1);
        verticale(x0, y0, y1, cercle.cz);
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

        rayon();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        disque(boule);
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
