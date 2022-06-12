#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 8
#define POL 6
#define PNT 4
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Point points[6][4];

struct Oeil{
    int ox,oy,oz;
};

struct Lumiere{
    int lx,ly,lz;
};

struct Prl{
    int px,py,pz;
};

struct Equation{
    int x, y, z, e;
    float tq;
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

int bordGauche(int xmin, int index){
    for (int i = 0; i < PNT; i++){
        if (points[index][i].x == xmin){
            return i;
        }
    }
}
int bordDroit(int xmax, int index){
    for (int i = 0; i < PNT; i++){
        if (points[index][i].x == xmax){
            return i;
        }
    }
}

SDL_Point listeP1(int min, int max, SDL_Point p[], int taille, int index){
    int i = min, j=0;
    while (i != max)
    {
        p[j] = points[index][i];
        j++;
        if (i == PNT-1)
            i = 0;
        else
            i++;
    }
    p[j] = points[index][i];
    return *p;
}

SDL_Point listeP2(int min, int max, SDL_Point p[], int taille, int index){
    int i = min, j=0;
    while (i != max)
    {
        p[j] = points[index][i];
        j++;
        if (i == 0)
            i = PNT-1;
        else
            i--;
    }
    p[j] = points[index][i];
    return *p;
}

void remplirPolygone(int xmin, int xmax, int index){
    printf("test\n");
    int y0, y1;
    int x = xmin;
    int i = 0, j = 0;
    int cpt1 = 0, cpt2 = 0;
    int min = bordGauche(xmin, index), max = bordDroit(xmax, index);
    SDL_Point p1[2], p2[2];
    listeP1(min, max, p1, 2, index);
    listeP2(min, max, p2, 2, index);
    //printf("%d, %d\n", ); 

    while (x < xmax){
        if(x == p1[i+1].x){
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

void polygones(){
    int j = 0;
    for (int i = 0; i < PNT; i++)
    {
        points[0][i].x = q[j].px;
        points[0][i].y = q[j].py;
        j++;
    }
    j = 3;
    for (int i = 0; i < PNT; i++)
    {
        points[1][i].x = q[j].px;
        points[1][i].y = q[j].py;
        if (j==5)
            j = 2;
        else
            j++;
    }
    j = 2;
    for (int i = 0; i < PNT; i++)
    {
        points[2][i].x = q[j].px;
        points[2][i].y = q[j].py;
        if (j==1)
            j = 6;
        else
            j--;
    }
    j = 5;
    for (int i = 0; i < PNT; i++)
    {
        points[3][i].x = q[j].px;
        points[3][i].y = q[j].py;
        if (j==4)
            j = 7;
        else
            j--;
    }
    j = 6;
    for (int i = 0; i < PNT; i++)
    {
        points[4][i].x = q[j].px;
        points[4][i].y = q[j].py;
        if (j==0)
            j = 7;
        else if (j==1)
            j=0;
        else
            j = 1;
    }
    j = 7;
    for (int i = 0; i < PNT; i++)
    {
        points[5][i].x = q[j].px;
        points[5][i].y = q[j].py;
        if (j==7 || j==3)
            j -= 3;
        else
            j--;
    }
}

int main(int argc, char *argv[])
{

    int xmin, xmax, ymin, ymax;

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
    polygones();

    /*Polygones parallelepipede*/

    for (int i = 0; i < POL; i++)
    {
        for (int j = 0; j < PNT; j++)
        {
            SDL_RenderDrawPoint(renderer, points[i][j].x, points[i][j].y);

            SDL_RenderPresent(renderer);
                        SDL_Delay(1000);
        }
    }
    SDL_Delay(1000);
    for (int i = 0; i < POL; i++)
    {
        for (int j = 0; j < PNT; j++)
        {
            if (j==PNT-1)
                algoNaif(points[i][j].x, points[i][j].y, points[i][0].x, points[i][0].y);
            else
                algoNaif(points[i][j].x, points[i][j].y, points[i][j+1].x, points[i][j+1].y);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
        }
    }
            /*algoNaif(q[0].px, q[0].py, q[4].px, q[4].py);
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
*/
    for (int i = 0; i < POL; i++){
        for (int j = 0; j < PNT; j++){
            xmin = WIDTH;
            xmax = 0;
            ymin = HEIGHT;
            ymax = 0;

            if (points[i][j].x >= xmax)
            {
                xmax = points[i][j].x;
            }
            if (points[i][j].x <= xmin){
                xmin = points[i][j].x;
            }
            if (points[i][j].y >= ymax){
                ymax = points[i][j].y;
            }
            if (points[i][j].y <= ymin){
                ymin = points[i][j].y;
            }
        }
        remplirPolygone(xmin, xmax, i);
        SDL_RenderPresent(renderer);
    }
        /*

        /*Dessiner les points par rapport à l'oeil*/

        /*
            xmin = WIDTH;
            xmax = 0;
            ymin = HEIGHT;
            ymax = 0;

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++){
                    if (points[i].x >= xmax)
                    {
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
            }
                remplirPolygone(xmin, xmax, i);
        */
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
