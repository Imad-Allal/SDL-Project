#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 8
#define POL 5
#define PNT 4
#define WIDTH 1500
#define HEIGHT 1000

SDL_Window *window;
SDL_Renderer *renderer;

struct Face {
    int x, y, z, cx, cy, cz, nx, ny, nz;
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

struct Cercle{
    double cx, cy, cz, r;
};

struct Point
{
    double x,y,z;
};

struct PntAdv
{
    double x,y,z,n,a;
};

struct Cercle boule = {300,1000,400,200};
struct Point l = {100,100,300};
struct Point o = {250,250,-500};
struct Equation eq[N];
struct Cercle cercle;
struct PntAdv p[2];

struct Face face[6][4];
struct Equation eq[N];
struct Prl q[N];
struct Lumiere cl[N];

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
        x++;
    }
}

void polygones(){

    face[0][0].x = 0;
    face[0][0].y = 0;
    face[0][1].x = 1500;
    face[0][1].y = 0;
    face[0][2].x = 1500;
    face[0][2].y = 1000;
    face[0][3].x = 0;
    face[0][3].y = 1000;

    face[1][0].x = 0;
    face[1][0].y = 0;
    face[1][1].x = 1500;
    face[1][1].y = 0;
    face[1][2].x = 1000;
    face[1][2].y = 400;
    face[1][3].x = 500;
    face[1][3].y = 400;

    face[2][0].x = 0;
    face[2][0].y = 0;
    face[2][1].x = 500;
    face[2][1].y = 400;
    face[2][2].x = 500;
    face[2][2].y = 600;
    face[2][3].x = 0;
    face[2][3].y = 1000;
    
    face[3][0].x = 0;
    face[3][0].y = 1000;
    face[3][1].x = 500;
    face[3][1].y = 600;
    face[3][2].x = 1000;
    face[3][2].y = 600;
    face[3][3].x = 1500;
    face[3][3].y = 1000;

    face[4][0].x = 1000;
    face[4][0].y = 400;
    face[4][1].x = 1500;
    face[4][1].y = 0;
    face[4][2].x = 1500;
    face[4][2].y = 1000;
    face[4][3].x = 1000;
    face[4][3].y = 600;
}

void vNormal(){
    struct Face u;
    struct Face v;
    for (int i = 0; i < POL; i++){
        u.x = face[i][0].x - face[i][3].x;
        u.y = face[i][0].y - face[i][3].y;
        u.z = face[i][0].z - face[i][3].z;

        v.x = face[i][2].x - face[i][3].x;
        v.y = face[i][2].y - face[i][3].y;
        v.z = face[i][2].z - face[i][3].z;

        face[i][0].nx = (u.y * v.z) - (u.z * v.y);
        face[i][0].ny = (u.z * v.x) - (u.x * v.z);
        face[i][0].nz = (u.x * v.y) - (u.y * v.x);
        if (face[i][0].nx > 0)
            face[i][0].nx = 1;
        else if (face[i][0].nx < 0)
            face[i][0].nx = -1;

        if (face[i][0].ny > 0)
            face[i][0].ny = 1;
        else if (face[i][0].ny < 0)
            face[i][0].ny = -1;

        if (face[i][0].nz > 0)
            face[i][0].nz = 1;
        else if (face[i][0].nz < 0)
            face[i][0].nz -1;

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
        cl[i].lx = l.lx - face[i][0].cx; // CL.x
        cl[i].ly = l.ly - face[i][0].cy; // CL.y
        cl[i].lz = l.lz - face[i][0].cz; // CL.z

        cl[i].ne = sqrt((cl[i].lx*cl[i].lx) + (cl[i].ly*cl[i].ly) + (cl[i].lz*cl[i].lz)); // Norme de CL 

        cl[i].vx = cl[i].lx / cl[i].ne; // Le rayon lumineux au point x
        cl[i].vy = cl[i].ly / cl[i].ne; // Le rayon lumineux au point y
        cl[i].vz = cl[i].lz / cl[i].ne; // Le rayon lumineux au point z

        cl[i].a = (cl[i].vx * face[i][0].nx) + (cl[i].vy * face[i][0].ny) + (cl[i].vz * face[i][0].nz); // l'intensité lumineuse α
        printf("α = %lf\n", cl[i].a);
    }
}

