#include "main.h"

enum	e_trigger_ids
{
	TRIGGER_NORTH,
	TRIGGER_SOUTH,
	TRIGGER_WEST,
	TRIGGER_EAST,
	TRIGGER_MAX
};

void				print_square(t_env *env)
{
	for (int x = 0; x < SQUARE_SIZE; x++)
	{
		for (int z = 0; z < SQUARE_SIZE; z++)
			printf("[%d %d] ", env->model.chunks[z][x].x_start, env->model.chunks[z][x].z_start);
		printf("\n");
	}
}

static unsigned char	free_chunk_stride(t_chunk *chunk)
{
	dynarray_free(&chunk->stride);
	return (ERR_NONE);
}

static t_chunk			*get_cached_chunk(t_env *env, int x, int z)
{
	t_chunk		*chunk;
	int			x_chunk, z_chunk;

	x_chunk = env->model.square_x + x;
	z_chunk = env->model.square_z + z;
	if (!(chunk = dyacc(dyacc(&env->model.chunks_cache, x_chunk), z_chunk))
		|| !chunk->surface_hmap || !chunk->sub_hmap)
		return (NULL);

	return (chunk);
}

static unsigned char	move_square_on_z(t_env *env, int trigger_id)
{
	unsigned char	code;
	bool			south = trigger_id == TRIGGER_NORTH;
	t_mesh			mesh;
	t_chunk			*new;
	t_chunk			*cached;
	int				new_z;

	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (south && !free_chunk_stride(&env->model.chunks[i][SQUARE_SIZE - 1]))
			for (int j = 0; j < SQUARE_SIZE - 1; j++)
				env->model.chunks[i][j] = env->model.chunks[i][j + 1];

		else if (!free_chunk_stride(&env->model.chunks[i][0]))
			for (int j = SQUARE_SIZE - 1; j >= 0; j--)
				env->model.chunks[i][j] = env->model.chunks[i][j - 1];
	}

	new_z = south ? SQUARE_SIZE - 1 : 0;
	env->model.square_z += south ? 1 : -1;
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		new = &env->model.chunks[i][new_z];
		memset(new, 0, sizeof(t_chunk));
		cached = get_cached_chunk(env, env->model.square_x + i, env->model.square_z + new_z);
		if (!cached && (code = gen_chunk(env, new, (env->model.square_x + i) * CHUNK_SIZE, (env->model.square_z + new_z) * CHUNK_SIZE, true)) != ERR_NONE)
			return (code);
		else if (cached)
		{
			memcpy(new, cached, sizeof(t_chunk));
			if ((code = gen_chunk(env, new, new->x_start, new->z_start, true)))
				return (code);
		}
		ft_memset(&mesh, 0, sizeof(t_mesh));

		if (dynarray_init(&mesh.vertices, sizeof(t_stride), CHUNK_SIZE *  6 * sizeof(t_stride)) < 0)
			return (ERR_MALLOC_FAILED);

		for (int i = 0; i < new->stride.nb_cells; i++)
			if (dynarray_push(&mesh.vertices, dyacc(&new->stride, i), false))
				return (ERR_MALLOC_FAILED);

		init_mesh(env, &mesh);
		if (dynarray_push(&env->model.meshs, &mesh, true) < 0)
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

//static unsigned char	move_square_on_x(t_env *env, int trigger_id, int x, int z)

static unsigned char	move_square(t_env *env, int trigger_id)
{
	char	*strs[TRIGGER_MAX] = {
		"north",
		"south",
		"west",
		"east"};

	printf("%s\n", strs[trigger_id]);
	(void)env;

	if (trigger_id == TRIGGER_NORTH || trigger_id == TRIGGER_SOUTH)
	{
		move_square_on_z(env, trigger_id);
	print_square(env);
	printf("-------------------\n");
		return (ERR_NONE);
	}
	//else
	//	return (move_square_on_x(env, trigger_id, x, z));*/

	return (ERR_NONE);
}

static bool				check_player_presence(vec3 pos, t_chunk chunk)
{
	bool	x, z;

	x = pos.x > chunk.x_start && pos.x < chunk.x_start + CHUNK_SIZE;
	z = pos.z > chunk.z_start && pos.z < chunk.z_start + CHUNK_SIZE;
	return (x && z);
}

static bool				check_trigger(int x, int z, int *trigger_id)
{
	bool	a, b, c, d;

	const int		p_trigger = (SQUARE_SIZE / 2 + SQUARE_TRIGGER_SIZE / 2);
	const int		n_trigger = (SQUARE_SIZE / 2 - SQUARE_TRIGGER_SIZE / 2);

	a = x < n_trigger && (*trigger_id = TRIGGER_EAST) != TRIGGER_MAX;
	b = x > p_trigger && (*trigger_id = TRIGGER_WEST) != TRIGGER_MAX;
	c = z < n_trigger && (*trigger_id = TRIGGER_SOUTH) != TRIGGER_MAX;
	d = z > p_trigger && (*trigger_id = TRIGGER_NORTH) != TRIGGER_MAX;

	return (a || b || c || d);
}

unsigned char			update_world(t_env *env)
{
	unsigned char	code;
	int				trigger_id;

	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
		{
			if (check_player_presence(env->camera.pos, env->model.chunks[x][z])
				&& check_trigger(x, z, &trigger_id)
				&& (code = move_square(env, trigger_id)))
				return (code);
		}
	return (ERR_NONE);
}
