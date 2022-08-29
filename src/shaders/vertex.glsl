#version 330 core

layout (location = 0) in vec4	aPosition;
layout (location = 1) in vec2	aTexture;
layout (location = 3) in int	aNormal;

uniform mat4 	model;
uniform mat4 	view;
uniform mat4 	projection;

out vec4	vNormal;
out vec4	vPosition;
out vec2	vTexture;

vec4		get_normal(int aNormal)
{
	vec4	normal;

	switch (aNormal) {
		case (0): // front
			normal = vec4(0, 0, 1, 1);
			break;
		case (1): // back
			normal = vec4(0, 0, -1, 1);
			break;
		case (2): // top
			normal = vec4(0, 1, 0, 1);
			break;
		case (3): // bottom
			normal = vec4(0, -1, 0, 1);
			break;
		case (4): // right
			normal = vec4(1, 0, 0, 1);
			break;
		case (5): // left
			normal = vec4(-1, 0, 0, 1);
			break;
	}
	return (normal);
}

void		main()
{
	vNormal		= get_normal(aNormal);
	vPosition	= aPosition;
	vTexture	= aTexture;

	gl_Position	= aPosition * model * view * projection;
}
