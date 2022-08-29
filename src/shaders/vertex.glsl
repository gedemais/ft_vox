#version 330 core

layout (location = 0) in vec4	aPosition;
layout (location = 1) in vec2	aTexture;
layout (location = 2) in int	aNormal;
layout (location = 3) in int	tid;

uniform mat4 	model;
uniform mat4 	view;
uniform mat4 	projection;

out vec4	vNormal;
out vec4	vPosition;
out vec2	vTexture;

out vec4	vColor;

vec4		get_normal(int aNormal)
{
	vec4	normal;

	switch (aNormal) {
		case (0): // front
			normal = vec4(0, 0, -1, 1);
			break;
		case (1): // back
			normal = vec4(0, 0, 1, 1);
			break;
		case (2): // top
			normal = vec4(0, -1, 0, 1);
			break;
		case (3): // bottom
			normal = vec4(0, 1, 0, 1);
			break;
		case (4): // right
			normal = vec4(-1, 0, 0, 1);
			break;
		case (5): // left
			normal = vec4(1, 0, 0, 1);
			break;
	}
	return (normal);
}

vec4		get_color(int aNormal)
{
	vec4	color;

	switch (aNormal) {
		case (0):
			color = vec4(0, 0, 0, 1);
			break ;
		case (1):
			color = vec4(1, 1, 1, 1);
			break ;
		case (2):
			color = vec4(1, 0, 0, 1);
			break ;
		case (3):
			color = vec4(0, 0, 1, 1);
			break ;
		case (4):
			color = vec4(0, 1, 0, 1);
			break ;
		case (5):
			color = vec4(1, 0, 1, 1);
			break ;
		default:
			color = vec4(1, 0, 0, 1);
			break ;
	}
	return (color);
}

void		main()
{
	vNormal		= get_normal(aNormal);
	vPosition	= aPosition;
	vTexture	= aTexture;

	vColor		= get_color(aNormal);

	gl_Position	= aPosition * model * view * projection;
}
