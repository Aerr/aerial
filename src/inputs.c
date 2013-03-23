/*
 * Input management
 * Project: Aerial
 * Developer: Aer
 */
#include "inputs.h"

int handleInputs(int *g, int *bt) {
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    switch (event.type)
      {
      case SDL_QUIT:
        return 1;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
          {
          case SDLK_ESCAPE:
            return 1;
            break;
          case SDLK_DOWN:
            if (*bt > 10) {
              if (*bt <= 100)
                *bt -= 10;
              else if (*bt < 500)
                *bt -= 50;
              else
                *bt -= 100;
              printf("Slowing down time : %d\n", *bt);
            }
            break;
          case SDLK_UP:
            if (*bt < 1000)
              {
                *bt += 100;
                printf("Speeding up time : %d\n", *bt);
              }
            break;
          default:
            break;
          }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym)
          {
          case SDLK_g:
            *g = !*g;
            break;
          default:
            break;
          }
        break;

      case SDL_MOUSEBUTTONUP:
        printf("Relâchement bouton souris %d\n", event.button.button);
        break;

      default:
        break;
      }
  }
  return 0;
}
