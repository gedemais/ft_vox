#include "main.h"

unsigned char	init_chunk(t_env *env, int x, int z)
{
	unsigned char	code = ERR_NONE;
	t_chunk			*chunk;
	t_dynarray		*line;

	line = dyacc(&env->model.chunks_cache, x);
	chunk = dyacc(line, z);

	return (generate_vertexs(chunk, x * CHUNK_SIZE, z * CHUNK_SIZE));
}

unsigned char	init_caches(t_env *env)
{
	t_chunk			chunk;
	t_dynarray		arr;
	unsigned char	code;

	if (dynarray_init(&env->model.chunks_cache, sizeof(t_dynarray), CACHE_SIZE))
		return (ERR_MALLOC_FAILED);

	for (int x = 0; x < CACHE_SIZE; x++)
	{
		printf("-");
		fflush(stdout);
		if (dynarray_init(&arr, sizeof(t_chunk), CACHE_SIZE)
			|| dynarray_push(&env->model.chunks_cache, &arr, false))
			return (ERR_MALLOC_FAILED);

		for (int z = 0; z < CACHE_SIZE; z++)
		{
			ft_memset(&chunk, 0, sizeof(t_chunk));
			if ((code = gen_chunk(env, &chunk, x * CHUNK_SIZE, z * CHUNK_SIZE, false))
				|| (dynarray_push(&arr, &chunk, false) && (code = ERR_MALLOC_FAILED)))
				return (code);
			// water
		}
	}
	return (ERR_NONE);
}
