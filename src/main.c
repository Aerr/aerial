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

  //  if (mB != 0)
  //    printf("Vector : (%f,%f)\n", sqA->vX - x * inv_mass(sqA->w), sqA->vY - y * inv_mass(sqA->w));


  sqA->vX -= x * inv_mass(sqA->w);
  sqA->vY -= y * inv_mass(sqA->w);
}

// Collision between objects and limits handler
void distPlane(object *square)
{
  if ((square->x <= 0) && (square->vX < 0))
    reflectionV(1, 0, square, 0, 0, 0);

  if ((square->y + 32 >= HEIGHT) && (square->vY > 0))
    reflectionV(0, -1, square, 0, 0, 0);

  if ((square->x + 32 >= WIDTH) && (square->vX > 0))
    reflectionV(-1, 0, square, 0, 0, 0);

  if ((square->y <= 0) && (square->vY < 0))
    reflectionV(0, 1, square, 0, 0, 0);
}

// Collision between objects and limits handler
void collision(object *squareA, object *squareB, double dt)
{
  double xA = squareA->x + squareA->vX * dt;
  double yA = squareA->y + squareA->vY * dt;

  double xB = squareB->x + squareB->vX * dt;
  double yB = squareB->y + squareB->vY * dt;

  double dX = pow(((xB + 16) - (xA + 16)),2);
  double dY = pow(((yB + 16) - (yA + 16)),2);

  if (sqrt(dX + dY) < 16 + 16)
    {
      if (squareA->vX < 0)
        {
          reflectionV(1, 0, squareA, squareB->vX, squareB->vY, squareB->w);
          reflectionV(1, 0, squareB, squareA->vX, squareA->vY, squareA->w);
        }
      else if (squareA->vX > 0)
        {
          reflectionV(-1, 0, squareA, squareB->vX, squareB->vY, squareB->w);
          reflectionV(-1, 0, squareB, squareA->vX, squareA->vY, squareA->w);
        }
      if (squareA->vY > 0)
        {
          reflectionV(0, -1, squareA, squareB->vX, squareB->vY, squareB->w);
          reflectionV(0, -1, squareB, squareA->vX, squareA->vY, squareA->w);
        }
      else if (squareA->vY < 0)
        {
          reflectionV(0, 1, squareA, squareB->vX, squareB->vY, squareB->w);
          reflectionV(0, 1, squareB, squareA->vX, squareA->vY, squareA->w);
        }
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
        random(-50,50),
        random(-50,50),
        random(0,1),
        random(10,100),
        IMG_Load("round.png")
      };
    }


  squares[0].w = 1000;

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
          squares[i].vY += 9.8 * dt * 100;

          distPlane(&squares[i]);

          for (int j = i + 1; j < NUM; j++)
	    collision(&squares[i], &squares[j], dt * 4);

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
