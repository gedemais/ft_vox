#version 330 core

layout (location = 0) in vec3	aPosition;
layout (location = 1) in vec2	aTexture;
layout (location = 2) in vec3	aNormal;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

out vec3						vNormal;
out vec3						vPosition;
out vec2						vTexture;

void	main()
{
	vec4	pos	= vec4(aPosition, 1);

	vNormal		= aNormal * mat3(transpose(inverse(model)));
	vPosition	= vec3(pos * model);
	vTexture	= aTexture;

	gl_Position	= pos * model * view * projection;
}
