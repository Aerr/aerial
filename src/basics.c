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

