/*
* Input management
* Project: Aerial
* Developer: Aer
*/
#ifndef _INPUTS_H_
#define _INPUTS_H_

// INCLUDE
#include "SDL/SDL.h"

#include "basics.h"

//PROTOTYPE
int handleInputs(int *g, int *bt, object *squares, int *num);
int createObject(object *squares, int num, int x, int ym);
void reinit(object *squares, int *num);

#endif
