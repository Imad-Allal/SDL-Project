#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


void dessinerLigne(int x0, int y0, int x1, int y1, SDL_Renderer *renderer)
{
    int x, y, xi, yi, dx, dy, e, i;

    dx = abs(x1-x0);
    dy = abs(y1-y0);
    if(x0<x1)
        xi = 1;
    else
        xi = -1;
    if(y0<y1)
        yi = 1;
    else
        yi = -1;

    x = x0;
    y = y0;
    if(dx>dy)
    {
        e = dx/2;
        for(i=0;i<dx;i++)
        {
            x += xi;
            e += dy;
            if(e>dx)
            {
                e -= dx;
                y += yi;
            }
            SDL_RenderDrawPoint(renderer, x, y);
            SDL_RenderPresent(renderer);
        }
    }
    else
    {
        e = dy/2;
        for(i=0;i<dy;i++)
        {
            y += yi;
            e += dx;
            if(e>dy)
            {
                e -= dy;
                x += xi;
            }
            SDL_RenderDrawPoint(renderer, x, y);
            SDL_RenderPresent(renderer);
        }
    }
    SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderPresent(renderer);
}

int rectangle(int x0,int y0, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Renderer *renderer){
    if (x1 == x0 || y0 != y1 || x2 != x1 || y2 == y1 || x3 != x0 || y3 != y2){
        printf("Ceci n'est pas un rectangle\n");
        return 0;
    }
        dessinerLigne(x0, y0, x1, y1, renderer);
        dessinerLigne(x1, y1, x2, y2, renderer);
        dessinerLigne(x2, y2, x3, y3, renderer);
        dessinerLigne(x3, y3, x0, y0, renderer);
        SDL_RenderPresent(renderer);
        return 1;
}

void cercle(int x0, int y0, int r, SDL_Renderer *renderer)
{
    int d, y, x;
    
    d = 3 - (2 * r);
    x = 0;
    y = r;
    
    while (y >= x) {
        SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
        SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
        SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
        SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
    
        if (d < 0)
        d = d + (4 * x) + 6;
        else {
        d = d + 4 * (x - y) + 10;
        y--;
        }
    
        x++;
    }
}

void Disc(int x1,int y1, int r, SDL_Renderer *renderer)
{
    int x,y,d,stepd,max;
    y=r;
    d= -r;
    stepd= -1;
    max = (int)(r/sqrt(2.0));
    for(x=0;x<=max;x++)
    {  
        stepd += 2;
        d+= stepd;
        if (d>=0) 
        {
            y--;
            d -= (y<<1); 
        }
        dessinerLigne(x1-x,y1+y,x1+x, y1+y, renderer);
        dessinerLigne(x1-x,y1-y,x1+x, y1-y, renderer);        
        dessinerLigne(x1-y,y1+x,x1+y, y1+x, renderer);
        dessinerLigne(x1-y,y1-x,x1+y, y1-x, renderer);        
    }
    
}
