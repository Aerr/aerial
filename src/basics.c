/*
* Basic functions (Loading optimized images, fast blitting)
* Project: Aerial
* Developer: Aer
*/

#include "basics.h"

SDL_Surface *load_image(char *filename) {

  IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load(filename);

  if(loadedImage != NULL)
  {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
  }
  else
      printf("Can't load the image : %s\n", SDL_GetError());

  return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
  SDL_Rect offset;

  offset.x = x;
  offset.y = y;

  SDL_BlitSurface(source, NULL, destination, &offset);
}

// Generates a random double between a and b
double random(double a, double b) {
  return (rand()/(double)RAND_MAX) * (b-a) + a;
}

double inv_mass(double w)
{
  if (w == 0)
    return 0;
  return (1/w);
}


//Returns the distance between two objects
double distance(object sqA, object sqB)
{
  double dX = pow(((sqB.x + sqB.r) - (sqA.x + sqA.r)),2);
  double dY = pow(((sqB.y + sqB.r) - (sqA.y + sqA.r)),2);
  return (dX + dY);
}

//Returns the distance between two objects
double distancePos(double aX, double aY, double bX, double bY, double aR, double bR)
{
  double dX = pow(((bX + bR) - (aX + aR)),2);
  double dY = pow(((bY + bR) - (aY + aR)),2);
  return sqrt(dX + dY);
}
