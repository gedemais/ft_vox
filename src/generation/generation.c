# include "main.h"

static unsigned char	generate_vertexs(t_chunk *chunk, int x_start, int y_start, unsigned int chunk_size)
{
	unsigned char	code;
	unsigned int	z;
	vec3			top_plane[6];

	for (unsigned int x = 0; x < chunk_size; x++)
		for (unsigned int y = 0; y < chunk_size; y++)
		{
			z = chunk->surface_hmap[x][y];

			if ((code = generate_top_plane(chunk, x, y, z, x_start, y_start, top_plane)) != ERR_NONE
				|| (code = generate_side_plane(chunk, x, y, z, chunk_size, top_plane)) != ERR_NONE)
				//|| (code = generate_bottom_plane(chunk, x, y, z, top_plane)) != ERR_NONE)
				return (code);
		}

	return (ERR_NONE);
}

static void				load_chunk_params(t_env *env, int x_start, int y_start, unsigned int size, t_biome_params *params)
{
	float			biome_factor;
	float			delta, delta_inf, delta_sup;
	unsigned int	i = 0;

	// Get biome factor to compute chunk's generation parameters
	biome_factor = env->model.biomes[x_start / size][y_start / size];
	// Identify both inferior and superior bounds to this factor in the parameters array
	while (i < TP_MAX - 1)
	{
		if (biome_factor >= bgp[i].bound && biome_factor < bgp[i + 1].bound)
			break ; i++;
	}
	// Compute parameters with the biome factor with an interpolation
	delta = bgp[i + 1].bound - bgp[i].bound; // Delta between both bounds
	delta_inf = biome_factor - bgp[i].bound; // Inferior delta
	delta_sup = bgp[i + 1].bound - biome_factor; // Superior delta
	// Scale to 0-1 interval
	delta_inf /= delta;
	delta_sup /= delta;
	// Interpolation
	params->frequency = (delta_inf * bgp[i].frequency + delta_sup * bgp[i + 1].frequency);
	params->depth = (delta_inf * bgp[i].depth + delta_sup * bgp[i + 1].depth);
}

static unsigned char	generate_chunk_content(t_env *env, t_chunk *chunk, int x_start, int y_start, unsigned int size)
{
	unsigned char	code;
	t_biome_params	params;

	// Load parameters for the current chunk
	load_chunk_params(env, x_start, y_start, size, &params);
	printf("frequency : %f | depth : %f\n", params.frequency, params.depth);
	// Generate height maps for surface and cave
	// Topography type should be a parameter which would affect perlin noise generation
	if (!(chunk->surface_hmap = generate_height_map(params, x_start, y_start, size))
		|| !(chunk->sub_hmap = generate_height_map(params, x_start, y_start, size))
		|| dynarray_init(&chunk->stride, sizeof(t_stride), size * size * 6 * 2))
		return (ERR_MALLOC_FAILED);

	return (generate_vertexs(chunk, x_start, y_start, size));
}

// This function will be used to generate new chunks of terrain with different noise algorithms

size_t	*stride_bytesize(void)
{
	static size_t	bytesize = 0;

	return (&bytesize);
}

unsigned char			gen_chunk(t_env *env, int x_start, int y_start, unsigned int size)
{
	t_chunk			chunk;
	unsigned char	code;

	if ((code = generate_chunk_content(env, &chunk, x_start, y_start, size)) != ERR_NONE)
		return (code);

	*stride_bytesize() += sizeof(t_stride) * chunk.stride.nb_cells;
	printf("%d vertexs | %zu bytes per chunk | %.2f Mo\n", chunk.stride.nb_cells, sizeof(t_stride), sizeof(t_stride) * chunk.stride.nb_cells / 1000000.0f);

	if ((env->model.chunks.arr == NULL
		&& dynarray_init(&env->model.chunks, sizeof(t_chunk), CHUNK_SIZE))
		|| dynarray_push(&env->model.chunks, &chunk, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}
