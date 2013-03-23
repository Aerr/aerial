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

/* Tells us if two balls are moving towards each other */
int movingToBall (object sqA, object sqB)
{
  /* Position Vector dotted with the Relative Velocity Vector */
  return ((sqB.x - sqA.x) * (sqA.vX - sqB.vX) + (sqB.y - sqA.y) * (sqA.vY - sqB.vY) > 0);
}

// Return the minimum between the spf and the time before collision
// So that none can be missed (used to remove the overlapping glitch)
double timeToCollision(object *sqs, double dt)
{
  double res = 999;
  for (int i = 0; i < NUM; i++)
    {
      for (int j = i + 1; j < NUM; j++)
        {
          if (movingToBall(sqs[i], sqs[j]))
            {
              double A,B,C,D,DISC;
              // Very long formula
              // equals the time depending on the distance
              A = pow(sqs[i].vX, 2) + pow(sqs[i].vY, 2) - 2 * sqs[i].vX * sqs[j].vX + pow(sqs[j].vX, 2) - 2 * sqs[i].vY * sqs[j].vY + pow(sqs[j].vY, 2);
              B = -sqs[i].x * sqs[i].vX - sqs[i].y * sqs[i].vY + sqs[i].vX * sqs[j].x + sqs[i].vY * sqs[j].y + sqs[i].x * sqs[j].vX - sqs[j].x * sqs[j].vX + sqs[i].y * sqs[j].vY - sqs[j].y * sqs[j].vY;
              C = pow(sqs[i].vX, 2) + pow(sqs[i].vY, 2) - 2 * sqs[i].vX * sqs[j].vX + pow(sqs[j].vX, 2) - 2 * sqs[i].vY * sqs[j].vY + pow(sqs[j].vY, 2);
              D = pow(sqs[i].x, 2) + pow(sqs[i].y, 2) - pow(sqs[i].r, 2) - 2 * sqs[i].x * sqs[j].x + pow(sqs[j].x, 2) - 2 * sqs[i].y * sqs[j].y + pow(sqs[j].y, 2) - 2 * sqs[i].r * sqs[j].r - pow(sqs[j].r, 2);
              DISC = pow((-2 * B), 2) - 4 * C * D;

              // ---------------------------------

              if (DISC >= 0)
                {
                  res = fmin(fmin(res, 0.5 * (2 * B - sqrt(DISC)) / A), 0.5 * (2 * B + sqrt(DISC)) / A);
                  //printf("Positive value : %f\n",res);
                }
            }
        }
    }
  if (fabs(res) < dt)
    printf("Collision between incoming : dt = %f ; res = %f\n", dt, res);
  return fmin(fabs(res),dt);
}
// Collision between objects and limits handler
void collision(object *sqA, object *sqB, double dt)
{
  double dX = pow(((sqB->x + sqB->r) - (sqA->x + sqA->r)),2);
  double dY = pow(((sqB->y + sqB->r) - (sqA->y + sqA->r)),2);
  // if distance < r1 + r2 ==> if there is collision
  // sqrt removed for performance issue
  if (movingToBall(*sqA, *sqB) && (dX + dY) <= pow(sqA->r + sqB->r, 2) + pow(10,-9))
    {
      double nx = (sqA->x - sqB->x) / (sqA->r + sqB->r);
      double ny = (sqA->y - sqB->y) / (sqA->r + sqB->r);
      double a1 = sqA->vX * nx + sqA->vY * ny;
      double a2 = sqB->vX * nx + sqB->vY * ny;
      double p = (a1 - a2) / (sqA->w + sqB->w);

      sqA->vX -= (1+sqA->e) * p * nx * sqB->w;
      sqA->vY -= (1+sqA->e) *  p * ny * sqB->w;
      sqA->x += sqA->vX * dt;
      sqA->y += sqA->vY * dt;

      sqB->vX += (1+sqB->e) * p * nx * sqA->w;
      sqB->vY += (1+sqB->e) * p * ny * sqA->w;
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
        random(-150,150),
        random(-150,150),
        random(0,1),
        random(10,100),
        20,
        IMG_Load("round.png")
      };
    }

  unsigned int frame = 0;
  double time = SDL_GetTicks();
  double update = SDL_GetTicks();
  double dt = 0;

  int g = 1;
  int bt = 100;
  while (!quit)
    {
      SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 64, 64, 64));

      if (SDL_GetTicks() - update > 1000)
        dt = spf(time, &update, frame);

      double incDt = timeToCollision(squares, dt);

      for (int i = 0; i < NUM; i++)
        {
          if (g)
            squares[i].vY += 9.8 * incDt * bt;

          distPlane(&squares[i], incDt);

          for (int j = i + 1; j < NUM; j++)
            collision(&squares[i], &squares[j], incDt);

          squares[i].x += squares[i].vX * incDt * (bt / 100);
          squares[i].y += squares[i].vY * incDt * (bt / 100);

          apply_surface(squares[i].x, squares[i].y, squares[i].img, screen);
        }

      // Screen updating
      if(SDL_Flip (screen) == -1)
        return EXIT_FAILURE;

      quit = handleInputs(&g, &bt);
      frame++;
    }

  // ========= FREEING ============ //
  for (int i = 0; i < NUM; i++)
    SDL_FreeSurface(squares[i].img);

  // ========= FREEING ============ //
  SDL_Quit();
  return EXIT_SUCCESS;
}
