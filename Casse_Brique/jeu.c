#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define SIZE 30
#define WIDTH 1500
#define HEIGHT 1500

SDL_Window *window;
SDL_Renderer *renderer;

struct Rectangle{
    int xmin, xmax, ymin, ymax, cx, cy;
};

SDL_Point ball = {305, 1305};
SDL_Point p_droite;
SDL_Point tr[2][3] = {
    {{0, 0},
     {750, 0},
     {0, 450}},
    {{1500, 0},
     {750, 0},
     {1500, 450}},
};

struct Rectangle rec[SIZE];
struct Rectangle palette = {600, 900, 1370, 1400, 750,1885};

const Uint8 *clavier;

int supprimerRec[SIZE];
int xmin, xmax, ymin, ymax;
int ux =5, uy = 5; // Vecteurs de translation
int taille = 0; // taille du buffer contenant les blocs touchés
int r = 25; // Rayon du disque
int j = 0;
int x, y;

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

void algoNaif(int x0, int y0, int x1, int y1){ //Tracage de lignes
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
            }
        }
        else{
            float m = ((float)dy) / ((float)dx);
            for (i = 0; i > dx; i--){
                x = x0 + i;
                y = Round(y0 + m * i);
            }
        }
    }
}

void pal(){ // Fonction pour palette
    for (int i = palette.ymin; i <= palette.ymax; i++){
        horizontale(palette.xmin, i, palette.xmax - palette.xmin);
    }
}

void triangle(int x0, int x1, int y0, int y1){ // Triangles verts en haut
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

void disque(int cx, int cy){ // Tracage de disque (balle)
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

void remplirRec(int x0, int x1, int y0, int y1){ // Remplissage de rectangles
    int y = y0;
    int dx = x1 - x0;
    while (y < y1)
    {
        horizontale(x0, y, dx);
        y++;
    }
}

void commutation(int *a, int *b) 
{ 
int tmp = *a; 
*a = *b; 
*b = tmp; 
}   
// Trie de tableau par selection
void trierTab(int tab[], int n) 
{ 
    int i, j, min; 
    for (i = 0; i < n-1; i++) 
    {
    min = i; 
    for (j = i+1; j < n; j++) 
    if (tab[j] < tab[min]) 
    min = j; 
    commutation(&tab[min], &tab[i]); 
    } 
}

void rectangle(){ // Tracage de rectangles
    int cx = 50, cy = 700;
    int longeur = 100, largeur = 50;
    int cpt = 0, key = 0;

    if (taille == 0) // Liste de rectangles à supprimer vide ==> tracer tous les rectangles 
    {
        for (int i = 0; i < 30; i++)
        {
            if (i == 15)
            {
                cx = 50;
                cy = 650;
            }
            rec[i].cx = cx;
            rec[i].cy = cy;
            rec[i].xmin = rec[i].cx - longeur / 2;
            rec[i].xmax = rec[i].cx + longeur / 2;
            rec[i].ymin = rec[i].cy - largeur / 2;
            rec[i].ymax = rec[i].cy + largeur / 2;
            SDL_SetRenderDrawColor(renderer, 100+i, i * 20, 100-i, 255);
            remplirRec(rec[i].xmin, rec[i].xmax, rec[i].ymin, rec[i].ymax);
            cx += 100;
        }
    }
    else{
        if (taille > 1) // Ne pas tracer les rectangles contenus dans la liste
            trierTab(supprimerRec, taille);
        for (int i = 0; i < 30; i++)
        {
            key = 0;
            while(cpt<taille){
                if (i == supprimerRec[cpt]){
                    cx += 100;
                    key = 1;
                    cpt++;
                    break;
                }
                else
                    break;
            }

            if (key){
                continue;
            }

            if (i == 15) // Chaque ligne contient 15 rectangles, si on est alors dans le 15eme, on monte d'une ligne
            {
                cx = 50;
                cy = 650;
            }
            rec[i].cx = cx;
            rec[i].cy = cy;
            rec[i].xmin = rec[i].cx - longeur / 2;
            rec[i].xmax = rec[i].cx + longeur / 2;
            rec[i].ymin = rec[i].cy - largeur / 2;
            rec[i].ymax = rec[i].cy + largeur / 2;
            SDL_SetRenderDrawColor(renderer, 100+i, i * 20, 100-i, 255);
            remplirRec(rec[i].xmin, rec[i].xmax, rec[i].ymin, rec[i].ymax);
            cx += 100;
        }     
    }       
}
//// Les deux fonctions en dessous permettent de récuperer chaque point (x,y) des droites limitantes en haut à l'aide de l'algo naïf ////
void droiteG(int x0, int y0, int x1, int y1, int i){ // Limite du terrain (haut à gauche)
    float x, y, m;
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    m = ((float)dy) / ((float)dx);
    p_droite.x = x0 + i;
    p_droite.y = Round(y0 + m * i);
}

void droiteD(int x0, int y0, int x1, int y1, int i){ // Limite du terrain (haut à droite)
    float x, y, m;
    int dx = x1 - x0;
    int dy = y1 - y0;
    
    m = ((float)dy) / ((float)dx);
    p_droite.x = x0 + i;
    p_droite.y = Round(y0 + m * i);
}
//// Les deux fonctions au dessus permettent de récuperer chaque point (x,y) des droites limitantes en haut à l'aide de l'algo naïf ////

//// Les deux fonctions en dessous permettent de récuperer chaque point (x,y) de la moitié haute de notre disque et les comparer avec les point (x,y) des droites limitantes en haut ////
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
        for (int i = 0; i < 750; i++)
        {
            droiteG(0, 425, 750, 25,i);
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
            droiteD(1500, 425, 750, 25, j);
            if (t.x == p_droite.x && t.y == p_droite.y){
                return 1;
            }
        }
        x0++;
        x--;
    }
    return 0;
}
//// Les deux fonctions au dessus permettent de récuperer chaque point (x,y) de la moitié haute de notre disque et les comparer avec les point (x,y) des droites limitantes en haut ////

