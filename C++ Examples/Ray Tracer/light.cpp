/**********************************************************************
* Author: Ben Golden
* File: light.cpp
* Modified On: 12/7/2015
* Description: Modifies Light class
***********************************************************************/
#include "rt.h"
#include "vector.h"
#include "object.h"
#include "scene.h"
#include "light.h"
#include <math.h>

//Constructor methods
Light::Light() :
  src(0.0, 0.0, 0.0)
{}

Light::Light(Vector newSrc) :
  src(newSrc)
{}

//Method to draw shadows in illuminate
bool Light::test_shadow(SCENE_T Scene, Vector int_pt, int closest_obj)
{
  //Declare dummy variables
  int i;
  bool intersect;
  Vector dummy_int_pt, dummy_normal;
  double dummy_t;
  
  //Create shadow ray
  RAY_T shadow_ray;
  
  shadow_ray.origin = int_pt;
  shadow_ray.dir = src - int_pt;
  shadow_ray.dir = shadow_ray.dir.normalize();
  
  //Loop to check for intersection
  for (i = 0; i < Scene.num_objs; i++){
    if (i != closest_obj){
      intersect = Scene.objs[i]->intersect(shadow_ray, &dummy_int_pt, &dummy_normal, &dummy_t);
      return intersect;
    }
  }
  
}

//Method to illuminate each pixel
COLOR_T Light::illuminate(RAY_T r, SCENE_T Scene, Vector *I, Vector *N, int closest)
{
  COLOR_T obj_color, light_color, shadow_color;
  
  obj_color = Scene.objs[closest]->color;

  //Checkerboard
  if(Scene.objs[closest]->check == 1){
    
    if((int)floor(I->getX()) + (int)floor(I->getY()) + (int)floor(I->getZ())&1){
      obj_color = Scene.objs[closest]->check_color;
    }
  }

  light_color.R = light_color.G = light_color.B = 0.0;

  //Light attenuation variables
  float atten, dist = (src - *I).magnitude();
  atten = 1/(.002*dist*dist + .02*dist + .2);
  
  //Diffuse lighting
  double diff = N->dot(src)/src.magnitude();
  
  if (diff > 0.1){

    diff *= (1 - Scene.objs[closest]->reflection);

    light_color.R += (diff * atten * obj_color.R * color.R);
    light_color.G += (diff * atten * obj_color.G * color.G);
    light_color.B += (diff * atten * obj_color.B * color.B);

  }

  //Specular lighting
  Vector spec;
  
  spec = src - (*N * 2 * N->dot(src));
  
  double power = pow(spec.normalize().dot(r.dir), 100);
  power = power * (1 - Scene.objs[closest]->reflection);

  if (spec.normalize().dot(r.dir) > 0){
    light_color.R += power * atten * color.R;
    light_color.G += power * atten * color.G;
    light_color.B += power * atten * color.B;
  }

  //Attenuate light
  light_color.R *= atten;
  
  light_color.G *= atten;
    
  light_color.B *= atten;

  return light_color;
}
