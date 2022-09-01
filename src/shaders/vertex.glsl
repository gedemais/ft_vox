#version 330 core

layout (location = 0) in vec3	aPosition;
layout (location = 1) in vec2	aTexture;
layout (location = 2) in uint	aNormal;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

out vec3						vNormal;
out vec3						vPosition;
out vec2						vTexture;

/*
	front / back : 0 / 1
	top / bottom : 2 / 3
	right / left : 4 / 5
*/

vec3	get_normal(uint aNormal)
{
	vec3	n = vec3(0);

	switch (aNormal) {
		case (0):
			n = vec3(0, 0, 1);
			break ;
		case (1):
			n = vec3(0, 0, -1);
			break ;
		case (2):
			n = vec3(0, 1, 0);
			break ;
		case (3):
			n = vec3(0, -1, 0);
			break ;
		case (4):
			n = vec3(1, 0, 0);
			break ;
		case (5):
			n = vec3(-1, 0, 0);
			break ;
	}
	return (n);
}

void	main()
{
	vec4	pos	= vec4(aPosition, 1);

	vNormal		= get_normal(aNormal) * mat3(transpose(inverse(model)));
	vPosition	= vec3(pos * model);
	vTexture	= aTexture;

	gl_Position	= pos * model * view * projection;
}
