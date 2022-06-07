#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>

#define N 6
#define WIDTH 1500
#define HEIGHT 1000

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

SDL_Point points[N];
SDL_Point rect[4];

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

int Fsigne (SDL_Point p0, SDL_Point p1, int xm, int ym){
    int dx, dy, f;
    dx = p1.x - p0.x;
    dy = p1.y - p0.y;
    f = dy * (xm - p0.x) - dx * (ym - p0.y);

    if (dx<0)
        return (-f);
    return f;
}

int decoupHaut(int xmax){
    for (int i = 0; i < N; i++){
        if (points[i].x == xmax)
            return i;
    }
}

void remplirPolygone(int xmin, int xmax, int ymin, int ymax){
    int x, y = ymin;
    int cle = 1;
    int extremiteMax = decoupHaut(xmax);
    int extremiteMin = N-1;
    while (y < ymax)
    {
        x = xmin;
        while (x < xmax){
            for (int i = 0; i < N; i++){
                if(i < extremiteMax){
                    if (Fsigne(points[i], points[i + 1], x, y) > 0){
                        break;
                    }
                }
                else{
                    if (i == N - 1){
                        if (Fsigne(points[i], points[0], x, y) < 0)
                        {
                            break;
                        }
                        else {
                            //printf("%d\n", Fsigne(points[i], points[0], x, y));
                            SDL_RenderDrawPoint(renderer, x, y);
                        }
                    }
                    else{
                        if (Fsigne(points[i], points[i+1], x, y) < 0){
                                break;
                        }
                    }
                }
            }
            x++;
        }
        y++;
    }
}

int main(int argc, char *argv[])
{
    int xmin, xmax, ymin, ymax;

    printf("T: Tracer le polygone\nR: Tracer le rectangle entourant le polygone\nP: Remplir le polygone\nE: Nettoyer la fenêtre\nA: Quitter\n\n");

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

    points[0].x = 200;
    points[0].y = 500;
    points[1].x = 350;
    points[1].y = 200;
    points[2].x = 650;
    points[2].y = 350;
    points[3].x = 800;
    points[3].y = 600;
    points[4].x = 400;
    points[4].y = 800;
    points[5].x = 300;
    points[5].y = 700;

    for (int i = 0; i < N; i++){
        if (i == N-1)
            algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
        else
            algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }

    SDL_RenderPresent(renderer);
    // Zone du rectangle d'apres le polygone
    xmin = points[0].x;
    xmax = points[0].x;
    ymin = points[0].y;
    ymax = points[0].y;

    for (int i = 0; i < N; i++)
    {
        if (points[i].x >= xmax){
            xmax = points[i].x;
        }
        if (points[i].x <= xmin){
            xmin = points[i].x;
        }
        if (points[i].y >= ymax){
            ymax = points[i].y;
        }
        if (points[i].y <= ymin){
            ymin = points[i].y;
        }
    }
    //Zone du rectangle d'apres le polygone

    //Rectangle
    rect[0].x = xmin;
    rect[0].y = ymin;
    rect[1].x = xmax;
    rect[1].y = ymin;
    rect[2].x = xmax;
    rect[2].y = ymax;
    rect[3].x = xmin;
    rect[3].y = ymax;


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
                    case SDLK_t:
                        printf("Tracage de polygone\n");
                        for (int i = 0; i < N; i++){
                            if (i == N-1)
                                algoNaif(points[i].x, points[i].y, points[0].x, points[0].y);
                            else
                                algoNaif(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
                        }
                        SDL_RenderPresent(renderer);
                        break;
                    case SDLK_p:
                        printf("Remplissage de polygone\n");
                        remplirPolygone(xmin, xmax, ymin, ymax);
                        SDL_RenderPresent(renderer);
                        break;
                    case SDLK_r:
                        printf("Tracage du rectangle\n");
                        for (int i = 0; i < 4; i++){
                            if(i == 3){ 
                                algoNaif(rect[i].x, rect[i].y, rect[0].x, rect[0].y);
                            }
                            else
                            algoNaif(rect[i].x, rect[i].y, rect[i+1].x, rect[i+1].y);
                        }
                        SDL_RenderPresent(renderer);
                        break;
                    case SDLK_e:
                        printf("Effacer dessin\n");
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        SDL_RenderClear(renderer);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderPresent(renderer);
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
