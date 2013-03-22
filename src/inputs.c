/*
 * Input management
 * Project: Aerial
 * Developer: Aer
 */
#include "inputs.h"

int handleInputs(int *g) {
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
