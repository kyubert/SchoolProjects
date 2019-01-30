#version 330 core

layout (location = 0) in vec3 position; // position has attribute
layout (location = 1) in vec3 normals;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

uniform mat4 transform;

void main()
{
  gl_Position = transform*vec4(position.x, position.y, position.z, 1.0);

	vertex.normal = transform*vec4(normals.x, normals.y, normals.z, 1.0);

  vertex.color = vec4(0.5, 0.0, 0.0, 1.0);
}
