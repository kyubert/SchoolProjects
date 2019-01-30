#version 330 core

out vec4 frag_color;
in vec2 tex;

uniform sampler2D stexture;

void main()
{
	frag_color = texture(stexture, tex);
}
