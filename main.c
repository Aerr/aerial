/*
* Main function
* Project: Aerial
* Developer: Aer
*/

#include "main.h"

#define NUM 10

// Generates a random double between a and b
double random(double a, double b) {
  return (rand()/(double)RAND_MAX ) * (b-a) + a;
}

// Calculates the reflection vector and returns the effect on velocity
Vector2 reflectionV(int a, int b, double vX, double vY)
{
  return (Vector2) {2*(a*(vX * a + vY * b)), 2*(b*(vX * a + vY * b)) };
}
// Collision between objects and limits handler
void distPlane(int j, object *square)
{
  switch (j)
    {
    case 0: // (1, 0, 1)
      if ((square->x < -1) && (square->vX < 0))
        {
	  Vector2 r = reflectionV(1, 0, square->vX, square->vY);
          square->vX -= r.X;
          square->vY -= r.Y;
        }
      break;

    case 1: // (0, -1, 1)
      if ((square->y + 32 > HEIGHT) && (-square->vY < 0))
        {
          Vector2 r = reflectionV(0, -1, square->vX, square->vY);
          square->vX -= r.X;
          square->vY -= r.Y;
        }
      break;

    case 2: // (-1, 0, 1)
      if ((square->x + 32 > WIDTH) && (-square->vX < 0))
        {
	        Vector2 r = reflectionV(-1, 0, square->vX, square->vY);
          square->vX -= r.X;
          square->vY -= r.Y;
        }
      break;

    case 3: // (0, 1, 1)
      if ((square->y < -1) && (square->vY < 0))
        {
      	  Vector2 r = reflectionV(0, 1, square->vX, square->vY);
          square->vX -= r.X;
          square->vY -= r.Y;
        }
      break;
    }
}


int main()
{
  srand(time(NULL)); // rand initialization
  size_t quit = 0;
  
  if( SDL_Init( SDL_INIT_VIDEO ) == -1 )
    {
      printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
      return EXIT_FAILURE;
    }

  SDL_WM_SetCaption( "Aerial", NULL );
  SDL_Surface *screen = NULL;
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);

  if( screen == NULL )
    {
      printf( "Can't set video mode: %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }

  object squares[NUM];
  for (int i = 0; i < NUM; i++)
    {
      squares[i] = (object) {
        random(0,WIDTH),
        random(0,HEIGHT),
        random(0,1.5),
        random(0,1.5),
        IMG_Load("round.png")
      };
    }

  while (!quit)
    {
      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

      for (int i = 0; i < NUM; i++)
        {
          /*
            if ((squares[i].x <= 0 && squares[i].vX < 0) ||
            (squares[i].x + 32 >= WIDTH && squares[i].vX > 0))
            squares[i].vX *= -1;
            if ((squares[i].y <= 0 && squares[i].vY < 0) ||
            (squares[i].y + 32 >= HEIGHT && squares[i].vY > 0))
            squares[i].vY *= -1;
          */

          for (int j = 0; j < 4; j++)
            distPlane(j, &(squares[i]));

          squares[i].x += squares[i].vX;
          squares[i].y += squares[i].vY;

          apply_surface(squares[i].x, squares[i].y, squares[i].img, screen);
        }

      // Screen updating
      if( SDL_Flip (screen) == -1 ) {
        return EXIT_FAILURE;
      }


      quit = handleInputs();
    }
    
  // ========= FREEING ============ //
  for (int i = 0; i < NUM; i++)
    SDL_FreeSurface(squares[i].img);

  // ========= FREEING ============ //
  SDL_Quit();
  return EXIT_SUCCESS;
}
