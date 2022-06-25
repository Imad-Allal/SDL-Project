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
    double x,y,z,n,a,b;
};

struct Equation{
    int x, y, z, e;
    double tq;
};

struct Cercle boule = {300,1000,400,200};
struct Point l = {100,800,100};
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
    struct PntAdv po[2];
    struct PntAdv q;
    struct PntAdv ql;
    struct PntAdv cq;
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
    c = (o.x - boule.cx) * (o.x - boule.cx) + (o.y - boule.cy) * (o.y - boule.cy) + (o.z - boule.cz) * (o.z - boule.cz) - ((boule.r * boule.r));

    rac = sqrt((b * b) - (4 * a * c));
    //printf("rac : %lf,%lf\n", rac, b * b - 4 * a * c);
    t0 = (-b + rac) / (2 * a);
    t1 = (-b - rac) / (2 * a);
    //printf("t0 = %f, t1 = %f\n", t0, t1);

    p[0].x = t0 * e.x + o.x;
    p[0].y = t0 * e.y + o.y;
    p[0].z = t0 * e.z + o.z;

    p[1].x = t1 * e.x + o.x;
    p[1].y = t1 * e.y + o.y;
    p[1].z = t1 * e.z + o.z;

    po[0].x = o.x - p[0].x;
    po[0].y = o.y - p[0].y;
    po[0].z = o.z - p[0].z;

    po[1].x = o.x - p[1].x;
    po[1].y = o.y - p[1].y;
    po[1].z = o.z - p[1].z;

    cp[0].x = p[0].x - boule.cx;
    cp[0].y = p[0].y - boule.cy;
    cp[0].z = p[0].z - boule.cz;

    cp[1].x = p[1].x - boule.cx;
    cp[1].y = p[1].y - boule.cy;
    cp[1].z = p[1].z - boule.cz;

    p[0].b = (po[0].x * cp[0].x) + (po[0].y * cp[0].y) + (po[0].z * cp[0].z);
    p[1].b = (po[1].x * cp[1].x) + (po[1].y * cp[1].y) + (po[1].z * cp[1].z);

    if (p[0].b>0)
        q = p[0];
    else
        q = p[1];

    ql.x = l.x - q.x;
    ql.y = l.y - q.y;
    ql.z = l.z - q.z;

    cq.x = q.x - boule.cx;
    cq.y = q.y - boule.cy;
    cq.z = q.y - boule.cz;

    ql.n = sqrt((ql.x * ql.x) + (ql.y * ql.y) + (ql.z * ql.z)); // La norme du vecteur QL
    cq.n = sqrt((cq.x * cq.x) + (cq.y * cq.y) + (cq.z * cq.z));

    //printf("p.x = %f, p.y = %f, p.z = %f, p.n = %f\n", p[0].x, p[0].y, p[0].z);

    q.a = (ql.x * cq.x + ql.y * cq.y + ql.z * cq.z) / (ql.n * cq.n);

    //printf("cp.x = %f, cp.y = %f, cp.z = %f, cp.n = %f\n", cp[0].x, cp[0].y, cp[0].z, cp[0].n);

    SDL_SetRenderDrawColor(renderer, q.a*255, q.a*255, q.a*255, 255);
    SDL_RenderDrawPoint(renderer, px, py);
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
    oc2 = sqrt((cercle.cx - o.x) * (cercle.cx - o.x) + (cercle.cy - o.y) * (cercle.cy - o.y));

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
    //disque(cercle); // Dessin de disque noire (avant raytracing)
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
