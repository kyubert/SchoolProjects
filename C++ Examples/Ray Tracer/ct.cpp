/**********************************************************************
* Author: Ben Golden
* File: rt.cpp
* Modified On: 12/7/2015
* Description: Uses vector functions and geometric types to ray trace creative image
***********************************************************************/
#include "vector.h"
#include "rt.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "object.h"
#include "scene.h"
#include "box.h"
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;


//Function for basic lighting
COLOR_T do_lighting(RAY_T r, SCENE_T *Scene, Vector *I, Vector *N, int closest){

  COLOR_T color = Scene->objs[closest]->color;

  int i;
  
  //Checkerboard
  if(Scene->objs[closest]->check == 1){
    
    if((int)floor(I->getX()) + (int)floor(I->getY()) + (int)floor(I->getZ())&1)
      color = Scene->objs[closest]->check_color;
  }

  //Ambient lighting
  color.R *= 0.1;
  color.G *= 0.1;
  color.B *= 0.1;

  //For loop for multiple lights
  for ( i = 0; i < Scene->num_lights; i++){
    //Shadow test
    if(Scene->light[i].test_shadow(*Scene, *I, closest) == 0){
      color.R += Scene->light[i].illuminate(r, *Scene, I, N, closest).R;
      color.G += Scene->light[i].illuminate(r, *Scene, I, N, closest).G;
      color.B += Scene->light[i].illuminate(r, *Scene, I, N, closest).B;
    }
  }
  

  return color;
}

//Function to trace objects
COLOR_T trace(RAY_T ray, SCENE_T *scene, int depth, int current_obj){
  
  COLOR_T color;
  
  //Make sure reflection doesn't overflow
  if(depth == 5){
    color.R = color.G = color.B = 0.0;
    return color;
  }
  
  //Declare variables for intersect function
  double t, closest_t = 1000.0;
  int closest_obj = -1;
  Vector closest_int_pt, closest_normal, int_pt, normal;
  
  //Initialize background color
  color.R = color.G = 0.3;
  color.B = 0.5;
  
  //Initialize iterator, intersect, and flag
  int i, flag = 0;
  bool intersect;
  
  //Loop check for intersections for all object
  for(i=0; i < scene->num_objs; i++){
    if( i != current_obj ){
    
      intersect = scene->objs[i]->intersect(ray, &int_pt, &normal, &t);
      
      //Determines closest object
      if(intersect == 1){
      
        if(t < closest_t){
          closest_t = t;
          closest_int_pt = int_pt;
          closest_normal = normal;
          closest_obj = i;
        }
      }
    }
  }
  
  if(closest_t < 1000.0){
    color = do_lighting(ray, scene, &closest_int_pt, &closest_normal, closest_obj);
  }
  
  //Reflection
  if(closest_obj >= 0){
    COLOR_T refl_color;
    refl_color.R = 0.0;
    refl_color.G = 0.0;
    refl_color.B = 0.0;
    
    COLOR_T local_color;
    
    RAY_T refl_ray;
    refl_ray.origin = Vector(0.0, 0.0, 0.0);
    refl_ray.dir = Vector(0.0, 0.0, 0.0);
    
    if(scene->objs[closest_obj]->reflection > 0){
      
      refl_ray.origin = int_pt;
      refl_ray.dir = ray.dir - normal * (2 * normal.dot(ray.dir));
      refl_ray.dir.normalize();
      
      refl_color = trace(refl_ray, scene, depth + 1, closest_obj);
    }
    
    if(scene->objs[closest_obj]->reflection < 1){
      local_color = do_lighting(ray, scene, &closest_int_pt, &closest_normal, closest_obj);
      
    }
    
    color.R = refl_color.R * scene->objs[closest_obj]->reflection + local_color.R;
    color.G = refl_color.G * scene->objs[closest_obj]->reflection + local_color.G;
    color.B = refl_color.B * scene->objs[closest_obj]->reflection + local_color.B;
  }
  
  return color;
}

