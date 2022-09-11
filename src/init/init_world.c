# include "main.h"


static unsigned char	read_seed(int argc, char **argv)
{
	long long int	seed;
	if (argc > 2)
		return (ERR_INVALID_ARGC);

	for (unsigned int i = 0; argv[1][i]; i++)
		if (i > 10 || ft_isdigit(argv[1][i]) == false)
			return (ERR_INVALID_SEED);

	seed = ft_atoi(argv[1]);

	*map_seed() = seed;

	return (ERR_NONE);
}

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
	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
		for (unsigned int y = 0; y < SQUARE_SIZE; y++)
			gen_chunk(env, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE);

	return (ERR_NONE);
}

unsigned char			init_world(t_env *env, int argc, char **argv)
{
	unsigned char	code;

	srand(time(NULL));
	env->window.fullscreen = false;

	if ((argc > 1 && (code = read_seed(argc, argv)) != ERR_NONE)
		|| (code = init_biomes(env)) != ERR_NONE
		|| (code = init_map(env)) != ERR_NONE)
		return (code);

	printf("seed : %d\n", *map_seed());
	if ((code = model(env)) != ERR_NONE)
		return (code);

	light(env);

	return (ERR_NONE);
}
