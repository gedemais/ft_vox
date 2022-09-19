# include "main.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


static int	switch_block_type(unsigned int z)
{
	if (z < 64)
		return (BT_STONE); // Water
	else if (z < 72)
		return (BT_GROUND); //Grass
	else if (z < 80)
		return (BT_GRASS);
	else if (z < 88)
		return (BT_STONE);
	else
		return (BT_SNOW);
	/*else if (z < 72)
		return (BT_WATER); // Water
	else if (z < 80)
		return (BT_SAND); // Water
	else if (z < 88)
		return (BT_GROUND); // Sand
	else if (z < 96)
		return (BT_GRASS); // Sand
	else if (z < 104)
		return (BT_STONE); // Grass*/
}

static unsigned char	push_plane(t_chunk *chunk, vec3 plane[6], uint8_t normal, unsigned int y, float fall_size, bool side)
{
	t_stride		vertex;
	uint8_t			block_type;
	const t_vt	vts[6] = {
						(t_vt){1, fall_size}, // A
						(t_vt){1, 0}, // B
						(t_vt){0, 0}, // C

						(t_vt){1, fall_size}, // D
						(t_vt){0, 0}, // E
						(t_vt){0, fall_size}, // F
						};

	// Addition of 6 vertexs plane to the mesh's data stride
	for (unsigned int i = 0; i < 6; i++)
	{
		ft_memset(&vertex, 0, sizeof(t_stride));

		block_type = switch_block_type(y);
		(void)side;
		//block_type = (block_type == BT_GRASS && side) ? BT_GRASS_SIDE : block_type;
		//block_type = (block_type == BT_SNOW && side) ? BT_SNOW_SIDE : block_type;

		// Constuction of the vertex
		vertex = (t_stride){(short)plane[i].x, 
							(short)plane[i].y,
							(short)plane[i].z,
							(uint8_t)i,
							(uint8_t)((int)fall_size),
							(uint8_t)normal,
							(uint8_t)block_type,
							(uint8_t)0};

		// Insertion of the vertex in the stride
		if (dynarray_push(&chunk->stride, &vertex, false))
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

unsigned char	generate_top_plane(t_chunk *chunk, int x, int y, int z, vec3 top_plane[6])
{
	float			xx, yy, zz;
	vec3			a, b, c, d;

	xx = x;
	yy = y;
	zz = z;

	// Cube's top plane ABCD points
	a = (vec3){xx, yy, (z + 1)};
	b = (vec3){(x + 1), yy, (z + 1)};
	c = (vec3){xx, yy, zz};
	d = (vec3){(x + 1), yy, zz};

	top_plane[0] = c;
	top_plane[1] = a;
	top_plane[2] = b;
	top_plane[3] = c;
	top_plane[4] = b;
	top_plane[5] = d;

	return (push_plane(chunk, top_plane, N_UP, y, 1.0f, false));
}

unsigned char	generate_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int y, float depth)
{
	vec3		c, d;

	c = vec_add(a, vec_fmult((vec3){0, -1.0f, 0}, depth));
	d = vec_add(b, vec_fmult((vec3){0, -1.0f, 0}, depth));

	vec3 side_plane[6] = {c, a, b, c, b, d};

	return (push_plane(chunk, side_plane, index, y, depth, true));
}

static unsigned char	generate_deep_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int offset, unsigned int y)
{
	unsigned char	code;
	uint8_t			prev_bt;
	unsigned int	n = 0;
	float			fall_size = 0.0f;

	while (n < offset)
	{
		fall_size = 0.0f;
		prev_bt = switch_block_type(y - n);
		while (n < offset && prev_bt == switch_block_type(y - n))
		{
			fall_size += 1.0f;
			n++;
		}

		if (n < offset)
			fall_size += (float)(offset - n);

		if ((code = generate_fall(chunk, a, b, index, y, fall_size)))
			return (code);
	}
	return (ERR_NONE);
}

unsigned char	generate_side_plane(t_chunk *chunk, uint8_t **hmap, int x, int y, int z, unsigned int size, vec3 top_plane[6])
{
	unsigned char	code;
	const int		neighbours[4][2] =	{{0, 1},
										 {0, -1},
										 {1, 0},
										 {-1, 0}};
	// Based on cbacbd order
	const uint8_t	fall_planes[4][2] =	{{2, 1},
										 {0, 5},
										 {2, 5},
										 {1, 0}};
	vec3			a, b;
	int				n_x, n_z;
	int				offset;

	for (unsigned int i = 0; i < PO_MAX; i++)
	{
		n_x = x + neighbours[i][0];
		n_z = z + neighbours[i][1];

	//	if (n_x < 0 || n_x >= (int)size || n_z < 0 || n_z >= (int)size)
	//		(void)n_x;
	//	else
	//		printf("%d %d : %d (y = %d)\n", n_x, n_z, hmap[n_x][n_z], y);
	
		if (n_x < 0 || n_x >= (int)size || n_z < 0 || n_z >= (int)size
			|| (offset = y - hmap[n_x][n_z]) <= 0)
			continue ;

		a = top_plane[fall_planes[i][0]];
		b = top_plane[fall_planes[i][1]];

		if ((offset == 1 && (code = generate_fall(chunk, a, b, i, y, 1.0f)))
			|| (code = generate_deep_fall(chunk, a, b, i, offset, y)))
			return (code);
	}
	return (ERR_NONE);
}

unsigned char	generate_bottom_plane(t_chunk *chunk, int x, int y, int z, vec3 top_plane[6])
{
	(void)chunk;
	(void)x;
	(void)y;
	(void)z;
	(void)top_plane;
	return (ERR_NONE);
}
