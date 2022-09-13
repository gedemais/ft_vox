#version 400 core

layout (location = 0) in int	data1;
layout (location = 1) in int	data2;

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
			normal = vec3(-1, 0, 0);
			break ;
		case (3):
			normal = vec3(1, 0, 0);
			break ;
		case (4):
			normal = vec3(0, 1, 0);
			break ;
		case (5):
			normal = vec3(0, -1, 0);
			break ;
	}
	return (normal);
}

vec2	get_uv(int n, float fall_size)
{
	vec2	uv;

	switch (n) {
		case (0):
			uv = vec2(1, fall_size);
			break ;
		case (1):
			uv = vec2(1, 0);
			break ;
		case (2):
			uv = vec2(0, 0);
			break ;
		case (3):
			uv = vec2(1, fall_size);
			break ;
		case (4):
			uv = vec2(0, 0);
			break ;
		case (5):
			uv = vec2(0, fall_size);
			break ;
	}
	return (uv);
}

void	main()
{
	vec4		pos;
	uint		normal_id;
	uint		uv_id;
	float		fall_size;
	uint		block_type;

	//////// Bitfield unpacking ///////////
	pos.x = float(data1 & 0xFFFF);
	pos.y = float((data1 & 0xFFFF0000) >> 16u);
	pos.z = float(data2 & 0xFFFF);
	pos.w = 1.0f;

	uv_id = ((data2 & 0x70000u) >> 16u);
	fall_size = float((data2 & 0x1F80000u) >> 19u);
	normal_id = ((data2 & 0xE000000u) >> 25u);
	block_type = ((data2 & 0x70000000u) >> 29u);
	//////////////////////////////////////

	// Water ripples
	vType		= block_type;
	if (vType == 0) {
		float	time, wavelength;

		time		= u_time * 0.05f;
		wavelength	= 0.1f;
		pos.y		+= (sin(pos.x * time) * cos(pos.y * time)) * wavelength + 0.5f;
	}

	// Output assignations
	vTextCoord	= get_uv(int(uv_id), fall_size);

	vNormal		= mat3(transpose(inverse(model))) * get_normal(int(normal_id));
	//vNormal		= get_normal(int(normal_id));

	vPosition	= vec3(pos * model);
	//vPosition	= pos;

	gl_Position	= pos * model * view * projection;
}
