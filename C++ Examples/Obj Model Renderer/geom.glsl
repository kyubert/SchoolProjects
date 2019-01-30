#version 330 core

layout (location = 0) in; // position has attribute
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normals;
layout (line_strip, max_vertices = 36) out;

uniform mat4 transform;

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
	EmitVertex();

  gl_Position = vec4(position.x+normals.x, position.y+normals.y, position.z+normals.z, 1.0);
	EmitVertex();

	EndPrimative();
}
