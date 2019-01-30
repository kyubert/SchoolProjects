/**********************************************************************
* Author: Ben Golden
* File: box.h
* Modified On: 12/7/2015
* Description: Creates Box class
***********************************************************************/
#ifndef BOX_H
#define BOX_H
#include "vector.h"
#include "rt.h"
#include "object.h"

class Box : public OBJ_T {
public:
  Box();
  Box(Vector, Vector);
  bool intersect(RAY_T, Vector*, Vector*, double*);

private:
  Vector lole;
  Vector ur;
};

#endif