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
	int			shift = 0;
	int			ret = 0;
	int			pow = 1;

	for (int i = start; i < bound; i++)
	{
		shift = (i < 32) ? i : i - 32;
		if (bool(((i < 32 ? data1 : data2) >> shift) & 1))
			ret += pow;

		pow *= 2;
	}
	return (ret);
}

void	main()
{
	vec3		pos;
	uint			normal_id;
	uint			uv_id;
	float		fall_size;
	uint			block_type;

	pos.x = float(data1 & 0xFFFFu);
    pos.y = float((data1 & 0xFFFFFFFFu) >> 16u);
    pos.z = float(data2 & 0xFFFFu);
	uv_id = int((data2 & 0x70000u) >> 16u);
	fall_size = float((data2 & 0x1F80000u >> 19u));
	normal_id = (data2 & 0xE000000u >> 25u);
	block_type = (data2 & 0x70000000u >> 29u);

	vPosition	= pos;
	vNormal		= get_normal(int(normal_id));
	vTextCoord	= get_uv(int(uv_id), fall_size);
	vType		= block_type;

	gl_Position	= vec4(pos, 1) * model * view * projection;
}
