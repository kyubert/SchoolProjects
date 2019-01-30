#version 330
layout(triangles) in;

// 18 lines will be generated: 36 vertices
layout(line_strip, max_vertices=36) out;

uniform mat4 transform;

in Vertex
{
  vec4 normal;
  vec4 color;
} vertex[];

out vec4 vertex_color;

void main()
{
  int i;
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal.xyz;
    
    gl_Position = transform * vec4(P, 1.0);
    vertex_color = vertex[i].color;
    EmitVertex();
    
    gl_Position = transform * vec4(P + (N * 0.2), 1.0);
    vertex_color = vertex[i].color;
    EmitVertex();
    
    EndPrimitive();
  }
}
