#include "main.h"

static unsigned char	allocate_caches(t_env *env)
{
	t_chunk			chunk;
	t_dynarray		arr;
	unsigned char	code;

	if (dynarray_init(&env->model.chunks_cache, sizeof(t_dynarray), MAP_SIZE))
		return (ERR_MALLOC_FAILED);

	for (int x = 0; x < MAP_SIZE; x++)
	{
		if (dynarray_init(&arr, sizeof(t_chunk), MAP_SIZE)
			|| dynarray_push(&env->model.chunks_cache, &arr, false))
			return (ERR_MALLOC_FAILED);

		for (int z = 0; z < MAP_SIZE; z++)
		{
			ft_memset(&chunk, 0, sizeof(t_chunk));
			if ((dynarray_push(&arr, &chunk, false) && (code = ERR_MALLOC_FAILED)))
				return (code);
			// water
		}
		//if ((code = gen_chunk(env, &chunk, x * CHUNK_SIZE, z * CHUNK_SIZE, false))
	}
	printf("Allocated %d x %d (%d) chunks\n", MAP_SIZE, MAP_SIZE, MAP_SIZE * MAP_SIZE);
	return (ERR_NONE);
}

unsigned char	init_caches(t_env *env)
{
	unsigned char	code;

	if ((code = allocate_caches(env)))
		return (code);
	return (ERR_NONE);
}
