#version 330 core

layout (location = 0) in vec3 position; // position has attribute
layout (location = 1) in vec2 texture;

out vec2 tex;                    // output to fragment shader

uniform mat4 transform;

void main()
{
  gl_Position = transform*vec4(position.x, position.y, position.z, 1.0);

  tex = vec2(texture.x, texture.y);
}
