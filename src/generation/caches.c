#include "main.h"

#define NB_THREADS 8

typedef struct	s_thread_params
{
	t_env		*env;
	t_dynarray	*arr;
	int			x;
}				t_thread_p;

static void	*generate_chunk_line(void *p)
{
	t_thread_p		*params = p;
	t_chunk			chunk;
	unsigned char	code;

	for (int y = 0; y < WORLD_SIZE; y++)
	{
		if ((code = gen_chunk(params->env, &chunk, params->x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, false))
			|| (dynarray_push(params->arr, &chunk, false) && (code = ERR_MALLOC_FAILED)))
			return (NULL);
		// water
	}
	return (NULL);
}

unsigned char	init_caches(t_env *env)
{
	pthread_t		thread_ids[NB_THREADS];
	t_thread_p	params;
	t_dynarray		arr;
	unsigned char	code;

	if (dynarray_init(&env->model.chunks_cache, sizeof(t_dynarray), WORLD_SIZE))
		return (ERR_MALLOC_FAILED);

	for (int x = 0; x < WORLD_SIZE; x += NB_THREADS)
	{

		printf("-");
		fflush(stdout);
		if (dynarray_init(&arr, sizeof(t_chunk), WORLD_SIZE)
			|| dynarray_push(&env->model.chunks_cache, &arr, false))
			return (ERR_MALLOC_FAILED);

		params.env = env;
		params.arr = &arr;
		for (int i = 0; i < NB_THREADS; i++)
		{
			params.x = x + i;
			pthread_create(&thread_ids[i], NULL, generate_chunk_line, &params);
		}

		for (int i = 0; i < NB_THREADS; i++)
		{
			pthread_join(thread_ids[i], NULL);
		}
	}

	exit(0);
	return (ERR_NONE);
}