void projet(){ // Porjette la sphere 3D en un disque 2D noire
    
    for (int i = 0; i < 2; i++)
    {
        eq[i].x = boule.cx - o.x;
        eq[i].y = boule.cy - o.y;
        eq[i].z = boule.cz - o.z;

        eq[i].tq = (double)-o.z/(double)eq[i].z;

        cercle.cx = eq[i].tq * eq[i].x + o.x;
        cercle.cy = eq[i].tq * eq[i].y + o.y;
    }
}
int intersection(double px, double py, double pz){ //L'intersection de OP avec la boule
    double a, b, c, rac, t0, t1;
    struct PntAdv cp[2];
    struct PntAdv p[2];
    struct PntAdv pl;
    struct Point e;
    
    //Calcul de vecteur OP
    e.x = px - o.x;
    e.y = py - o.y;
    e.z = 0 - o.z;
    //printf("e : %f ,%f ,%f\n", e.x, e.y, e.z);
    // Calcul de vecteur OP

    //Calcul des valeurs de a, b ,c
    a = e.x*e.x + e.y*e.y + e.z*e.z;
    b = 2 * e.x * (o.x - boule.cx) + 2 * e.y * (o.y - boule.cy) + 2 * e.z * (o.z - boule.cz);
    c = (o.x - boule.cx) * (o.x - boule.cx) + (o.y - boule.cy) * (o.y - boule.cy) + (o.z - boule.cz) * (o.z - boule.cz) - (boule.r * boule.r);
    //printf("a : %f, b : %f, c : %f\n", a, b, c);

    rac = sqrt(b * b - 4 * a * c);
    //printf("rac : %f\n", rac);
    t0 = (-b + rac) / (2 * a);
    t1 = (-b-rac)/(2 * a);
    //printf("t0 = %f, t1 = %f\n", t0, t1);

    p[0].x = t0 * e.x + o.x;
    p[0].y = t0 * e.y + o.y;
    p[0].z = t0 * e.z + o.z;

    p[1].x = t1 * e.x + o.x;
    p[1].y = t1 * e.y + o.y;
    p[1].z = t1 * e.z + o.z;

    pl.x = l.x - px;
    pl.y = l.y - py;
    pl.z = l.z - pz;

    pl.n = sqrt((pl.x * pl.x) + (pl.y * pl.y) + (pl.z * pl.z));

    //printf("p.x = %f, p.y = %f, p.z = %f, p.n = %f\n", p[0].x, p[0].y, p[0].z);

    cp[0].x = p[0].x - boule.cx;
    cp[0].y = p[0].y - boule.cy;
    cp[0].z = p[0].z - boule.cz;

    cp[1].x = p[1].x - boule.cx;
    cp[1].y = p[1].y - boule.cy;
    cp[1].z = p[1].z - boule.cz;

    cp[0].n = sqrt((cp[0].x * cp[0].x) + (cp[0].y * cp[0].y) + (cp[0].z * cp[0].z));
    cp[1].n = sqrt((cp[1].x * cp[1].x) + (cp[1].y * cp[1].y) + (cp[1].z * cp[1].z));

    p[0].a = ((pl.x * cp[0].x) + (pl.y * cp[0].y) + (pl.z * cp[0].z)) / (pl.n * cp[0].n);
    p[1].a = ((pl.x * cp[1].x) + (pl.y * cp[1].y) + (pl.z * cp[1].z)) / (pl.n * cp[1].n);
    //printf("cp.x = %f, cp.y = %f, cp.z = %f, cp.n = %f\n", cp[0].x, cp[0].y, cp[0].z, cp[0].n);

    if (p[0].a > 0){
        SDL_SetRenderDrawColor(renderer, p[0].a*255, p[0].a*255, p[0].a*255, 255);
        SDL_RenderDrawPoint(renderer, px, py);
    }
    if (p[1].a > 0){
        SDL_SetRenderDrawColor(renderer, p[1].a*255, p[1].a*255, p[1].a*255, 255);
        SDL_RenderDrawPoint(renderer, px, py);
    }

    //printf("a1 = %lf, a2 = %lf\n", p[0].a, p[1].a);
}

