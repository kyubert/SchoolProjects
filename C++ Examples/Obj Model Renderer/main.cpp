#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <thread>       // std::thread (C++11)
#include <cmath>        // math
#include <vector>
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLFW_INCLUDE_GLCOREARB  // to prevent GLFW from loading gl.h
#include <GLFW/glfw3.h>         // GL window context

#include "shader.h"
#include "aw_t.h"

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void key_callback(GLFWwindow *window,int key,int scancode,int action,int mods)
{
  if(key == GLFW_KEY_W && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if(key == GLFW_KEY_F && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
  glViewport(0,0,w,h);
}

int main()
{
        GLFWwindow* window = NULL;

  // init GLFW library
  if(!glfwInit())
    return -1;

  // set up window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	int WIDTH = 800, HEIGHT = 600;

  // windowed instead of full-screen
  if(!(window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr))) {
    glfwTerminate();
    return -1;
  }

  // callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // make window's context current
  glfwMakeContextCurrent(window);

	// depth test so texture doesn't look screwy
	glEnable(GL_DEPTH_TEST);

  // GLSL shaders
  Shader shader("normals.vert", "normals.geom", "normals.frag");

//std::this_thread::sleep_for(std::chrono::seconds(1));

	aw_t object("bunny.obj");

	object.computeNormals();

  GLfloat verts[object.verts.size()*3];

	object.pack(verts);

  GLuint indices[object.vi.size()];

	std::copy(object.vi.begin(), object.vi.end(), indices);
/*
  GLfloat verts[] = {
     0.5f,  0.5f, 0.0f, // top right
     0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f  // top left
  };

  GLuint indices[] = {
     0, 1, 3,           // first tri
     1, 2, 3            // second tri
  };*/

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  // set up vertex array object
  GLuint vao;
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  // set up vertex buffer object
  GLuint vbo;
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // transfer (copy) verts to GPU
  // GL_STATIC_DRAW: data will most likely not change at all or very rarely
  // GL_DYNAMIC_DRAW: data is likely to change often
  // GL_STREAM_DRAW: data will every time it is drawn
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // link vertex attributes, let GL know how to use vbo
  //      index:               0: layout (location = 0)
  //       size:               3: size of vertex attribute (vec3)
  //       type:        GL_FLOAT: type of verts
  // normalized:        GL_FALSE: don't normalize
  //     stride: 3*sizeof(float): bits per attr
  //     offset:        (void*)0: offset into array

  // using named in vec3 position
	if (object.getType() == 0){
		const GLint location(glGetAttribLocation(shader.getID(),"position"));
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
		glEnableVertexAttribArray(location);
	}
	else if (object.getType() == 1){
		const GLint location(glGetAttribLocation(shader.getID(),"position"));
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
		glEnableVertexAttribArray(location);
		const GLint texAttrib = glGetAttribLocation(shader.getID(), "texture");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(texAttrib);
	}
	else if (object.getType() == 2){
		const GLint location(glGetAttribLocation(shader.getID(),"position"));
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
		glEnableVertexAttribArray(location);
		const GLint normals(glGetAttribLocation(shader.getID(),"normals"));
		glVertexAttribPointer(normals, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(normals);
	}
	else if (object.getType() == 3){
		const GLint location(glGetAttribLocation(shader.getID(),"position"));
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
		glEnableVertexAttribArray(location);
		const GLint texAttrib = glGetAttribLocation(shader.getID(), "texture");
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(texAttrib);
		const GLint normals(glGetAttribLocation(shader.getID(),"normals"));
		glVertexAttribPointer(normals, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5*sizeof(float)));
		glEnableVertexAttribArray(normals);
	}

	GLuint texture;
	if (object.getType() % 2 == 1){

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char *data = stbi_load("texture.png", &width, &height, &nrChannels, 0);
		if (data)
		{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
				std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	}

	shader.use();
	if(object.getType() % 2 == 1) shader.setInt("texture", 0);

  // loop until user closes window
  while(!glfwWindowShouldClose(window)) {

    // check for key input
    processInput(window);

    // render here
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (object.getType() % 2 == 1){
		  // bind textures on corresponding texture units
		  glBindTexture(GL_TEXTURE_2D, texture);
		}

		//transform to rotate the object
		glm::mat4 t1, t2, t3, r, p, trans;
		t1 = glm::translate(trans, -object.getCenter());
		t2 = glm::translate(trans, object.getCenter());
		t3 = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
		r = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));
		//ortho ratio: -width/height, width/height
		p = glm::ortho(-WIDTH/HEIGHT, WIDTH/HEIGHT, -1, 1, 1, -1);
		trans = p * t3 * t2 * r * t1;

    shader.use();
    unsigned int transformLoc = glGetUniformLocation(shader.getID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		if (object.getType() % 2 == 0){
		  // set variables used in shader
		  const GLint location(glGetUniformLocation(shader.getID(),"color"));
		  float time = glfwGetTime(); // time in seconds
		  GLfloat green = (sin(time) / 2.0f) + 0.5f;
		  glUniform4f(location, 0.0f, green, 0.0f, 1.0f);
		}

    glBindVertexArray(vao);
    // render rectangle from indeces
    // see: https://learnopengl.com/Getting-started/Hello-Triangle
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawArrays(GL_TRIANGLES, 0, object.getSize());
    glBindVertexArray(0);

    // swap buffers
    glfwSwapBuffers(window);

    // poll for events
    glfwPollEvents();

  }

  glfwTerminate();
  return 0;
}
