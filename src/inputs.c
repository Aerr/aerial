/*
 * Input management
 * Project: Aerial
 * Developer: Aer
 */
#include "inputs.h"

void reinit(object *squares, int *num)
{
  printf("======== Resetting ========\n");
  for (int i = 0; i < *num; i++)
    SDL_FreeSurface(squares[i].img);
  *num = 0;
  squares = malloc(sizeof(object) * MAX_NUM);
}


int createObject(object *squares, int num, int x, int y)
{
  int diameter = 40;
  x -= diameter / 2;
  y -= diameter / 2;
  if (x < 0)
    x = 0;
  else if (x > WIDTH - diameter)
    x = WIDTH - diameter;

  if (y < 0)
    y = 0;
  else if (y > HEIGHT - diameter)
    y = HEIGHT - diameter;


  if (!squares)
    printf("ERROR!\n");
  squares[num] = (object) {
    x,
    y,
    random(0,0),
    random(0,0),
    random(0.25,0.75),
    random(10,2000),
    diameter / 2,
    IMG_Load("round.png")
  };
  printf("Added\n");
  for (int j = num - 1; j > 0; j--)
    {
      if (distance(squares[num],squares[j]) <= pow(squares[num].r + squares[j].r, 2))
        {
          j = num - 1;
          printf("Replacing\n");
          squares[num].x = random(0,WIDTH - diameter);
          squares[num].y = random(0,HEIGHT - diameter);
        }
    }

  return (num + 1);
}

int handleInputs(int *g, int *bt, object *squares, int *num) {
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
            if (*bt > 1) {
              if (*bt <= 10)
                *bt -= 1;
              else if (*bt < 100)
                *bt -= 10;
              else if (*bt < 500)
                *bt -= 50;
              else
                *bt -= 100;
              printf("Slowing down time : %d\n", *bt);
            }
            break;
          case SDLK_UP:
            if (*bt < 100) {
              if (*bt < 10)
                *bt += 1;
              else if (*bt < 100)
                *bt += 10;
              else if (*bt < 500)
                *bt += 50;
              else
                *bt += 100;
              printf("Speeding up time : %d\n", *bt);
            }
            break;
          case SDLK_p:
            if (*bt)
              {
                *bt = 0;
                printf("Simulation paused.\n");
              }
            else
              {
                *bt = 100;
                printf("Simulation resumed.\n");
              }
            break;
          case SDLK_i:
            printf(" ==== Infos : %d circles ====\n", *num);
            for (int i = 0; i < *num; i++)
              {
                printf("%d -- (X;Y) : (%f;%f) - (vX;vY) : (%f;%f)\n", (i+1), squares[i].x, squares[i].y, squares[i].vX, squares[i].vY);
                printf("mass : %f - rest : %f - radius : %d\n", squares[i].w, squares[i].e, squares[i].r);
              }
            break;
          case SDLK_g:
            if (*g)
              printf("Gravity disabled.\n");
            else
              printf("Gravity enabled.\n");
            *g = !*g;
            break;
          case SDLK_TAB:
            reinit(squares, num);
            break;
          default:
            break;
          }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym)
          {
          default:
            break;
          }
        break;

      case SDL_MOUSEBUTTONUP:
        if (*num < MAX_NUM)
          {
            *num = createObject(squares, *num, event.button.x, event.button.y);
            printf("Object created : %d\n", *num);
          }
        else
          printf("Can't create objects no more.\n");
        break;

      default:
        break;
      }
  }
  return 0;
}
