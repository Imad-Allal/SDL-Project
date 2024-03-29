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

void verticale (int x0, int y0, int y1){
    int i, x, y;
    int dy = y1 - y0;
    if (dy > 0)
    {
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
        verticale(x0, y0, y1);
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

int segSup(int x0, int x1, int y0, int y1, int n){
    int y;
    int dx = x1 - x0;
    int dy = y1 - y0;
    float m = ((float)dy) / ((float)dx);
    
    y = Round(y0 + m * n);
    return y;
}
int segInf(int x0, int x1, int y0, int y1, int k){
    int y;
    int dx = x1 - x0;
    int dy = y1 - y0;
    float m = ((float)dy) / ((float)dx);

    y = Round(y0 + m * k);
    k--;
    return y;
}

int bordGauche(int xmin){
    for (int i = 0; i < N; i++){
        if (points[i].x == xmin){
            return i;
        }
    }
}
int bordDroit(int xmax){
    for (int i = 0; i < N; i++){
        if (points[i].x == xmax){
            return i;
        }
    }
}

int tailleP1(int min, int max){
    int i = min, taille = 1;
    while(i != max){
        taille++;
        if (i == N-1)
            i = 0;
        else
            i++;
    }
    return taille;
}

int tailleP2(int min, int max){
    int i = min, taille = 1;
    while(i != max){
        taille++;
        if (i == 0)
            i = N-1;
        else
            i--;
    }
    return taille;
}

SDL_Point listeP1(int min, int max, SDL_Point p[], int taille){
    int i = min, j=0;
    while (i != max)
    {
        p[j] = points[i];
        j++;
        if (i == N-1)
            i = 0;
        else
            i++;
    }
    p[j] = points[i];
    return *p;
}

SDL_Point listeP2(int min, int max, SDL_Point p[], int taille){
    int i = min, j=0;
    while (i != max)
    {
        p[j] = points[i];
        j++;
        if (i == 0)
            i = N-1;
        else
            i--;
    }
    p[j] = points[i];
    return *p;
}

void remplirPolygone(int xmin, int xmax){

    int y0, y1;
    int x = xmin;
    int i = 0, j = 0;
    int cpt1 = 0, cpt2 = 0; // compteurs qui nous indiquent aux la fonction segSup, segInf, dans quel point du segement on se trouve
    int min = bordGauche(xmin), max = bordDroit(xmax); // Renvoient l'index du bord le plus à gauche et de l'autre le plus à droite
    int taille1 = tailleP1(min, max), taille2 = tailleP2(min, max); // Pour initialiser p1 et p2
    SDL_Point p1[taille1], p2[taille2];
    listeP1(min, max, p1, taille1); // Stock dans p1 tous les segements hauts du polygones
    listeP2(min, max, p2, taille2); // Stock dans p2 tous les segements bas du polygones

    while (x < xmax){
        if(x == p1[i+1].x){ // x == debut du segement p1[i+1] ==> Réinitialiser le compteur cpt1
            i++;
            cpt1 = 0;
        }
        if (x == p2[j + 1].x){
            j++;
            cpt2 = 0;
        }
        if (p1[i].x < p1[i + 1].x)
            y0 = segSup(p1[i].x, p1[i + 1].x, p1[i].y, p1[i + 1].y, cpt1);
        else
            y0 = segInf(p1[i].x, p1[i + 1].x, p1[i].y, p1[i + 1].y, cpt1);
        cpt1++;

        if (p2[j].x < p2[j+1].x)
            y1 = segSup(p2[j].x, p2[j + 1].x, p2[j].y, p2[j + 1].y, cpt2);
        else
            y1 = segInf(p2[j].x, p2[j + 1].x, p2[j].y, p2[j + 1].y, cpt2);
        cpt2++;
        verticale(x, y0, y1);
        SDL_RenderPresent(renderer);
        x++;
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

    window = SDL_CreateWindow("Remplissage polygone verticalemet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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
                        remplirPolygone(xmin, xmax);
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
