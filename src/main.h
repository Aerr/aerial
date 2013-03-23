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
#define WIDTH 400
#define HEIGHT 600
#define BPP 32
#define NUM 12

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
