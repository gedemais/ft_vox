# include "main.h"

const t_vt	top_vt[6] = { (t_vt){0, 0}, (t_vt){0, 1}, (t_vt){1, 1}, (t_vt){0, 0}, (t_vt){1, 1}, (t_vt){1, 0} };

static unsigned char	generate_top_plane(t_chunk *chunk, int x, int y, int z, int x_start, int y_start)
{
	const float	size = 1.0f;

	t_stride		stride;
	vec3			a, b, c, d;

	float	xx = (x_start + x) * size;
	float	yy = (y_start + y) * size;
	float	zz = z * size;

	// Cube's top plane ABCD points
	a = (vec3){xx, yy, (z + 1) * size, 1};
	b = (vec3){(x_start + x + 1) * size, yy, (z + 1) * size, 1};
	c = (vec3){xx, yy, zz, 1};
	d = (vec3){(x_start + x + 1) * size, yy, zz, 1};

	// Triangles construction (clockwise)
	const vec3	top_plane[6] = {c, a, b, c, b, d};

	// Addition of 6 vertexs plane to the mesh's data stride
	for (unsigned int i = 0; i < 6; i++)
	{
		// Constuction of data stride
		stride = (t_stride){top_plane[i].x, top_plane[i].y, top_plane[i].z, top_vt[i], (uint8_t)N_UP, (uint8_t)chunk->block_map[x][y][z].type};
		if (dynarray_push(&chunk->stride, &stride, false))
			return (ERR_MALLOC_FAILED);
	}

	return (ERR_NONE);
}

static unsigned char	generate_falls_planes(t_chunk *chunk, int x, int y, int z, int x_start, int y_start)
{
	(void)chunk;
	(void)x;
	(void)y;
	(void)z;
	(void)x_start;
	(void)y_start;
	return (ERR_NONE);
}

// This function will write the data stride by converting t_block 3D array to vertexs,
// normals, uv, and texture_id.

static unsigned char	generate_data_stride(t_chunk *chunk, int x_start, int y_start, unsigned int size)
{
	t_stride		new;
	unsigned char	code;

	if (dynarray_init(&chunk->stride, sizeof(t_stride), size * size))
		return (ERR_MALLOC_FAILED);

	for (unsigned int x = 0; x < size; x++)
		for (unsigned int y = 0; y < size; y++)
			for (unsigned int z = 0; z < size; z++)
			{
				// if (chunk->block_map[x][y][z].type > BT_MAX)
				// unified matrix of planes -> special treat
				if (chunk->block_map[x][y][z].type != BT_NONE
					&& chunk->block_map[x][y][z].type != BT_AIR
					&& ((code = generate_top_plane(chunk, x, y, z, x_start, y_start)) != ERR_NONE
						|| (code = generate_falls_planes(chunk, x, y, z, x_start, y_start)) != ERR_NONE))
					return (code);
			}

	return (ERR_NONE);
}

static unsigned char	generate_chunk_content(t_chunk *chunk, int x_start, int y_start, unsigned int size)
{
	unsigned char	code;

	// Generate height map for surface and cave
	// Topography type should be a parameter which would affect perlin noise generation
	if (!(chunk->surface_hmap = generate_height_map(x_start, y_start, size))
		|| !(chunk->sub_hmap = generate_height_map(x_start, y_start, size)))
		return (ERR_MALLOC_FAILED);

//	if ((code = greedy_meshing(chunk, x_start, y_start, size)))
//		return (code);

	// Generate 3D block map
	if  ((code = gen_block_map(chunk, x_start, y_start, size)))
		return (code);

	return (ERR_NONE);
}

// This function will be used to generate new chunks of terrain with different noise algorithms

unsigned char			gen_chunk(t_env *env, int x_start, int y_start, unsigned int size)
{
	t_chunk			chunk;
	unsigned char	code;

	if ((code = generate_chunk_content(&chunk, x_start, y_start, size)) != ERR_NONE)
		return (code);

	if ((code = generate_data_stride(&chunk, x_start, y_start, size)))
		return (code);

	printf("%d\n", chunk.stride.nb_cells);

	if ((env->model.chunks.arr == NULL
		&& dynarray_init(&env->model.chunks, sizeof(t_chunk), 64))
		|| dynarray_push(&env->model.chunks, &chunk, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}
