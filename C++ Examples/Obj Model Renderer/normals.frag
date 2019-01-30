#version 330 core

out vec4 frag_color;

in vec4 vertex_color;

uniform vec4 color;                     // set this in OpenGL code

void main()
{
  frag_color = vertex_color;
}
