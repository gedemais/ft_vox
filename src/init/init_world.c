# include "main.h"

static unsigned char	init_biomes(t_env *env)
{
	if (!(env->model.biomes = (float**)malloc(MAP_SIZE * sizeof(float*))))
		return (ERR_MALLOC_FAILED);

	for (unsigned int x = 0; x < MAP_SIZE; x++)
	{
		if (!(env->model.biomes[x] = (float*)malloc(MAP_SIZE * sizeof(float))))
			return (ERR_MALLOC_FAILED);

		for (unsigned int y = 0; y < MAP_SIZE; y++)
			env->model.biomes[x][y] = perlin2d_a((float)x, (float)y, 0.1f, 6.0f);
	}
	return (ERR_NONE);
}

static unsigned char	init_map(t_env *env)
{
	unsigned int square = 10;
	for (unsigned int x = 0; x < square; x++)
		for (unsigned int y = 0; y < square; y++)
			gen_chunk(env, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE);

	return (ERR_NONE);
}

unsigned char			init_world(t_env *env)
{
	unsigned char	code;

	if ((code = init_biomes(env)) != ERR_NONE
		|| (code = init_map(env)))
		return (code);

	return (ERR_NONE);
}
