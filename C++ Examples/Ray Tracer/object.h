/**********************************************************************
* Author: Ben Golden
* File: object.h
* Modified On: 12/7/2015
* Description: Creates OBJ_T class from which sphere and plane inherit the intersect function
***********************************************************************/
#ifndef OBJECT_H
#define OBJECT_H
#include "vector.h"
#include "rt.h"

class OBJ_T {

public:
  COLOR_T color;
  int check;
  COLOR_T check_color;
  double reflection;
  
  virtual bool intersect(RAY_T, Vector*, Vector*, double*) = 0;
};

#endif