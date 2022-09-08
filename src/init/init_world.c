# include "main.h"

const char	*textures_paths[TEXTURE_MAX] = {
	"./resources/ground.png",
	"./resources/default.png",
	"./resources/darksouls.png"
};

static unsigned char	load_textures(t_env *env)
{
	t_texture		*txt;
	unsigned int	err;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		txt = &env->model.textures[i];

		err = lodepng_decode32_file(&txt->ptr, &txt->w, &txt->h, textures_paths[i]);

		if (err)
		{
			ft_putendl_fd(lodepng_error_text(err), 2);
			return (ERR_TEXTURE_LOADING_FAILED);
		}
	}
	return (ERR_NONE);
}

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
	env->gl.window.fullscreen = false;

	if ((argc > 1 && (code = read_seed(argc, argv)) != ERR_NONE)
		|| (code = init_biomes(env)) != ERR_NONE
		|| (code = init_map(env)) != ERR_NONE
		|| (code = load_textures(env)) != ERR_NONE)
		return (code);

	printf("seed : %d\n", *map_seed());
	model(env);
	light(env); // light after model because we set the sun's light pos with the model center

	return (ERR_NONE);
}
