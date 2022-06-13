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

void verticale (int x0, int y0, int y1){
    printf("Verticale\n");
    float i, x, y;
    if (y1 > y0){
        for (i = y0; i <= y1; i++)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
            printf("y0 = %f, y1 = %d\n", i, y1);
        }
    }
    else{
        for (i = y0; i > y1; i--)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
            printf("y0 = %f, y1 = %f, dy = %d\n", i, y1, y);
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

int segInf(int x0, int x1, int y0, int y1, int n){
    float y;
    int dx = x1 - x0;
    int dy = y1 - y0;
    float m = ((float)dy) / ((float)dx);
    if (dx == 0)
        return y0;
    y = Round((float)y0 + m * n);
    return y;
}
int segSup(int x0, int x1, int y0, int y1, int k){
    float y;
    int dx = x1 - x0;
    int dy = y1 - y0;
    float m = ((float)dy) / ((float)dx);
    if (dx == 0)
        return y0;
    y = Round((float)y0 + m * k);
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
    int i = min;

    for (int j = 0; j < 3; j++){
        p[j] = points[index][i];
        printf("xh = %d, yh = %d\n", p[j].x, p[j].y);
        printf("\n");
        if (i == PNT-1)
            i = 0;
        else
            i++;
    }
    return *p;
}

SDL_Point listeP2(int min, int max, SDL_Point p[], int taille, int index){
    int i = min;

    for (int j = 0; j < 3; j++){
        p[j] = points[index][i];
        printf("xb = %d, yb = %d\n", p[j].x, p[j].y);
        printf("\n");
        if (i == 0)
            i = PNT - 1;
        else
            i--;
    }
    return *p;
}

void remplirPolygone(int xmin, int xmax, int index){
    printf("Polygone[%d]\n",index);
    int y0, y1;
    int x = xmin;
    int i = 0, j = 0;
    int cpt1 = 0, cpt2 = 0;
    int min = bordGauche(xmin, index), max = bordDroit(xmax, index);
    SDL_Point p1[3], p2[3];
    listeP1(min, max, p1, 3, index);
    listeP2(min, max, p2, 3, index);

    while (x < xmax){
        if(x == p1[i+1].x){
            i++;
            cpt1 = 0;
        }
        if (x == p2[j + 1].x){
            j++;
            cpt2 = 0;
        }
        if (p1[i].x < p1[i + 1].x){
            y0 = segSup(p1[i].x, p1[i + 1].x, p1[i].y, p1[i + 1].y, cpt1);
        }
        else{
            y0 = segInf(p1[i].x, p1[i + 1].x, p1[i].y, p1[i + 1].y, cpt1);
        }
        cpt1++;
        if (p2[j].x < p2[j + 1].x){
            y1 = segSup(p2[j].x, p2[j + 1].x, p2[j].y, p2[j + 1].y, cpt2);
        }
        else{
            y1 = segInf(p2[j].x, p2[j + 1].x, p2[j].y, p2[j + 1].y, cpt2);
        }
        cpt2++;
        printf("y0 = %d, y1 = %d\n", y0, y1);

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
    int j = 4;
    for (int i = 0; i < PNT; i++)
    {
        points[0][i].x = q[j].px;
        points[0][i].y = q[j].py;
        j++;
    }

    points[1][0].x = q[7].px;
    points[1][0].y = q[7].py;
    points[1][1].x = q[6].px;
    points[1][1].y = q[6].py;
    points[1][2].x = q[2].px;
    points[1][2].y = q[2].py;
    points[1][3].x = q[3].px;
    points[1][3].y = q[3].py;

    points[2][0].x = q[1].px;
    points[2][0].y = q[1].py;
    points[2][1].x = q[5].px;
    points[2][1].y = q[5].py;
    points[2][2].x = q[6].px;
    points[2][2].y = q[6].py;
    points[2][3].x = q[2].px;
    points[2][3].y = q[2].py;

    points[3][0].x = q[0].px;
    points[3][0].y = q[0].py;
    points[3][1].x = q[4].px;
    points[3][1].y = q[4].py;
    points[3][2].x = q[7].px;
    points[3][2].y = q[7].py;
    points[3][3].x = q[3].px;
    points[3][3].y = q[3].py;

    points[4][0].x = q[4].px;
    points[4][0].y = q[4].py;
    points[4][1].x = q[5].px;
    points[4][1].y = q[5].py;
    points[4][2].x = q[1].px;
    points[4][2].y = q[1].py;
    points[4][3].x = q[0].px;
    points[4][3].y = q[0].py;

    j = 0;
    for (int i = 0; i < PNT; i++)
    {
        points[5][i].x = q[j].px;
        points[5][i].y = q[j].py;
        j++;
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

    struct Oeil o = {250,250,-500};



    projection(o);
    polygones();

    /*Polygones parallelepipede*/
    for (int i = 0; i < POL; i++)
    {
        for (int j = 0; j < PNT; j++)
        {
            if (j == PNT - 1)
                SDL_RenderDrawPoint(renderer, points[i][j].x, points[0][0].y);
            else 
                SDL_RenderDrawPoint(renderer, points[i][j].x, points[i][j].y);

            SDL_RenderPresent(renderer);
        }
    }
    for (int i = 0; i < POL; i++)
    {
        for (int j = 0; j < PNT; j++)
        {
            if (j==PNT-1)
                algoNaif(points[i][j].x, points[i][j].y, points[i][0].x, points[i][0].y);
            else
                algoNaif(points[i][j].x, points[i][j].y, points[i][j+1].x, points[i][j+1].y);
            SDL_RenderPresent(renderer);
        }
    }
    /*Polygones parallelepipede*/
    int a = 255;
    for (int i = 0; i <POL; i++)
    {
        xmin = WIDTH;
        xmax = 0;
        ymin = HEIGHT;
        ymax = 0;
        for (int j = 0; j < PNT; j++){
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
                printf("xmin = %d, xmax = %d, ymin = %d, ymax = %d\n", xmin, xmax, ymin, ymax);
        }
        remplirPolygone(xmin, xmax, i);
        SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 0+a, 255-a, 255/a, a);
        a -= 20;
        SDL_Delay(1000);
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