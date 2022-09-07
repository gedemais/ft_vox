#version 400 core

layout (location = 0) in vec3	aPosition;
layout (location = 1) in vec2	aTextCoord;
layout (location = 2) in int	aNormal;
layout (location = 3) in float	aType;

// uniform vec3					normals[5];

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

out vec3						vNormal;
out vec3						vPosition;
out vec2						vTextCoord;
flat out float					vType;

const vec3						normals[6] = vec3[6](
	vec3(0, 0, 1),
	vec3(0, 0, -1),
	vec3(0, 1, 0),
	vec3(0, -1, 0),
	vec3(1, 0, 0),
	vec3(-1, 0, 0)
);

void	main()
{
	vNormal		= normals[aNormal];
	vPosition	= aPosition;
	vTextCoord	= aTextCoord;
	vType		= aType;

	gl_Position	= vec4(aPosition, 1) * model * view * projection;
}
