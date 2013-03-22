/*
 * Main function
 * Project: Aerial
 * Developer: Aer
 */

#include "main.h"

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

// Calculates the reflection vector and returns the effect on velocity
void reflectionV(int a, int b, object *sqA, double vBx, double vBy, double mB)
{
  double x = ((1+sqA->e)*(a*((sqA->vX - vBx) * a + (sqA->vY - vBy) * b))) / (inv_mass(sqA->w) + inv_mass(mB));
  double y = ((1+sqA->e)*(b*((sqA->vX - vBx) * a + (sqA->vY - vBy) * b))) / (inv_mass(sqA->w) + inv_mass(mB));

  sqA->vX -= x * inv_mass(sqA->w);
  sqA->vY -= y * inv_mass(sqA->w);
}

// Collision between objects and limits handler
void distPlane(object *sq, double dt)
{
  if ((sq->x <= 0 + 1) && (sq->vX < 0))
    reflectionV(1, 0, sq, 0, 0, 0);

  if ((sq->y + sq->r * 2 >= HEIGHT - 1) && (sq->vY > 0))
    reflectionV(0, -1, sq, 0, 0, 0);

  if ((sq->x + sq->r * 2 >= WIDTH - 1) && (sq->vX > 0))
    reflectionV(-1, 0, sq, 0, 0, 0);

  if ((sq->y <= 0 + 1) && (sq->vY < 0))
    reflectionV(0, 1, sq, 0, 0, 0);

  sq->x += sq->vX * dt;
  sq->y += sq->vY * dt;

}

// Collision between objects and limits handler
void collision(object *sqA, object *sqB, double dt)
{
  double dX = pow(((sqB->x + sqB->r) - (sqA->x + sqA->r)),2);
  double dY = pow(((sqB->y + sqB->r) - (sqA->y + sqA->r)),2);

  if ((dX + dY) < pow(sqA->r + sqB->r, 2))
    {
      double nx = (sqA->x - sqB->x) / (sqA->r + sqB->r);
      double ny = (sqA->y - sqB->y) / (sqA->r + sqB->r);
      double a1 = sqA->vX * nx + sqA->vY * ny;
      double a2 = sqB->vX * nx + sqB->vY * ny;
      double p = 2 * (a1 - a2) / (sqA->w + sqB->w);

      sqA->vX -= p * nx * sqB->w;
      sqA->vY -= p * ny * sqB->w;
      sqA->x += sqA->vX * dt;
      sqA->y += sqA->vY * dt;

      sqB->vX += p * nx * sqA->w;
      sqB->vY += p * ny * sqA->w;
      sqB->x += sqB->vX * dt;
      sqB->y += sqB->vY * dt;
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

  if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
      printf("Can't init SDL:  %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }

  SDL_WM_SetCaption("Aerial", NULL);
  SDL_Surface *screen = NULL;
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);

  if(!screen)
    {
      printf("Can't set video mode: %s\n", SDL_GetError());
      return EXIT_FAILURE;
    }

  object squares[NUM];
  for (int i = 0; i < NUM; i++)
    {
      squares[i] = (object) {
        random(0,WIDTH - 32),
        random(0,HEIGHT - 32),
        random(0,0),
        random(0,0),
        random(0,1),
        random(10,100),
        24,
        IMG_Load("round.png")
      };
    }

  unsigned int frame = 0;
  double time = SDL_GetTicks();
  double update = SDL_GetTicks();
  double dt = 0;

  int g = 1;
  while (!quit)
    {
      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 64, 64, 64));

      if (SDL_GetTicks() - update > 1000)
        dt = spf(time, &update, frame);

      for (int i = 0; i < NUM; i++)
        {
	  if (g)
	    squares[i].vY += 9.8 * dt * 100;

          distPlane(&squares[i], dt);

          for (int j = i + 1; j < NUM; j++)
            collision(&squares[i], &squares[j], dt);

          squares[i].x += squares[i].vX * dt;
          squares[i].y += squares[i].vY * dt;

          apply_surface(squares[i].x, squares[i].y, squares[i].img, screen);
        }

      // Screen updating
      if(SDL_Flip (screen) == -1)
        return EXIT_FAILURE;

      quit = handleInputs(&g);
      frame++;
    }

  // ========= FREEING ============ //
  for (int i = 0; i < NUM; i++)
    SDL_FreeSurface(squares[i].img);

  // ========= FREEING ============ //
  SDL_Quit();
  return EXIT_SUCCESS;
}



/*      double A,B,C,D,DISC;
        A = pow(sqA->vX , 2) + pow(sqA->vY , 2) - 2 * sqA->vX * sqB->vX + pow(sqB->vX , 2) - 2 * sqA->vY * sqB->vY + pow(sqB->vY , 2);
        B = -sqA->x * sqA->vX - sqA->y * sqA->vY + sqA->vX * sqB->x + sqA->vY * sqB->y + sqA->x * sqB->vX - sqB->x * sqB->vX + sqA->y * sqB->vY - sqB->y * sqB->vY;
        C = pow(sqA->vX , 2) + pow(sqA->vY , 2) - 2 * sqA->vX * sqB->vX + pow(sqB->vX , 2) - 2 * sqA->vY * sqB->vY + pow(sqB->vY , 2);
        D = pow(sqA->x , 2) + pow(sqA->y , 2) - pow(sqA->r , 2) - 2 * sqA->x * sqB->x + pow(sqB->x , 2) - 2 * sqA->y * sqB->y + pow( sqB->y , 2) - 2 * sqA->r * sqB->r - pow(sqB->r , 2);
        DISC = pow((-2 * B) , 2) - 4 * C * D;
*/
