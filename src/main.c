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

double inv_mass(double w)
{
  if (w == 0)
    return 0;
  return (1/w);
}

// Calculates the reflection vector and returns the effect on velocity
Vector2 reflectionV(int a, int b, double vX, double vY, double e, double mA, double mB)
{
  double x = ((1+e)*(a*(vX * a + vY * b))) / (inv_mass(mA) + inv_mass(mB));
  double y = (1+e)*(b*(vX * a + vY * b))  / (inv_mass(mA) + inv_mass(mB));
  return (Vector2) { x, y };
}

int isVectorZero(Vector2 v)
{
  return (v.X == 0) && (v.Y) == 0;
}
// Collision between objects and limits handler
void distPlane(int j, object *square)
{
  Vector2 r = (Vector2) {0, 0};
  switch (j)
    {
    case 0: // (1, 0, 1)
      if ((square->x < -1) && (square->vX < 0))
        r = reflectionV(1, 0, square->vX, square->vY, square->e, square->w, 0);
      break;

    case 1: // (0, -1, 1)
      if ((square->y + 32 > HEIGHT) && (-square->vY < 0))
        r = reflectionV(0, -1, square->vX, square->vY, square->e, square->w, 0);
      break;

    case 2: // (-1, 0, 1)
      if ((square->x + 32 > WIDTH) && (-square->vX < 0))
        r = reflectionV(-1, 0, square->vX, square->vY, square->e, square->w, 0);
      break;

    case 3: // (0, 1, 1)
      if ((square->y < -1) && (square->vY < 0))
        r = reflectionV(0, 1, square->vX, square->vY, square->e, square->w, 0);
      break;
    }
  if (!isVectorZero(r))
    {
      square->vX -= r.X * inv_mass(square->w);
      square->vY -= r.Y * inv_mass(square->w);
    }
}

double spf(double time, double *update, int frame)
{
  // frame per second
  double fps = ((float)frame / ((SDL_GetTicks() - time) / 1000.f));
  // displaying fps in the taskbar
  char caption[ 64 ];
  sprintf(caption, "Frames Par Seconde: %f", fps);
  SDL_WM_SetCaption(caption, NULL);
  // restarting the timer
  *update = SDL_GetTicks();
  // second per frames : used to compute how much movements must be made
  return ((SDL_GetTicks() - time) / 1000.f) / (float)frame;
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

  if(!screen)
    {
      printf( "Can't set video mode: %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }

  object squares[NUM];
  for (int i = 0; i < NUM; i++)
    {
      squares[i] = (object) {
        random(0,WIDTH - 32),
        random(0,HEIGHT - 32),
        random(-50,50),
        random(-500,500),
        random(0,1),
        random(50,100),
        IMG_Load("round.png")
      };
    }


  unsigned int frame = 0;
  double time = SDL_GetTicks();
  double update = SDL_GetTicks();
  double dt = 0;
  while (!quit)
    {
      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

      if (SDL_GetTicks() - update > 1000)
        dt = spf(time, &update, frame);

      for (int i = 0; i < NUM; i++)
        {

          squares[i].vY += 9.8 * dt;


          for (int j = 0; j < 4; j++)
            distPlane(j, &(squares[i]));

          squares[i].x += squares[i].vX * dt;
          squares[i].y += squares[i].vY * dt;

          apply_surface(squares[i].x, squares[i].y, squares[i].img, screen);
        }

      // Screen updating
      if(SDL_Flip (screen) == -1)
        return EXIT_FAILURE;

      quit = handleInputs();
      frame++;
    }

  // ========= FREEING ============ //
  for (int i = 0; i < NUM; i++)
    SDL_FreeSurface(squares[i].img);

  // ========= FREEING ============ //
  SDL_Quit();
  return EXIT_SUCCESS;
}
