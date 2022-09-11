#version 400 core

layout (location = 0) in uint	data1;
layout (location = 1) in uint	data2;

uniform mat4 					model;
uniform mat4 					view;
uniform mat4 					projection;

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
	return (normalize(normal));
}

vec2	get_uv(int n, float fall_size)
{
	vec2	uv = vec2(0);

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

int	get_int_from_bits(uint data1, uint data2, int start, int size)
{
	int			bound = start + size;
	int			ret = 0;
	int			pow = 1;

	for (int i = bound - 1; i >= start; i--)
	{
		if (bool(((i < 32 ? data1 : data2) >> i) & 1))
			ret += pow;

		pow *= 2;
	}
	return (ret);
}

void	main()
{
	vec3		pos;
	int			normal_id;
	int			uv_id;
	float		fall_size;
	int			block_type;

	//pos.x = float(int(data1 >> 16));
	//pos.y = float(int(data1 & 0xffff));

	pos.x = float(get_int_from_bits(data1, data2, 0, 16));
    pos.y = float(get_int_from_bits(data1, data2, 16, 16));
    pos.z = float(get_int_from_bits(data1, data2, 32, 16));
	uv_id = get_int_from_bits(data1, data2, 48, 3);
	fall_size = float(get_int_from_bits(data1, data2, 51, 6));
	normal_id = get_int_from_bits(data1, data2, 57, 3);
	block_type = get_int_from_bits(data1, data2, 60, 3);

	vPosition	= pos;
	vNormal		= get_normal(normal_id);
	vTextCoord	= get_uv(uv_id, fall_size);
	vType		= block_type;

	gl_Position	= vec4(pos, 1) * model * view * projection;
}
