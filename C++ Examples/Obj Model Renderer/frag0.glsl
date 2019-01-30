#version 330 core

out vec4 frag_color;

in vec4 vert_color;

uniform vec4 color;                     // set this in OpenGL code

void main()
{
//frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//frag_color = vert_color;
  frag_color = color;
}
