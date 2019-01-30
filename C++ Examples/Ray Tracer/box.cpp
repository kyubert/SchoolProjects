/**********************************************************************
* Author: Ben Golden
* File: box.cpp
* Modified On: 12/7/2015
* Description: Provides methods for Box class
***********************************************************************/
#include "rt.h"
#include "vector.h"
#include "object.h"
#include "box.h"
#include <math.h>

//Constructor
Box::Box() :
     lole(0.0, 0.0, 0.0),
     ur(0.0, 0.0, 0.0)
{}

Box::Box(Vector newLoLe, Vector newUR) :
     lole(newLoLe),
     ur(newUR)
{}

//Intersect function for a box
bool Box::intersect(RAY_T ray, Vector *int_pt, Vector *normal, double *t){

     double t_near = -1000, t_far = 1000, t1, t2;
     
     //Declare and initialize arrays for ray values
     double raydir[3], rayor[3], upperright[3], lowerleft[3];
     
     raydir[0] = ray.dir.getX();
     raydir[1] = ray.dir.getY();
     raydir[2] = ray.dir.getZ();
     
     rayor[0] = ray.origin.getX();
     rayor[1] = ray.origin.getY();
     rayor[2] = ray.origin.getZ();
     
     upperright[0] = ur.getX();
     upperright[1] = ur.getY();
     upperright[2] = ur.getZ();
     
     lowerleft[0] = lole.getX();
     lowerleft[1] = lole.getY();
     lowerleft[2] = lole.getZ();
     
     //Declare and initialize array for planes
     Vector norm_arr[6];
     
     norm_arr[0] = Vector(1.0, 0.0, 0.0);
     norm_arr[1] = Vector(-1.0, 0.0, 0.0);
     norm_arr[2] = Vector(0.0, 1.0, 0.0);
     norm_arr[3] = Vector(0.0, -1.0, 0.0);
     norm_arr[4] = Vector(0.0, 0.0, -1.0);
     norm_arr[5] = Vector(0.0, 0.0, 1.0);
     
     int n_index, i;
     
     bool swap_flag;
     
     //Begin for loop to intersect planes
     
     for(i = 0; i < 3; i++){
       
       swap_flag = 0;
       //Find parallels
       if(raydir[i] == 0){
         if(raydir[i] < lowerleft[i] || raydir[i] > upperright[i]){
           return false;
         }
       }
       
       //Compute ts
       t1 = (lowerleft[i] - rayor[i]) / raydir[i];
       t2 = (upperright[i] - rayor[i]) / raydir[i];
       
       if(t1 > t2){
         std::swap(t1, t2);
         swap_flag = 1;
       }
       
       if(t1 > t_near){
         t_near = t1;
         n_index = 2 * i + swap_flag;
       }
         
       if(t2 < t_far) t_far = t2;
       
       if(t_near > t_far) return false;
       if(t_far < 0) return false;
     }
     
     *t = -(norm_arr[n_index].dot(ray.dir) + t_near) / (norm_arr[n_index].dot(ray.dir));
     
     *int_pt = ray.origin + ray.dir * *t;
     *normal = int_pt->normalize();
     
     return 1;
}