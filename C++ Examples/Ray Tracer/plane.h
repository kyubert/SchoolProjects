/**********************************************************************
* Author: Ben Golden
* File: plane.h
* Modified On: 12/7/2015
* Description: Creates Plane class
***********************************************************************/
#ifndef PLANE_H
#define PLANE_H
#include "vector.h"
#include "rt.h"
#include "object.h"

class Plane : public OBJ_T{
public:
  Plane();
  Plane(double, Vector);
  bool intersect(RAY_T, Vector*, Vector*, double*);

private:
  Vector nor;
  double D;
};

#endif