void ombre (double x0, double y0, double y1, int z){
    int i, x, y;
    int dy = y1 - y0;
    if (dy > 0)
    {
        for (i = y0; i <= y1; i++)
        {
            intersection(x0, i, z); // Calcule l'intersection de chaque point (x, y) du cercle
        }
    }
    else{
        y = y0 + dy;
        for (i = y0; i >= y1; i--)
        {
            intersection(x0, i, z);
        }
    }    
}
void verticale2 (double x0, double y0, double y1){
    //printf("VERTICAAAALE\n");
    int i, x, y;
    int dy = y1 - y0;
    if (dy > 0)
    {
        for (i = y0; i <= y1; i++)
        {
            SDL_RenderDrawPoint(renderer,x0, Round(i));
        }
    }
    else{
        y = y0 + dy;
        for (i = y0; i >= y1; i--)
        {
            SDL_RenderDrawPoint(renderer,x0, Round(i));
        }
    }    
}

void rayon(){
    double oc1, oc2;
    oc1 = sqrt((boule.cx - o.x) * (boule.cx - o.x) + (boule.cy - o.y) * (boule.cy - o.y) + (boule.cz - o.z) * (boule.cz - o.z));
    oc2 = sqrt((boule.cx - o.x) * (boule.cx - o.x) + (boule.cy - o.y) * (boule.cy - o.y));

    cercle.r = boule.r * oc2 / oc1;
}

void disque(struct Cercle sphere){
    double x0 = sphere.cx - sphere.r;
    double x1 = sphere.cx + sphere.r;
    double y0, y1;
    double x = x1;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = sphere.cy + sqrt((sphere.r * sphere.r) - (x0 - sphere.cx) * (x0 - sphere.cx));
        y1 = sphere.cy - sqrt((sphere.r * sphere.r) - (x0 - sphere.cx) * (x0 - sphere.cx));
        verticale2(x0, y0, y1); // Trace une ligne verticale
        x0++;
        x--;
    }
}
void point(struct Cercle sphere){
    int x0 = sphere.cx - sphere.r;
    int x1 = sphere.cx + sphere.r;
    double y0, y1;

    while(x0 <= x1){
        y0 = sphere.cy + sqrt((sphere.r * sphere.r) - (x0 - sphere.cx) * (x0 - sphere.cx));
        y1 = sphere.cy - sqrt((sphere.r * sphere.r) - (x0 - sphere.cx) * (x0 - sphere.cx));
        ombre(x0, y0, y1, boule.cz); // Prend les 2 extrémités (hautes et basses) de chaque point x du cercle, et calcul l'ombre de tous les points entre y0 et y1
        x0++;
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

    struct Oeil o = {50,250,-1000};
    struct Lumiere l = {20,50,100};

    polygones(); // Initialiser chaque face avant projection
    vNormal();
    centre(); // calcule du centre de chaque face
    lumiere(l); // calcule du centre de chaque face

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
    for (int i = 0; i <POL; i++)
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
        if (cl[i].a > 0)
            SDL_SetRenderDrawColor(renderer, cl[i].a*255, cl[i].a*255, cl[i].a*255, 255);
        else
            SDL_SetRenderDrawColor(renderer, cl[i].a*255, cl[i].a*255, cl[i].a*255, 255);
        remplirPolygone(xmin, xmax, i);
        SDL_RenderPresent(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    projet(); // Projection du centre de la sphere
    rayon(); // Calcul du rayon du cercle
    disque(cercle); // Dessin de disque noire (avant raytracing)
    point(cercle); // Calcul la luminausité de chaque point du cercle de centre C', rayon R'
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