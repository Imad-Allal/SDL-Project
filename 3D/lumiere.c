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

struct Face {
    float x, y, z, cx, cy, cz, nx, ny, nz, vx, vy, vz;
};

struct Oeil
{
    int ox,oy,oz;
};

struct Lumiere{
    int lx, ly, lz;
    float vx, vy, vz, ne, a;
};

struct Prl{
    int px, py, pz;
};

struct Equation{
    int x, y, z, e;
    float tq;
};
struct Vecteur{
    float x, y, z;
};
struct Scalaire{
    float x, y, z;
};

struct Face face[6][4];

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
struct Lumiere cl[POL];
struct Vecteur co[POL];
struct Scalaire b[POL];

struct Oeil o = {100,250,-500};
struct Lumiere l = {200,90,-100};

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
    float i, x, y;
    if (y1 > y0){
        for (i = y0; i <= y1; i++)
        {
            SDL_RenderDrawPoint(renderer, x0, i);
        }
    }
    else{
        for (i = y0; i >= y1; i--)
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
        verticale(x0, y0, y1);
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
        if (face[index][i].x == xmin){
            return i;
        }
    }
}
int bordDroit(int xmax, int index){
    for (int i = 0; i < PNT; i++){
        if (face[index][i].x == xmax){
            return i;
        }
    }
}

SDL_Point listeP1(int min, int max, SDL_Point p[], int taille, int index){
    int i = min;

    for (int j = 0; j < 3; j++){
        p[j].x = face[index][i].x;
        p[j].y = face[index][i].y;
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
        p[j].x = face[index][i].x;
        p[j].y = face[index][i].y;
        if (i == 0)
            i = PNT - 1;
        else
            i--;
    }
    return *p;
}

void remplirPolygone(int xmin, int xmax, int index){
    int y0, y1;
    int x = xmin;
    int i = 0, j = 0;
    int cpt1 = 0, cpt2 = 0;
    int min = bordGauche(xmin, index), max = bordDroit(xmax, index);
    SDL_Point p1[3], p2[3];
    listeP1(min, max, p1, 3, index);
    listeP2(min, max, p2, 3, index);

    while (x <= xmax){
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

        verticale(x, y0, y1);
        SDL_RenderPresent(renderer);
        x++;
    }
}

void polygones(struct Prl p[8]){
    int j = 4;
    for (int i = 0; i < PNT; i++)
    {
        face[0][i].x = p[j].px;
        face[0][i].y = p[j].py;
        face[0][i].z = p[j].pz;
        j++;
    }

    face[1][0].x = p[7].px;
    face[1][0].y = p[7].py;
    face[1][0].z = p[7].pz;
    face[1][1].x = p[6].px;
    face[1][1].y = p[6].py;
    face[1][1].z = p[6].pz;
    face[1][2].x = p[2].px;
    face[1][2].y = p[2].py;
    face[1][2].z = p[2].pz;
    face[1][3].x = p[3].px;
    face[1][3].y = p[3].py;
    face[1][3].z = p[3].pz;

    face[2][0].x = p[1].px;
    face[2][0].y = p[1].py;
    face[2][0].z = p[1].pz;
    face[2][1].x = p[5].px;
    face[2][1].y = p[5].py;
    face[2][1].z = p[5].pz;
    face[2][2].x = p[6].px;
    face[2][2].y = p[6].py;
    face[2][2].z = p[6].pz;
    face[2][3].x = p[2].px;
    face[2][3].y = p[2].py;
    face[2][3].z = p[2].pz;
    
    face[4][0].x = p[4].px;
    face[4][0].y = p[4].py;
    face[4][0].z = p[4].pz;
    face[4][1].x = p[5].px;
    face[4][1].y = p[5].py;
    face[4][1].z = p[5].pz;
    face[4][2].x = p[1].px;
    face[4][2].y = p[1].py;
    face[4][2].z = p[1].pz;
    face[4][3].x = p[0].px;
    face[4][3].y = p[0].py;
    face[4][3].z = p[0].pz;

    face[3][0].x = p[0].px;
    face[3][0].y = p[0].py;
    face[3][0].z = p[0].pz;
    face[3][1].x = p[4].px;
    face[3][1].y = p[4].py;
    face[3][1].z = p[4].pz;
    face[3][2].x = p[7].px;
    face[3][2].y = p[7].py;
    face[3][2].z = p[7].pz;
    face[3][3].x = p[3].px;
    face[3][3].y = p[3].py;
    face[3][3].z = p[3].pz;
    

    j = 0;
    for (int i = 0; i < PNT; i++)
    {
        face[5][i].x = p[j].px;
        face[5][i].y = p[j].py;
        face[5][i].z = p[j].pz;
        j++;
    }
}

