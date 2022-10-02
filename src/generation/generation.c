# include "main.h"

static unsigned int		check_neighbours(t_chunk *chunk, unsigned char neighbours[6],
																					unsigned int x,
																					unsigned int y,
																					unsigned int z)
{
	// neighbours coordinates
	const int		n[6][3] =	{
											[N_NORTH] = {0, 0, 1},
											[N_SOUTH] = {0, 0, -1},
											[N_WEST] = {-1, 0, 0},
											[N_EAST] = {1, 0, 0},
											[N_UP] = {0, 1, 0},
											[N_DOWN] = {0, -1, 0}
								};
	unsigned int	index = 0;

	ft_memset(neighbours, 0, sizeof(unsigned char) * 6);
	for (unsigned int i = 0; i < N_MAX; i++)
	{
		x += n[i][0];
		y += n[i][1];
		z += n[i][2];

		if (x >= CHUNK_SIZE
			|| y >= CHUNK_SIZE
			|| z >= CHUNK_SIZE)
			continue;

		if (chunk->cave_map[x][y][z] == BT_STONE)
			neighbours[index++] = (unsigned char)i;
	}
	return (index);
}

static unsigned char	generate_cave_vertexs(t_chunk *chunk, int x_start, int z_start)
{
	vec3			top_plane[6] = {};
	unsigned char	neighbours[N_MAX] = {};
	unsigned char	code;

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int y = 0; y < CHUNK_SIZE; y++)
			for (unsigned int z = 0; z < CHUNK_SIZE; z++)
			{
				if (chunk->cave_map[x][y][z] == 0 && check_neighbours(chunk, neighbours, x, y, z) > 0
					&& (code = generate_cave_plane(chunk, neighbours, x, y, z, x_start, z_start)))
					return (code);
			}

	return (ERR_NONE);
}

static unsigned char	generate_surface_vertexs(t_chunk *chunk, int x_start, int z_start)
{
	unsigned int	y;
	vec3			top_plane[6];
	unsigned char	code;

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
		{
			y = chunk->surface_hmap[x][z];

			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane)) != ERR_NONE
				|| (code = generate_side_plane(chunk, chunk->surface_hmap, x, y, z, CHUNK_SIZE, top_plane)) != ERR_NONE)
				return (code);
		}
	return (ERR_NONE);
}

unsigned char	generate_vertexs(t_chunk *chunk, int x_start, int z_start)
{
	unsigned char	code;

	if ((code = generate_surface_vertexs(chunk, x_start, z_start))
		|| (code = generate_cave_vertexs(chunk, x_start, z_start)))
		return (code);
	return (ERR_NONE);
}

static unsigned char	generate_chunk_content(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride)
{
	unsigned char	code;
	t_biome_params	surface_params = {0.02f, 6.0f, 0.0f, 0.0f};
	t_biome_params	sub_params = {0.05f, 6.0f, 0.0f, 40.0f};

	chunk->x_start = x_start;
	chunk->z_start = z_start;

	(void)env;
	if (!chunk->surface_hmap || !chunk->cave_map)
	{
		// Load parameters for the current chunk
		//printf("frequency : %f | depth : %f\n", params.frequency, params.depth);
		// Generate height maps for surface and cave
		// Topography type should be a parameter which would affect perlin noise generation
		if (!(chunk->surface_hmap = generate_height_map(surface_params, x_start, z_start, CHUNK_SIZE, 40))
			|| (code = generate_cave_map(chunk, CHUNK_SIZE)))
			return (ERR_MALLOC_FAILED);
	}

	if (stride)
	{
		if (stride && dynarray_init(&chunk->stride, sizeof(t_stride), CHUNK_SIZE * CHUNK_SIZE * 6 * 2))
			return (ERR_MALLOC_FAILED);

		if (stride && (code = generate_vertexs(chunk, x_start, z_start)) != ERR_NONE)
			return (code);
	}

	return (ERR_NONE);
}

// This function will be used to generate new chunks of terrain with different noise algorithms

size_t	*stride_bytesize(void)
{
	static size_t	bytesize = 0;

	return (&bytesize);
}

unsigned char			gen_chunk(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride)
{
	unsigned char	code;

	if ((code = generate_chunk_content(env, chunk, x_start, z_start, stride)) != ERR_NONE)
		return (code);

	*stride_bytesize() += sizeof(t_stride) * chunk->stride.nb_cells;
	return (ERR_NONE);
}
