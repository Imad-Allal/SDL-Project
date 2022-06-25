#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define N 8
#define POL 6
#define PNT 4
#define WIDTH 1500
#define HEIGHT 1500

SDL_Window *window;
SDL_Renderer *renderer;

struct Vecteur{
    int x,y, norme;
};

SDL_Point ball = {900, 900};
SDL_Point p_droite;
SDL_Point tr[2][3] = {
    {{0, 0},
     {750, 0},
     {0, 450}},
    {{1500, 0},
     {750, 0},
     {1500, 450}},
};

struct Vecteur u[2];
SDL_Point palette[4] = {{600,1370},
                        {900, 1370},
                        {900,1400}, 
                        {600,1400}
                        };

int xmin, xmax, ymin, ymax;
int x, y;
int ux =10, uy = 7; //A modifier: ux = 1
int r = 22;

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

void rectangle(){
    int xmin = WIDTH, xmax = 0, ymin = HEIGHT, ymax = 0;
    for (int i = 0; i < 4; i++)
    {
        if (palette[i].x >= xmax){
            xmax = palette[i].x;
        }
        if (palette[i].x <= xmin){
            xmin = palette[i].x;
        }
        if (palette[i].y >= ymax){
            ymax = palette[i].y;
        }
        if (palette[i].y <= ymin){
            ymin = palette[i].y;
        }

    }
    for (int i = ymin; i <= ymax; i++){
        horizontale(xmin, i, xmax - xmin);
    }
}

void triangle(int x0, int x1, int y0, int y1){
    float i, x, y, m;
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    if (dx > 0){
        float m = ((float)dy) / ((float)dx);
        for (i = 0; i < dx; i++){
            x = x0 + i;
            y = Round(y0 + m * i);
            SDL_RenderDrawPoint(renderer, x, y);
            verticale(x, y0, y);
        }
    }
    else{
        float m = ((float)dy) / ((float)dx);
        for (i = 0; i > dx; i--){
            x = x0 + i;
            y = Round(y0 + m * i);
            SDL_RenderDrawPoint(renderer, x, y);
            verticale(x, y0, y);
        }
    }
}

void droiteG(int x0, int y0, int x1, int y1, int i){
    float x, y, m;
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    m = ((float)dy) / ((float)dx);
    p_droite.x = x0 + i;
    p_droite.y = Round(y0 + m * i);
}

void droiteD(int x0, int y0, int x1, int y1, int i){
    float x, y, m;
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    m = ((float)dy) / ((float)dx);
    p_droite.x = x0 + i;
    p_droite.y = Round(y0 + m * i);
}

void disque(int cx, int cy){
    int x0 = cx - r;
    int x1 = cx + r;
    int x = x1;
    float y0, y1;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = cy + sqrt((r * r) - (x0 - cx) * (x0 - cx));
        y1 = cy - sqrt((r * r) - (x0 - cx)  *(x0 - cx));

        verticale(x0, y0, y1);
        x0++;
        x--;
    }
}

void all(){
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 3; j++){
            if (j == 2)
            {
                triangle(tr[i][j].x, tr[i][0].x, tr[i][j].y, tr[i][0].y);
            }
            else{
                triangle(tr[i][j].x, tr[i][j+1].x, tr[i][j].y, tr[i][j+1].y);
            }
        }
    }
    
    algoNaif(0, 400, 750, 0);
    algoNaif(1500, 400, 750, 0);
    disque(x, y);
    rectangle();
}

int toitG(int xc, int yc){
    SDL_Point t;
    int x0 = xc - r;
    int x1 = xc + r;
    int x = x1;
    float y0;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = ball.y + sqrt((r * r) - (x0 - ball.x) * (x0 - ball.x));
        t.x = x0;
        t.y = yc;
        for (int i = 0; i < 760; i++)
        {
            droiteG(0, 430, 750, 30,i);
            if (t.x == p_droite.x && t.y == p_droite.y)
            {
                return 1;
            }
        }
        x0++;
        x--;
    }
    return 0;
}

