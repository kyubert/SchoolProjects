/**********************************************************************
* Author: Ben Golden
* File: sphere.cpp
* Modified On: 12/7/2015
* Description: Provides methods for Sphere class
***********************************************************************/
#include "rt.h"
#include "vector.h"
#include "object.h"
#include "sphere.h"
#include <math.h>

//Constructor
Sphere::Sphere() :
  ctr(0.0, 0.0, 0.0),
  r(0.0)
{}

Sphere::Sphere(double newR, Vector newCtr) :
  ctr(newCtr),
  r(newR)
{}

//Method to determine if a ray intersects with a sphere
bool Sphere::intersect(RAY_T ray, Vector *int_pt, Vector *normal, double *t){

  //Declare and obtain the values for A, B, and C using ray and sphere
  double A, B, C;
  double t0, t1, disc;
  
  A = 1;
  B = 2 * ray.dir.dot(ray.origin - ctr);
  C = ctr.dot(ctr) + ray.origin.dot(ray.origin) - 2 * ctr.dot(ray.origin) - r * r;
  
  disc = B * B - 4 * A * C;
  
  if (disc < 0)
    return 0;
  
    
  //Find the minimum positive "t"; if both values for t are negative, return 0
  t0 = (-B - sqrt(disc)) / (2 * A);
  t1 = (-B + sqrt(disc)) / (2 * A);
  
  if (t0 < 0 && t1 < 0){
    return 0;
  }

  if (t1 < 0 || (t0 < t1 && t0 > 0)) {
    *t = t0;
  }
  else{
    *t = t1;
  }
    
  //Record values to pointers
  *int_pt = ray.origin + ray.dir * (*t);
    
  *normal = (*int_pt - ctr)/r;
    
  return 1;
    
}