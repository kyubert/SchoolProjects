/**********************************************************************
* Author: Ben Golden
* File: plane.cpp
* Modified On: 12/7/2015
* Description: Provides methods for Plane class
***********************************************************************/
#include "rt.h"
#include "vector.h"
#include "plane.h"
#include <math.h>

//Constructor
Plane::Plane() :
  nor(0.0, 0.0, 0.0),
  D(0.0)
{}

Plane::Plane(double newD, Vector newNor) :
  nor(newNor.normalize()),
  D(newD)
{}
//Function to determine if ray intersects with a plane
bool Plane::intersect(RAY_T ray, Vector *int_pt, Vector *normal, double *t){

  //Takes dot product of plane normal and ray direction
  double dot;
  
  dot = nor.dot(ray.dir);

  if(dot == 0) return 0;

  //Finds t and intersection point, then normalizes int_pt
  *t = -(dot + D)/dot;
  
  if ( *t < 0 ) return 0;
  
  *int_pt = ray.origin + ray.dir * *t;
  
  *normal = nor;
  
  return 1;
}