#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
	vertexColor = vec4(1, 0, 0, 1);
	TexCoord = aTexCoord;

	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) * model * view * projection;
}
