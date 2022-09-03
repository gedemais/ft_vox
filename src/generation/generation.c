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
				|| (code = generate_side_plane(chunk, x, y, z, chunk_size, top_plane)) != ERR_NONE
				|| (code = generate_bottom_plane(chunk, x, y, z, top_plane)) != ERR_NONE)
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
		|| !(chunk->sub_hmap = generate_height_map(x_start, y_start, size))
		|| dynarray_init(&chunk->stride, sizeof(t_stride), size * size * 6 * 2))
		return (ERR_MALLOC_FAILED);

	return (generate_vertexs(chunk, x_start, y_start, size));
}

// This function will be used to generate new chunks of terrain with different noise algorithms

unsigned char			gen_chunk(t_env *env, int x_start, int y_start, unsigned int size)
{
	t_chunk			chunk;
	unsigned char	code;

	if ((code = generate_chunk_content(&chunk, x_start, y_start, size)) != ERR_NONE)
		return (code);

	printf("%d vertexs | %zu bytes per vertex | %.2f Mo on heap\n", chunk.stride.nb_cells, sizeof(t_stride), sizeof(t_stride) * chunk.stride.nb_cells / 1000000.0f);

	if ((env->model.chunks.arr == NULL
		&& dynarray_init(&env->model.chunks, sizeof(t_chunk), 64))
		|| dynarray_push(&env->model.chunks, &chunk, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}