void vNormal(){
    struct Face u;
    struct Face v;
    double norme = 0;
    for (int i = 0; i < POL; i++)
    {
        u.x = face[i][0].x - face[i][1].x;
        u.y = face[i][0].y - face[i][1].y;
        u.z = face[i][0].z - face[i][1].z;

        v.x = face[i][1].x - face[i][2].x;
        v.y = face[i][1].y - face[i][2].y;
        v.z = face[i][1].z - face[i][2].z;

        //Produit scalaire U V
        face[i][0].nx = (u.y * v.z) - (u.z * v.y);
        face[i][0].ny = (u.z * v.x) - (u.x * v.z);
        face[i][0].nz = (u.x * v.y) - (u.y * v.x);
        //Produit scalaire

        //Norme du produit vectoriel
        norme = sqrt(face[i][0].nx * face[i][0].nx + face[i][0].ny * face[i][0].ny + face[i][0].nz * face[i][0].nz);

        face[i][0].vx = (face[i][0].nx) / norme;
        face[i][0].vy = (face[i][0].ny) / norme;
        face[i][0].vz = (face[i][0].nz) / norme;
        printf("Face = %d, n'x = %lf, n'y = %lf, n'z = %lf\n", i, face[i][0].vx, face[i][0].vy, face[i][0].vz);
    }
}

void centre(){
    for (int i = 0; i < POL; i++)
    {
        face[i][0].cx = (face[i][0].x + face[i][2].x) / 2; 
        face[i][0].cy = (face[i][0].y + face[i][2].y) / 2;
    }
}

void lumiere(struct Lumiere l){
    for (int i = 0; i < POL; i++)
    {
        co[i].x = o.ox - face[i][0].cx;
        co[i].y = o.oy - face[i][0].cy;
        co[i].z = o.oz - face[i][0].cz;

        b[i].x = face[i][0].vx * co[i].x + face[i][0].vy * co[i].y + face[i][0].vz * co[i].z;
        printf("β = %lf\n", b[i].x);
    }
    b[2].x = -b[2].x;
    b[4].x = -b[4].x;
    b[5].x = -b[5].x;

    for (int i = 0; i < POL; i++) // Intensité lumineuse
    {
        cl[i].lx = l.lx - face[i][0].cx; // CL.x
        cl[i].ly = l.ly - face[i][0].cy; // CL.y
        cl[i].lz = l.lz - face[i][0].cz; // CL.z

        cl[i].ne = sqrt((cl[i].lx * cl[i].lx) + (cl[i].ly * cl[i].ly) + (cl[i].lz * cl[i].lz)); // Norme de CL

        cl[i].vx = cl[i].lx / cl[i].ne; // Le rayon lumineux au point x
        cl[i].vy = cl[i].ly / cl[i].ne; // Le rayon lumineux au point y
        cl[i].vz = cl[i].lz / cl[i].ne; // Le rayon lumineux au point z

        cl[i].a = (cl[i].vx * face[i][0].vx) + (cl[i].vy * face[i][0].vy) + (cl[i].vz * face[i][0].vz); // l'intensité lumineuse α
        printf("α = %lf\n", cl[i].a);
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
    polygones(p1); // Initialiser chaque face avant projection
    vNormal();
    centre(); // calcule du centre de chaque face

    lumiere(l); // calcule du centre de chaque face

    projection(o); // Projection des faces du polygone
    polygones(q); // Attribution des nouvelles coordonnées projetées au polygone

/*Si on fait */

    /*Polygones parallelepipede*/
    for (int i = 0; i < POL; i++)
    {
        for (int j = 0; j < PNT; j++)
        {
            if (j==PNT-1)
            {
                algoNaif(face[i][j].x, face[i][j].y, face[i][0].x, face[i][0].y);
            }
            else{
                algoNaif(face[i][j].x, face[i][j].y, face[i][j+1].x, face[i][j+1].y);
            }
        }
    }
    /*Polygones parallelepipede*/
    for (int i = 0; i < POL; i++)
    {
        xmin = WIDTH;
        xmax = 0;
        ymin = HEIGHT;
        ymax = 0;
        for (int j = 0; j < PNT; j++){
                if (face[i][j].x >= xmax)
                {
                    xmax = face[i][j].x;
                    }
                if (face[i][j].x <= xmin){
                    xmin = face[i][j].x;
                }
                if (face[i][j].y >= ymax){
                    ymax = face[i][j].y;
                }
                if (face[i][j].y <= ymin){
                    ymin = face[i][j].y;
                }
        }
        if (b[i].x > 0)
            SDL_SetRenderDrawColor(renderer, cl[i].a*255, cl[i].a*255, cl[i].a*255, 5);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 5);
        remplirPolygone(xmin, xmax, i);
        SDL_Delay(100);
        SDL_RenderPresent(renderer);
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