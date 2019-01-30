/**********************************************************************
* Author: Ben Golden
* File: sphere.h
* Modified On: 12/7/2015
* Description: Creates Sphere class
***********************************************************************/
#ifndef SPHERE_H
#define SPHERE_H
#include "vector.h"
#include "rt.h"
#include "object.h"

class Sphere : public OBJ_T {
public:
  Sphere();
  Sphere(double, Vector);
  bool intersect(RAY_T, Vector*, Vector*, double*);

private:
  double r;
  Vector ctr;
};

#endif