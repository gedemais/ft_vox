#version 330 core

layout (location = 0) in vec3	aPosition;
layout (location = 1) in vec2	aTexture;
layout (location = 2) in uint	tid;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

out vec2						vTexture;

void	main()
{
	vTexture = aTexture;

	gl_Position = vec4(aPosition, 1) * model * view * projection;
}
