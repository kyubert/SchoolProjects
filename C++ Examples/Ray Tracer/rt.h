/**********************************************************************
* Author: Ben Golden
* File: rt.h
* Modified On: 12/7/2015
* Description: Defines structures for use in ray tracer
***********************************************************************/
#ifndef RT_H
#define RT_H
#define NUM_OBJS 4
#define X_RES 1024
#define Y_RES 768

#include "vector.h"

//Define structure RAY_T for rays
typedef struct {
  Vector origin;
  Vector dir;
} RAY_T;

//Define structure COLOR_T for colors
typedef struct {
  double R, G, B;
} COLOR_T;

#endif