void rendu(){ // Crée, mis à jour et affiche notre terrain de jeu
    SDL_SetRenderDrawColor(renderer,105,255,105,255);
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
    SDL_SetRenderDrawColor(renderer,255,255,255,255);  
    algoNaif(0, 400, 750, 0);
    algoNaif(1500, 400, 750, 0);
    disque(x, y);
    memset(rec, 0, sizeof(rec));
    rectangle();
    pal();
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
}

void bougerD(){ // Deplacer la palette à droite
    if (palette.xmax < WIDTH){
            palette.xmin += 10;
            palette.xmax += 10;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            rendu();
            SDL_RenderPresent(renderer);
    }
}
void bougerG(){ // Deplacer la palette à gauche
        if (palette.xmin > 0){
            palette.xmin -= 10;
            palette.xmax -= 10;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            rendu();
            SDL_RenderPresent(renderer);
        }
}

int mouvement(){ // Animation du disque dans le terrain

    int i = 0;
    int stop = 0;
    int x_precedent, y_precedent;
    x_precedent = x;
    y_precedent = y;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    xmin = x - r;
    ymin = y - r;
    xmax = x + r;
    ymax = y + r;

    if (ymax >= HEIGHT+r*2){ // Balle sort du terrain en bas
        return EXIT_FAILURE;
    }
    for (i = 0; i < 30; i++)
    {
        if ((ymin == rec[i].ymax || ymax == rec[i].ymin) && x >= rec[i].xmin && x <= rec[i].xmax) // Ball tape en haut ou en bas des rectangles
        {
            uy = -uy;
            stop++;
            supprimerRec[j] = i; // Sauvegarder les blocks touchés dans le buffer
            j++;
            taille++;
            break;
        }
        else if ((xmin == rec[i].xmax || xmax == rec[i].xmin) && y >= rec[i].ymin && y <= rec[i].ymax) // Ball tape à gauche ou à droite des rectangles 
        {
            ux = -ux;
            stop++;
            supprimerRec[j] = i;
            j++;
            taille++;
            break;
        }
    }
    if (ymin <= 20) // Limite en haut
        uy = -uy;

    if (y <= 400) // cette condition permet d'eviter de faire des calcules inutiles dans les zones ou cy > 400 (400 = debut des droites limitants: 400 -> 0)
    {
        if(x<=750){ // Droite haute gauche
            if (toitG(x, y))
            { // Calcul de collision sur la Droite Gauche
                if (ux != 0)
                {
                    if (y_precedent > y && uy != 0)
                    {
                        if (x_precedent>x){
                            ux = -ux;
                            uy = -uy;
                        }
                        else if (x_precedent<x)
                            ux = -5;{
                        }
                    }
                    else if (y_precedent < y && uy != 0)
                    {
                        ux = -5;
                    }
                    else if (y_precedent == y && uy != 0){
                        uy = -5;
                        ux = 0;
                    }
                    else if (uy == 0) {
                        uy = -5;
                        ux = 0;
                    }
                }
                else{
                    ux = -5;
                    uy = 0;
                }
            }
        }
        else if(x>=750){ // Droite haute droite
            if(toitD(x,y))
            { // Calcul de collision sur la Droite Droite
                if(ux != 0){
                    if (y_precedent > y && uy !=0){
                        if (x_precedent > x){
                            ux = 5;
                        }
                        else if (x_precedent < x){
                            ux = -ux;
                            uy = -uy;
                        }
                    }
                    else if (y_precedent < y && uy !=0){
                        ux = -5;
                    }
                    else if (y_precedent < y && uy != 0){
                        uy = -5;
                        ux = 0;
                    }
                    else if (uy == 0) {
                        uy = -5;
                        ux = 0;
                    }
                }
                else{
                    ux = 5;
                    uy = 0;
                }
            }
        }
    }
    if (xmax >= WIDTH){ // Limite à gauche
        ux = -ux;
        x_precedent = x;
        y_precedent = y;
        stop++;
    }

    if (xmin <= 0) // Limite à droite
    {
        ux = -ux;
        x_precedent = x;
        y_precedent = y;
        stop++;
    }        

    if ((ymax == palette.ymin && x>=palette.xmin && x <= palette.xmax) || (xmax == palette.xmin && y>=palette.ymin && x <= palette.ymax) || (xmin == palette.xmin && y>=palette.ymin && y <= palette.ymax)){ // Collision de la balle avec la palette
        if (x<palette.cx){
            ux -= -5;
        }
        else if (x>palette.cx){
            ux += -5;
        }
        else
            ux = 0;
        uy = -uy;
    }

    x = x - ux ;
    y = y - uy ;
    rendu();

    SDL_RenderPresent(renderer);


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


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        fprintf(stderr,"Renderer error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    if (SDL_SetRenderDrawColor(renderer, 5, 5, 0, 255) != 0) {    
        fprintf(stderr,"Color error %s", SDL_GetError());
        quit();
        return EXIT_FAILURE;
    }

    clavier = SDL_GetKeyboardState(NULL);

    SDL_RenderClear(renderer);
    // projection(o);
    // sphere();
    // projection(o);
    // polygones();
    x = ball.x;
    y = ball.y;
    int supprimerRec[SIZE];

    rendu();

    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_bool quitter = SDL_FALSE;

    while(!quitter){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_a)
                quitter = SDL_TRUE;
        }
        if(mouvement())
            break;
        if (clavier[SDL_SCANCODE_RIGHT]){
            bougerD();
        }
        if (clavier[SDL_SCANCODE_LEFT]){
            bougerG();
        }
    }
    quit();
    return EXIT_SUCCESS;
}