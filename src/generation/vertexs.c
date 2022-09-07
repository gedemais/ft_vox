# include "main.h"

enum	e_side_orientation
{
	PO_UP,
	PO_DOWN,
	PO_NORTH,
	PO_SOUTH,
	PO_EST,
	PO_WEST,
	PO_MAX
};

const float		block_size = 1.0f;

const vec3	orientations[PO_MAX] = {{0, 0, block_size},
									{0, 0, -block_size},
									{block_size, 0, 0},
									{-block_size, 0, 0}};


static int	switch_block_type(unsigned int z)
{
	if (z < 20)
		return (BT_ROCK);
	else if (z >= 20 && z < 40)
		return (BT_SAND);
	else if (z >= 40 && z < 60)
		return (BT_GROUND);
	else if (z >= 60 && z < 80)
		return (BT_GRASS);
	else
		return (BT_SNOW);
}

static unsigned char	push_plane(t_chunk *chunk, vec3 plane[6], uint8_t normal, unsigned int z, float fall_size)
{
	t_stride		vertex;
	const t_vt	vts[6] = {  (t_vt){0, 0},
							(t_vt){fall_size, 0},
							(t_vt){0, fall_size},
							(t_vt){0, fall_size},
							(t_vt){fall_size, 0},
							(t_vt){fall_size, fall_size}  };

	// Addition of 6 vertexs plane to the mesh's data stride
	for (unsigned int i = 0; i < 6; i++)
	{
		// Constuction of the vertex
		printf("%f %f %f | %f %f | %d | %d\n", plane[i].x, plane[i].y, plane[i].z, vts[i].u, vts[i].v, normal, (uint8_t)switch_block_type(z));
		vertex = (t_stride){plane[i], vts[i], normal, (uint8_t)switch_block_type(z)};
		// Insertion of the vertex in the stride
		if (dynarray_push(&chunk->stride, &vertex, false))
			return (ERR_MALLOC_FAILED);
	}
	printf("-----------------------\n");
	return (ERR_NONE);
}

unsigned char	generate_top_plane(t_chunk *chunk, int x, int y, int z,
									int x_start, int y_start, vec3 top_plane[6])
{
	float			xx, yy, zz;
	vec3			a, b, c, d;

	xx = (x_start + x) * block_size;
	yy = (y_start + y) * block_size;
	zz = z * block_size;

	// Cube's top plane ABCD points
	a = (vec3){xx, yy, (z + 1) * block_size};
	b = (vec3){(x_start + x + 1) * block_size, yy, (z + 1) * block_size};
	c = (vec3){xx, yy, zz};
	d = (vec3){(x_start + x + 1) * block_size, yy, zz};

	top_plane[0] = c;
	top_plane[1] = a;
	top_plane[2] = b;
	top_plane[3] = c;
	top_plane[4] = b;
	top_plane[5] = d;

	return (push_plane(chunk, top_plane, N_UP, z, 1.0f));
}

static unsigned char	generate_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int z, float depth)
{
	vec3		c, d;

	c = vec_add(a, vec_fmult(orientations[index], depth));
	d = vec_add(b, vec_fmult(orientations[index], depth));

	vec3 side_plane[6] = {c, a, b, c, b, d};

	return (push_plane(chunk, side_plane, index, z, depth));
}

static unsigned char	generate_deep_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int offset, unsigned int z)
{
	unsigned char	code;
	uint8_t			prev_bt;
	unsigned int	n = 0;
	float			fall_size = 0.0f;

	while (n < offset)
	{
		fall_size = 0;
		prev_bt = switch_block_type(z + n);
		while (n < offset && prev_bt == switch_block_type(z + n))
		{
			fall_size += 1.0f;
			n++;
		}
		if ((code = generate_fall(chunk, a, b, index, z, fall_size)))
			return (code);
	}
	return (ERR_NONE);
}

unsigned char	generate_side_plane(t_chunk *chunk, int x, int y, int z, unsigned int size, vec3 top_plane[6])
{
	unsigned char	code;
	const int		neighbours[4][2] =	{{0, 1},
										 {0, -1},
										 {1, 0},
										 {-1, 0}};
	// Based on cbacbd order
	const uint8_t	fall_planes[4][2] =	{{2, 1},
										 {0, 5},
										 {2, 0},
										 {1, 5}};
	vec3			a, b;
	int				n_x, n_y;
	int				offset;

	for (unsigned int i = 0; i < PO_MAX; i++)
	{
		n_x = x + neighbours[i][0];
		n_y = y + neighbours[i][1];

		if (n_x < 0 || n_x >= (int)size || n_y < 0 || n_y >= (int)size
			|| (offset = z - chunk->surface_hmap[n_x][n_y]) <= 0)
			continue ;

		a = top_plane[fall_planes[i][0]];
		b = top_plane[fall_planes[i][1]];

		if ((offset == 1 && (code = generate_fall(chunk, a, b, i, z, 1.0f)))
			|| (code = generate_deep_fall(chunk, a, b, i, offset, z)))
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
