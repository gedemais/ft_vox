# include "main.h"

enum e_topography_type
{
	E_PLAINS,
	E_MOUNTAINS,
	E_DESERT,
	E_SEA,
	E_MAX
};

static unsigned char	generate_chunk_content(t_chunk *chunk, unsigned int x_start, unsigned int y_start, unsigned int size)
{
	if (!(chunk->height_map = generate_height_map(x_start, y_start, size))) // Topography type should be a parameter which would affect perlin noise generation
		return (ERR_MALLOC_FAILED);

	generate_blocks();
	return (ERR_NONE);
}

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