int toitD(int xc, int yc){
    SDL_Point t;
    int x0 = xc - r;
    int x1 = xc + r;
    int x = x1;
    float y0;
    int dx;

    while(x0 <= x1){
        dx = x - x0;
        y0 = ball.y + sqrt((r * r) - (x0 - ball.x) * (x0 - ball.x));
        t.x = x0;
        t.y = yc;
        for (int j = 0; j > -750; j--){
            droiteD(1500, 430, 750, 30, j);
            if (t.x == p_droite.x && t.y == p_droite.y){
                return 1;
            }
        }
        x0++;
        x--;
    }
    return 0;
}

int mouvement(){

    int stop = 0;
    int x_precedent, y_precedent;
    x_precedent = x;
    y_precedent = y;
    int a, b;
    float alpha;
/*    
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
                    case SDLK_c:
                        if(!mouvement())
                            quitter = SDL_TRUE;
                        break;
*/                        
    while (stop < 3)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        xmin = x - r;
        ymin = y - r;
        xmax = x + r;
        ymax = y + r;

        /*if (ymax >= HEIGHT+r*2){
            quit();
            return EXIT_SUCCESS;
        }*/
        if (ymax >= HEIGHT){
            uy = -uy;
            stop++;
        }
            if (ymin <= 10)
                uy = -uy;

            if (y <= 400)
            {
                if(x<=750){
                    if (toitG(x, y))
                    { // Droite Gauche
                        if (ux != 0)
                        {
                            if (y_precedent > y && uy != 0)
                            {
                                if (x_precedent>x){
                                    ux = -ux;
                                    uy = -uy;
                                }
                                else if (x_precedent<x)
                                    ux = -20;{
                                }
                            }
                            else if (y_precedent < y && uy != 0)
                            {
                                ux = 4;
                            }
                            else if (y_precedent == y && uy != 0){
                                uy = -3;
                                ux = 0;
                            }
                            else if (uy == 0) {
                                uy = -3;
                                ux = 0;
                            }
                        }
                        else{
                            ux = -3;
                            uy = 0;
                        }
                    }
                }
                else if(x>=750){
                    if(toitD(x,y))
                    { // Droite Droite
                        if(ux != 0){
                            if (y_precedent > y && uy !=0){
                                if (x_precedent > x){
                                    ux = 20;
                                }
                                else if (x_precedent < x){
                                    ux = -ux;
                                    uy = -uy;
                                    printf("y_pre = %d,, y = %d\n",y_precedent, y);
                                }
                            }
                            else if (y_precedent < y && uy !=0){
                                ux = -1;
                            }
                            else if (y_precedent < y && uy != 0){
                                uy = -3;
                                ux = 0;
                            }
                            else if (uy == 0) {
                                uy = -3;
                                ux = 0;
                            }
                        }
                        else{
                            ux = 3;
                            uy = 0;
                        }
                    }
                }
        }
        if (xmax >= WIDTH){
            ux = -ux;
            x_precedent = x;
            y_precedent = y;
            stop++;
        }  

        if (xmin <= 0)
        {
            ux = -ux;
            x_precedent = x;
            y_precedent = y;
            stop++;
        }        
        x = x - ux;
        y = y - uy;

        all();
        if (ux == -4 || ux == 4)
            SDL_Delay(2);
        if (ux == -8 || ux == 8)
            SDL_Delay(6);
        SDL_RenderPresent(renderer);
    }
}


int main(int argc, char *argv[])
{

    int r = 35;
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

    //projection(o); // Projection des faces du polygone
    //polygones(q); // Attribution des nouvelles coordonnées projetées au polygone
    x = ball.x;
    y = ball.y;

    all();
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
                    case SDLK_c:
                        if(!mouvement())
                            quitter = SDL_TRUE;
                        break;
                    default:
                        printf("Vous avez appuyé sur la touche : %c\n", event.key.keysym.sym);
                        continue;
                    }
                    break;
                case SDL_QUIT: quitter = SDL_TRUE;
                    quit();
                    return EXIT_SUCCESS;
                    break;
                }
            }
    }

    quit();
    return EXIT_SUCCESS;
}