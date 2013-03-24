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
#include <math.h>

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

//PROTOTYPE
SDL_Surface *load_image(char *filename);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
double random(double a, double b);
double inv_mass(double w);
//Returns the distance between two objects
double distance(object sqA, object sqB);
double distancePos(double aX, double aY, double bX, double bY, double aR, double bR);
// MACROS
#define WIDTH 1280
#define HEIGHT 720
#define BPP 32
#define MAX_NUM 300

#endif
