# include "main.h"

unsigned char	generate_vertexs(t_chunk *chunk, int x_start, int z_start)
{
	unsigned char	code;
	unsigned int	y;
	vec3			top_plane[6];

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
		{
			y = chunk->surface_hmap[x][z];

			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane)) != ERR_NONE
				|| (code = generate_side_plane(chunk, chunk->surface_hmap, x, y, z, CHUNK_SIZE, top_plane)) != ERR_NONE)
				return (code);
		}

	for (unsigned int x = 0; x < CHUNK_SIZE; x++)
		for (unsigned int z = 0; z < CHUNK_SIZE; z++)
		{
			y = chunk->sub_hmap[x][z];

			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane)) != ERR_NONE
				|| (code = generate_side_plane(chunk, chunk->sub_hmap, x, y, z, CHUNK_SIZE, top_plane)) != ERR_NONE)
				return (code);
		}
	return (ERR_NONE);
}

static void				load_chunk_params(t_env *env, int x_start, int z_start, t_biome_params *params)
{
	float			biome_factor;
	float			delta, delta_inf, delta_sup;
	unsigned int	i = 0;

	// Get biome factor to compute chunk's generation parameters
	biome_factor = env->model.biomes[x_start / CHUNK_SIZE][z_start / CHUNK_SIZE];
	// Identify both inferior and superior bounds to this factor in the parameters array
	while (i < TP_MAX - 1)
	{
		if (biome_factor >= bgp[i].bound && biome_factor < bgp[i + 1].bound)
			break ;
		i++;
	}
	// Compute parameters with the biome factor with an interpolation
	delta = bgp[i + 1].bound - bgp[i].bound; // Delta between both bounds
	delta_inf = biome_factor - bgp[i].bound; // Inferior delta
	delta_sup = bgp[i + 1].bound - biome_factor; // Superior delta
	// Scale to 0-1 interval
	delta_inf /= delta;
	delta_sup /= delta;
	// Interpolation
	params->frequency = 0.02f;//(delta_inf * bgp[i].frequency + delta_sup * bgp[i + 1].frequency);
	params->depth = 6.0f;//(delta_inf * bgp[i].depth + delta_sup * bgp[i + 1].depth);

	//if (params->frequency > 0.3f)
	//	printf("factor : %f | lower bound : %f | upper bound : %f | delta_inf : %f | delta_sup : %f\n",
	//			biome_factor, bgp[i].bound, bgp[i + 1].bound, delta_inf, delta_sup);
	(void)delta;
	(void)delta_inf;
	(void)delta_sup;
}

static unsigned char	generate_chunk_content(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride)
{
	unsigned char	code;
	t_biome_params	surface_params = {0.02f, 6.0f, 0.0f, 0.0f};
	t_biome_params	sub_params = {0.05f, 6.0f, 0.0f, 40.0f};

	chunk->x_start = x_start;
	chunk->z_start = z_start;

	if (!chunk->surface_hmap || !chunk->sub_hmap)
	{
		// Load parameters for the current chunk
		load_chunk_params(env, x_start, z_start, &surface_params);
		//printf("frequency : %f | depth : %f\n", params.frequency, params.depth);
		// Generate height maps for surface and cave
		// Topography type should be a parameter which would affect perlin noise generation
		if (!(chunk->surface_hmap = generate_height_map(surface_params, x_start, z_start, CHUNK_SIZE, 40))
			|| !(chunk->sub_hmap = generate_height_map(sub_params, x_start, z_start, CHUNK_SIZE, 0)))
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
