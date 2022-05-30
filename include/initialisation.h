#ifndef INITIALISATION_H
#define INITIALISATION_H

#include <SDL2/SDL.h>

int initialisation();
int initWindow(SDL_Window*,char *, int, int);
int initRenderer(SDL_Renderer* ,SDL_Window*);

#endif