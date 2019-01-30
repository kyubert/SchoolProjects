#ifndef AW_T_H
#define AW_T_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#ifdef __APPLE__
//#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//#include <OpenGL/gl.h>
//#include <OpenGL/glext.h>
//#include <GLUT/glut.h>
// use modern GL 3.2
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include "aw_t.h"

aw_t::aw_t(const char* ofile){
	type = 0;
  //create file streams to read from given files
  std::ifstream ofs;
  std::vector<std::string> object;
	std::string element;
  std::stringstream ostring;
  // ensure ifstream objects can throw exceptions:
  ofs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try 
    {
      // open files
      ofs.open(ofile);
      // read file's buffer contents into streams
      ostring << ofs.rdbuf();		
      // close file handlers
      ofs.close();
    }
  //check if files have been read
  catch(std::ifstream::failure e)
  {
      std::cerr << "FILE NOT READ" << std::endl;
  }

	//parse string with getline
	while(ostring >> element) 
  { 
      object.push_back(element);
  }

	//temp vectors to store values
	std::vector<glm::vec3 >       vv;
	std::vector<glm::vec3 >       vt;
	std::vector<glm::vec3 >       vn;


	int i = 0;
	while(i < object.size()){
		if(object[i] == "v"){
			vv.emplace_back(glm::vec3(stof(object[i+1]), stof(object[i+2]), stof(object[i+3])));
			i += 3;
		}
		else if(object[i] == "vt"){
			vt.emplace_back(glm::vec3(stof(object[i+1]), stof(object[i+2]), 0.0));
			i += 2;
			type = type | 1;
		}
		else if(object[i] == "vn"){
			vn.emplace_back(glm::vec3(stof(object[i+1]), stof(object[i+2]), stof(object[i+3])));
			i += 3;
			type = type | 2;
		}
		else if(object[i] == "g"){
			group = object[i+1];
			i++;
		}
		else if(object[i] == "f"){
			vi.emplace_back(stoi(object[i+1]) - 1);
			vi.emplace_back(stoi(object[i+2]) - 1);
			vi.emplace_back(stoi(object[i+3]) - 1);
			i += 3;
		}
		i++;
	}

	for(unsigned int j = 0; j < vi.size(); j++){
		verts.emplace_back(vv[vi[j]]);
	}

	verts.insert(verts.end(), vt.begin(), vt.end());
	verts.insert(verts.end(), vn.begin(), vn.end());
}

void aw_t::pack(GLfloat v[]){

	if(type == 0){
		v[verts.size()*3];
		for(int i = 0; i < verts.size(); i++){
			v[3*i] = verts[i].x;
			v[3*i+1] = verts[i].y;
			v[3*i+2] = verts[i].z;
		}
	}

	else if(type == 1){
		v[verts.size()*3];
		for(int i = 0; i < verts.size()/2; i++){
			v[5*i] = verts[i].x;
			v[5*i+1] = verts[i].y;
			v[5*i+2] = verts[i].z;
			v[5*i+3] = verts[i+verts.size()/2].x;
			v[5*i+4] = verts[i+verts.size()/2].y;
		}
	}

	else if(type == 2){
		v[verts.size()*3];
		for(int i = 0; i < verts.size()/2; i++){
			v[6*i] = verts[i].x;
			v[6*i+1] = verts[i].y;
			v[6*i+2] = verts[i].z;
			v[6*i+3] = verts[i+verts.size()/2].x;
			v[6*i+4] = verts[i+verts.size()/2].y;
			v[6*i+5] = verts[i+verts.size()/2].z;
		}
	}

	else if(type == 3){
		v[verts.size()*3];
		for(int i = 0; i < verts.size()/3; i++){
			v[8*i] = verts[i].x;
			v[8*i+1] = verts[i].y;
			v[8*i+2] = verts[i].z;
			v[8*i+3] = verts[i+verts.size()/3].x;
			v[8*i+4] = verts[i+verts.size()/3].y;
			v[8*i+5] = verts[i+2*verts.size()/3].x;
			v[8*i+6] = verts[i+2*verts.size()/3].y;
			v[8*i+7] = verts[i+2*verts.size()/3].z;
		}
	}
}

glm::vec3 aw_t::getCenter(){
	glm::vec3 oc = glm::vec3(0.0,0.0,0.0);

	for(int i = 0; i < getSize(); i++){
		oc.x += verts[i].x;
		oc.y += verts[i].y;
		oc.z += verts[i].z;
	}

	oc.x /= verts.size();
	oc.y /= verts.size();
	oc.z /= verts.size();

	return oc;
}

int aw_t::getSize(){
	if(type == 0) return verts.size();
	else if (type == 3) return verts.size()/3;
	else return verts.size()/2;
}

std::vector<glm::vec3> aw_t::faceNormals(){
	std::vector<glm::vec3> f;
	for(int i = 0; i < getSize(); i += 3){
		f.emplace_back(glm::normalize(glm::cross(verts[i+2]-verts[i],verts[i+1]-verts[i])));
	}
	return f;
}

void aw_t::computeNormals(){
	std::vector<glm::vec3> f = faceNormals();
	int sizeConst = getSize();
	if (type | 2 != type){
		for(int i = 0; i < sizeConst; i++){
			glm::vec3 placeholder(0.,0.,0.);
			for(int j = 0; j < sizeConst; j++){
				if(verts[i] == verts[j]){
					placeholder += f[j/3];
				}
			}
			verts.emplace_back(glm::normalize(placeholder));
		}
		type += 2;
	}
	else return;
}

#endif
