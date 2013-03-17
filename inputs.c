/*
* Input management
* Project: Aerial
* Developer: Aer
*/
#include "inputs.h"

int handleInputs() {
  SDL_Event event;
  while( SDL_PollEvent( &event ) ) {
    
    if( event.type == SDL_QUIT )
      return 1; 
      
    Uint8 *keystates = SDL_GetKeyState( NULL );
    if (keystates[SDLK_ESCAPE])
      return 1;
  }
  return 0;
}
