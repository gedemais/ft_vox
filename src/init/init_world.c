# include "main.h"


const char	*textures_paths[TEXTURE_SB_MAX] = {
	// TEXTURES HD
	[TEXTURE_HD_WATER]		= "./resources/HD/gravel.png",
	[TEXTURE_HD_SAND]		= "./resources/HD/sand.png",
	[TEXTURE_HD_GRASS]		= "./resources/HD/grass.png",
	[TEXTURE_HD_GROUND]		= "./resources/HD/ground.png",
	[TEXTURE_HD_STONE]		= "./resources/HD/stone.png",
	[TEXTURE_HD_SNOW]		= "./resources/HD/snow.png",
	[TEXTURE_HD_GRASS_SIDE]	= "./resources/HD/grass_side.png",
	[TEXTURE_HD_SNOW_SIDE]	= "./resources/HD/stone_side.png",
	// TEXTURES LD
	[TEXTURE_LD_WATER]		= "./resources/HD/gravel.png",
	[TEXTURE_LD_SAND]		= "./resources/LD/sand.png",
	[TEXTURE_LD_GRASS]		= "./resources/LD/grass.png",
	[TEXTURE_LD_GROUND]		= "./resources/LD/ground.png",
	[TEXTURE_LD_STONE]		= "./resources/LD/stone.png",
	[TEXTURE_LD_SNOW]		= "./resources/LD/snow.png",
	[TEXTURE_LD_GRASS_SIDE]	= "./resources/LD/grass_side.png",
	[TEXTURE_LD_SNOW_SIDE]	= "./resources/LD/stone_side.png",
	// TEXTURES SKYBOX
	[TEXTURE_SB_PX]			= "./resources/skybox/px.png",
	[TEXTURE_SB_PY]			= "./resources/skybox/py.png",
	[TEXTURE_SB_PZ]			= "./resources/skybox/pz.png",
	[TEXTURE_SB_NX]			= "./resources/skybox/nx.png",
	[TEXTURE_SB_NY]			= "./resources/skybox/ny.png",
	[TEXTURE_SB_NZ]			= "./resources/skybox/nz.png"
};

static unsigned char	load_textures(t_env *env)
{
	t_texture		*txt;
	unsigned int	err;
	int				i;

	i = -1;
	while (++i < TEXTURE_SB_MAX) {
		txt = &env->model.textures[i];
		err = lodepng_decode32_file(&txt->ptr, &txt->w, &txt->h, textures_paths[i]);
		if (err) {
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
	unsigned char	code;


	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
		for (unsigned int y = 0; y < SQUARE_SIZE; y++)
			if ((code = gen_chunk(env, &env->model.chunks[x][y], x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE)))
				return (code);

	if ((code = borders(env)))
		return (code);

	printf("%zu bytes per chunk | %.2f Mo on heap\n", sizeof(t_stride), *stride_bytesize() / 1000000.0f);
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

	if ((code = model(env)) != ERR_NONE)
		return (code);

	light(env); // Light after model because we set the sun's light pos with the model center

	return (ERR_NONE);
}
