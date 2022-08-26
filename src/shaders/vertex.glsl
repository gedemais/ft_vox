#version 330 core

layout (location = 0) in vec4	aPos;
layout (location = 1) in vec4	aNormal;
layout (location = 2) in vec2	aTexture;

uniform mat4 	model;
uniform mat4 	view;
uniform mat4 	projection;

out vec4	vNormal;
out vec2	vTexture;

void main()
{
	vNormal		= aNormal;
	vTexture	= aTexture;

	gl_Position	= aPos * model * view * projection;
}
