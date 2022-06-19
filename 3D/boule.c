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

struct Equation{
    int x, y, z, e;
    double tq;
};

struct Cercle boule = {300,1000,400,200};
struct Point l = {100,100,300};
struct Point o = {250,250,-500};
struct Equation eq[N];
struct Cercle cercle;
struct PntAdv p[2];



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
void verticale (double x0, double y0, double y1){
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
        verticale(x0, y0, y1); // Trace une ligne verticale
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

    if (SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255) != 0) {    
        fprintf(stderr,"Color error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    projet(); // Projection du centre de la sphere
    rayon(); // Calcul du rayon du cercle
    disque(cercle); // Dessin de disque noire (avant raytracing)
    point(cercle); // Calcul la luminausité de chaque point du cercle de centre C', rayon R'
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