//Initialization
void init(SCENE_T *scene){
  
  scene->num_objs = 9;
  scene->objs = new OBJ_T*[9];
  scene->num_lights = 3;
  scene->light = new Light[3];
  
  scene->light[0].color.R = 1.0;
  scene->light[0].color.G = scene->light[0].color.B = 0.5;
  scene->light[0].src = Vector(0, 10.0, -5.0);

  scene->light[1].color.R = 0.5;
  scene->light[1].color.G = 1.0;
  scene->light[1].color.B = 0.5;
  scene->light[1].src = Vector(-10.0, -10.0, -5.0);
  
  scene->light[2].color.R = 0.5;
  scene->light[2].color.G = 0.5;
  scene->light[2].color.B = 1.0;
  scene->light[2].src = Vector(10.0, -10.0, -5.0);
  
  Plane *plane;
  Sphere *sphere;
  Box *box;
  
  sphere = new Sphere(0.5, Vector(0, 1.2, 4.0));
  scene->objs[0] = sphere;

  scene->objs[0]->color.R = 0.8;
  scene->objs[0]->color.G = 0.0;
  scene->objs[0]->color.B = 0.0;
  scene->objs[0]->check = 0;
  scene->objs[0]->reflection = 0.5;
  
  sphere = new Sphere(0.5, Vector(1.0, 0.5, 4.0));
  scene->objs[1] = sphere;
  
  scene->objs[1]->color.G = 0.8;
  scene->objs[1]->color.R = 0.8;
  scene->objs[1]->color.B = 0.0;
  scene->objs[1]->check = 0;
  scene->objs[1]->reflection = 0.5;
  
  sphere = new Sphere(0.5, Vector(1.0, -0.5, 4.0));
  scene->objs[2] = sphere;

  scene->objs[2]->color.G = 0.8;
  scene->objs[2]->color.R = 0.0;
  scene->objs[2]->color.B = 0.0;
  scene->objs[2]->check = 0;
  scene->objs[2]->reflection = 0.5;
  
  sphere = new Sphere(0.5, Vector(0, -1.2, 4.0));
  scene->objs[3] = sphere;

  scene->objs[3]->color.G = 0.8;
  scene->objs[3]->color.R = 0.0;
  scene->objs[3]->color.B = 0.8;
  scene->objs[3]->check = 0;
  scene->objs[3]->reflection = 0.5;
  
  sphere = new Sphere(0.5, Vector(-1.0, -0.5, 4.0));
  scene->objs[4] = sphere;

  scene->objs[4]->color.G = 0.0;
  scene->objs[4]->color.R = 0.0;
  scene->objs[4]->color.B = 0.8;
  scene->objs[4]->check = 0;
  scene->objs[4]->reflection = 0.5;
  
  sphere = new Sphere(0.5, Vector(-1.0, 0.5, 4.0));
  scene->objs[5] = sphere;

  scene->objs[5]->color.G = 0.0;
  scene->objs[5]->color.R = 0.8;
  scene->objs[5]->color.B = 0.8;
  scene->objs[5]->check = 0;
  scene->objs[5]->reflection = 0.5;
  
  plane = new Plane(8.0, Vector(-0.8, 0.0, -0.6));
  scene->objs[6] = plane;
  
  scene->objs[6]->color.R = scene->objs[6]->color.G = scene->objs[6]->color.B = 1.0;
  scene->objs[6]->check = 1;
  scene->objs[6]->check_color.R = scene->objs[6]->check_color.G = scene->objs[6]->check_color.B = 0.0;
  scene->objs[6]->reflection = 0.0;
  
  plane = new Plane(8.0, Vector(0.8, 0.0, -0.6));
  scene->objs[7] = plane;
  
  scene->objs[7]->color.R = scene->objs[7]->color.G = scene->objs[7]->color.B = 1.0;
  scene->objs[7]->check = 1;
  scene->objs[7]->check_color.R = scene->objs[7]->check_color.G = scene->objs[7]->check_color.B = 0.0;
  scene->objs[7]->reflection = 0.0;
  
  box = new Box(Vector(-.5, -.5, 4.0), Vector(.5, .5, 4.5));
  scene->objs[8] = box;
  
  scene->objs[8]->color.R = scene->objs[8]->color.G = scene->objs[8]->color.B = 1.0;
  scene->objs[8]->check = 0;
}

//Main function
int main(void)
{

  //Declare iterator + resolution variables
  int i;
  int x_res = 240, y_res = 240;
  float ratio = (float)x_res/(float)y_res;

  
  //Declare scene and allocate memory for objects
  SCENE_T scene;
  
  //Initialize objects
  init(&scene);

  //Declare dimensions for ray
  RAY_T ray;
  
  //Declare color for ray tracer
  COLOR_T drawn_color;
  
  //Declare file pointer/write to img.ppm
  ofstream outf("img.ppm");
 
  //Print ppm header
  outf << "P6" << endl << x_res << " " << y_res << endl << "255" << endl;
  
  //Loop draws a vector pointing to every pixel
  int x, y;
  
  for (y = 0; y < y_res; y++) {
    for (x = 0; x < x_res; x++) {
    
      ray.origin = Vector();
      
      ray.dir = Vector(-(ratio / 2) + x / (float)y_res, .5 - y / (float)y_res, 1);
      
      ray.dir.normalize();
      
      
      //Checks for negative space and uses intersect function to color sphere

       drawn_color = trace(ray, &scene, 0, -1);
       
       //Caps out colors
       if(drawn_color.R > 1.0){
         drawn_color.R = 1.0;
       }
       
       if(drawn_color.G > 1.0){
         drawn_color.G = 1.0;
       }
       
       if(drawn_color.B > 1.0){
         drawn_color.B = 1.0;
       }
        
       outf << (unsigned char)(drawn_color.R * 255) << (unsigned char)(drawn_color.G * 255) << (unsigned char)(drawn_color.B * 255);
    }
  }
  
  //Close outf
  outf.close();
  
  return 0;
}
