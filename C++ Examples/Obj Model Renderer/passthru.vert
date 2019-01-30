#version 330 core
in vec4 position;
in vec4 normal;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

void main()
{
  gl_Position = position;
  vertex.normal = normal;
  vertex.color =  vec4(1.0, 1.0, 0.0, 1.0);
}
