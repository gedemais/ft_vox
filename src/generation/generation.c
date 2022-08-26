# include "main.h"

static unsigned char	generate_chunk_content(t_chunk *chunk, unsigned int x_start, unsigned int y_start, unsigned int size)
{
	unsigned char	code;

	// Generate height map for surface and cave
	// Topography type should be a parameter which would affect perlin noise generation
	if (!(chunk->surface_hmap = generate_height_map(x_start, y_start, size))
		|| !(chunk->sub_hmap = generate_height_map(x_start, y_start, size)))
		return (ERR_MALLOC_FAILED);

	// Generate 3D block map
	if  ((code = gen_block_map(chunk, x_start, y_start, size)))
		return (code);

	return (ERR_NONE);
}

// This function will be used to generate new chunks of terrain

unsigned char			gen_chunk(t_env *env, int x_start, int y_start, unsigned int size)
{
	t_chunk			chunk;
	unsigned char	code;

	if ((code = generate_chunk_content(&chunk, x_start, y_start, size)) != ERR_NONE)
		return (code);

	if ((env->model.chunks.arr == NULL
		&& dynarray_init(&env->model.chunks, sizeof(t_chunk), 64))
		|| dynarray_push(&env->model.chunks, &chunk, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}
