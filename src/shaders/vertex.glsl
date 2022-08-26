#version 330 core

layout (location = 0) in vec4		aPos;
layout (location = 1) in vec4		aNormal;
layout (location = 2) in vec2		aTexCoord;

uniform mat4 	model;
uniform mat4 	view;
uniform mat4 	projection;

void main()
{
	gl_Position = aPos * model * view * projection;
}
