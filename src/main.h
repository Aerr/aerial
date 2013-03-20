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
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <time.h>

#include "basics.h"
#include "inputs.h"


// MACROS
#define WIDTH 640
#define HEIGHT 480
#define BPP 32

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
  SDL_Surface *img;
};

typedef struct vector2 Vector2;
struct vector2
{
  double X;
  double Y;
};

#endif
