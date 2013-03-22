/*
* Main function
* Project: Aerial
* Developer: Aer
*/
#ifndef _MAIN_H_
#define _MAIN_H_

// INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <time.h>

#include "basics.h"
#include "inputs.h"


// MACROS
#define WIDTH 1600
#define HEIGHT 900
#define BPP 32
#define NUM 15

// TYPE

typedef struct object object;
struct object
{
  double x;
  double y;
  double vX;
  double vY;
  double e;
  double w;
  int r;
  SDL_Surface *img;
};

#endif
