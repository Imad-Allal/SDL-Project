#ifndef DESSIN_H
#define DESSIN_H

#include <SDL2/SDL.h>

void dessinerLigne(int, int, int, int, SDL_Renderer*);
void rectangle(int, int, int, int, int, int, int, int, SDL_Renderer*);
void remplirRct(int, int, int, int, SDL_Renderer*);
void cercle(int, int, int, SDL_Renderer *);
void cercle(int, int, int, SDL_Renderer *);

#endif