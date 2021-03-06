/**********************************************************************
* Author: Ben Golden
* File: scene.h
* Modified On: 12/7/2015
* Description: Creates SCENE_T structure to store objects
***********************************************************************/
#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "rt.h"
#include "object.h"

class Light;

//Declare SCENE_T for scene
typedef struct {
  OBJ_T **objs;
  int num_objs;
  Light *light;
  int num_lights;
} SCENE_T;

#endif
