#include "main.h"

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

		for (int y = 0; y < CACHE_SIZE; y++)
		{
			ft_memset(&chunk, 0, sizeof(t_chunk));
			if ((code = gen_chunk(env, &chunk, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, false))
				|| (dynarray_push(&arr, &chunk, false) && (code = ERR_MALLOC_FAILED)))
				return (code);
			// water
		}
	}
	exit(0);
	return (ERR_NONE);
}
