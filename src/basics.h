/*
* Basic functions (Loading optimized images, fast blitting)
* Project: Aerial
* Developer: Aer
*/
#ifndef _BASICS_H_
#define _BASICS_H_

// INCLUDE
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//PROTOTYPE
SDL_Surface *load_image(char *filename);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );


#endif
