#version 400 core

layout (location = 0) in vec3	aPosition;
layout (location = 1) in vec2	aTextCoord;
layout (location = 2) in float	aNormal;
layout (location = 3) in float	aType;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;
uniform float 					u_time;

out vec3						vNormal;
out vec3						vPosition;
out vec2						vTextCoord;
flat out float					vType;

vec3	get_normal(int n)
{
	vec3	normal = vec3(0);

	switch (n) {
		case (0):
			normal = vec3(0, 0, 1);
			break ;
		case (1):
			normal = vec3(0, 0, -1);
			break ;
		case (2):
			normal = vec3(0, 1, 0);
			break ;
		case (3):
			normal = vec3(0, -1, 0);
			break ;
		case (4):
			normal = vec3(1, 0, 0);
			break ;
		case (5):
			normal = vec3(-1, 0, 0);
			break ;
	}
	return (normal);
}

void	main()
{
	vec4	pos	= vec4(aPosition, 1);

	if (int(aType) == 0) {
		float	time, wavelength;

		time		= u_time * 0.05f;
		wavelength	= 0.1f;
		pos.y		+= (sin(pos.x * time) * cos(pos.y * time)) * wavelength;
	}

	vNormal		= mat3(transpose(inverse(model))) * get_normal(int(aNormal));
	//vNormal		= get_normal(int(aNormal));

	vPosition	= vec3(pos * model);
	//vPosition	= aPosition;

	vTextCoord	= aTextCoord;
	vType		= aType;

	gl_Position	= pos * model * view * projection;
}
