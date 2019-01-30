#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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

#define GLFW_INCLUDE_GLCOREARB  // to prevent GLFW from loading gl.h
#include <GLFW/glfw3.h>         // GL window context

#include "shader.h"

//Parts of the following code based on the example given at https://learnopengl.com/Getting-Started/Shaders
Shader::Shader(const char *vfile, const char *ffile){
  //create file streams to read from given files
  std::ifstream vfs;
  std::ifstream ffs;
  std::string vertex;
  std::string fragment;
  // ensure ifstream objects can throw exceptions:
  vfs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  ffs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try 
    {
      // open files
      vfs.open(vfile);
      ffs.open(ffile);
      std::stringstream vstring, fstring;
      // read file's buffer contents into streams
      vstring << vfs.rdbuf();
      fstring << ffs.rdbuf();		
      // close file handlers
      vfs.close();
      ffs.close();
      // convert stream into string
      vertex   = vstring.str();
      fragment = fstring.str();		
    }
  //check if files have been read
  catch(std::ifstream::failure e)
  {
      std::cerr << "FILE NOT READ" << std::endl;
  }
  //convert strings into cstrings
  const char* vc = vertex.c_str();
  const char* fc = fragment.c_str();

	int success;
	char infoLog[512];
  //creates vertex shader and saves to svo
  svo = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(svo, 1, &vc, NULL);
  glCompileShader(svo);
	// print compile errors if any
	glGetShaderiv(svo, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		  glGetShaderInfoLog(svo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
  //creates fragment shader and saves to sfo
  sfo = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(sfo, 1, &fc, NULL);
  glCompileShader(sfo);
	// print compile errors if any
	glGetShaderiv(sfo, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		  glGetShaderInfoLog(sfo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
  //creates program and saves to spo
  spo = glCreateProgram();
  glAttachShader(spo, svo);
  glAttachShader(spo, sfo);
  glLinkProgram(spo);
	// print linking errors
	glGetProgramiv(spo, GL_LINK_STATUS, &success);
	if(!success)
	{
		  glGetProgramInfoLog(spo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(const char *vfile, const char *gfile, const char *ffile){
  //create file streams to read from given files
  std::ifstream vfs;
  std::ifstream gfs;
  std::ifstream ffs;
  std::string vertex;
  std::string geometry;
  std::string fragment;
  // ensure ifstream objects can throw exceptions:
  vfs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  gfs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  ffs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  try 
    {
      // open files
      vfs.open(vfile);
      gfs.open(gfile);
      ffs.open(ffile);
      std::stringstream vstring, fstring, gstring;
      // read file's buffer contents into streams
      vstring << vfs.rdbuf();
      gstring << gfs.rdbuf();	
      fstring << ffs.rdbuf();		
      // close file handlers
      vfs.close();
      gfs.close();
      ffs.close();
      // convert stream into string
      vertex   = vstring.str();
      geometry = gstring.str();		
      fragment = fstring.str();		
    }
  //check if files have been read
  catch(std::ifstream::failure e)
  {
      std::cerr << "FILE NOT READ" << std::endl;
  }
  //convert strings into cstrings
  const char* vc = vertex.c_str();
  const char* gc = geometry.c_str();
  const char* fc = fragment.c_str();

	int success;
	char infoLog[512];
  //creates vertex shader and saves to svo
  svo = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(svo, 1, &vc, NULL);
  glCompileShader(svo);
	// print compile errors if any
	glGetShaderiv(svo, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		  glGetShaderInfoLog(svo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
  //creates geometry shader and saves to sgo
  sgo = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(sgo, 1, &gc, NULL);
  glCompileShader(sgo);
	// print compile errors if any
	glGetShaderiv(sgo, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		  glGetShaderInfoLog(sgo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
  //creates fragment shader and saves to sfo
  sfo = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(sfo, 1, &fc, NULL);
  glCompileShader(sfo);
	// print compile errors if any
	glGetShaderiv(sfo, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		  glGetShaderInfoLog(sfo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
  //creates program and saves to spo
  spo = glCreateProgram();
  glAttachShader(spo, svo);
  glAttachShader(spo, sgo);
  glAttachShader(spo, sfo);
  glLinkProgram(spo);
	// print linking errors
	glGetProgramiv(spo, GL_LINK_STATUS, &success);
	if(!success)
	{
		  glGetProgramInfoLog(spo, 512, NULL, infoLog);
		  std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(spo, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(spo, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
  glUniform1f(glGetUniformLocation(spo, name.c_str()), value);
}

#endif
