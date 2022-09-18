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

// Shall we return an error if the chunk to remove is not found ?
static unsigned char	remove_chunk_mesh(t_env *env, t_chunk *chunk)
{
	bool	found = false;
	t_mesh	*m;

	dynarray_free(&chunk->stride);

	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		m = dyacc(&env->model.meshs, i);
		if (m->x_start == chunk->x_start && m->z_start == chunk->z_start)
		{
			dynarray_extract(&env->model.meshs, i);
			found = true;
			break ;
		}
	}

	if (!found)
	{
		printf("Not found (%d %d)\n", chunk->x_start, chunk->z_start);
		print_square(env);
	}

	return (ERR_NONE);
}
/*
static t_chunk			*get_cached_chunk(t_env *env, int x, int z)
{
	t_chunk		*chunk;
	int			x_chunk, z_chunk;

	x_chunk = (env->model.square_x + x) / 2;
	z_chunk = (env->model.square_z + z) / 2;
	//printf("looking for %d %d\n", x_chunk, z_chunk);
	if (!(chunk = dyacc(dyacc(&env->model.chunks_cache, x_chunk), z_chunk))
		|| !chunk->surface_hmap || !chunk->sub_hmap)
		return (NULL);

	return (chunk);
}

static void	cache_chunk(t_env *env, t_chunk *chunk)
{
	t_chunk			*cached;
	int				x, z;

	x = chunk->x_start / CHUNK_SIZE;
	z = chunk->z_start / CHUNK_SIZE;

	//printf("%d %d cached\n", x, z);
	cached = dyacc(dyacc(&env->model.chunks_cache, x), z);

	cached->surface_hmap = chunk->surface_hmap;
	cached->sub_hmap = chunk->sub_hmap;
	cached->x_start = chunk->x_start;
	cached->z_start = chunk->z_start;
}*/
/*
typedef struct	s_ms_params
{
	t_env		*env;
	int			trigger_id;
	pthread_t	*thread_id;
}				t_ms_params;*/

static unsigned char move_square_on_z(t_env *env, int trigger_id)
{
	//t_env			*env = ((t_ms_params*)params)->env;
	//int				trigger_id = ((t_ms_params*)params)->trigger_id;
	bool			north = (trigger_id == TRIGGER_NORTH);
	//pthread_t		*tid = ((t_ms_params*)params)->thread_id;

	unsigned char	code;
	t_mesh			mesh;
	t_chunk			*news[SQUARE_SIZE];
	//t_chunk			*cached;
	int				new_z;

	new_z = north ? SQUARE_SIZE - 1 : 0;
	//printf("remove line\n");
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (north && !remove_chunk_mesh(env, &env->model.chunks[i][0]))
		{
			//cache_chunk(env, &env->model.chunks[i][0]);
			for (int j = 0; j < SQUARE_SIZE - 1; j++)
				env->model.chunks[i][j] = env->model.chunks[i][j + 1];
		}

		else if (!remove_chunk_mesh(env, &env->model.chunks[i][SQUARE_SIZE - 1]))
		{
			//cache_chunk(env, &env->model.chunks[i][SQUARE_SIZE - 1]);
			for (int j = SQUARE_SIZE - 1; j > 0; j--)
				env->model.chunks[i][j] = env->model.chunks[i][j - 1];
		}
	}

	env->model.square_z += north ? 1 : -1;
	//printf("add new chunks\n");
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		news[i] = &env->model.chunks[i][new_z];
		memset(news[i], 0, sizeof(t_chunk));

	//	cached = get_cached_chunk(env, env->model.square_x + i, env->model.square_z + new_z);
		if (gen_chunk(env, news[i], (env->model.square_x + i) * CHUNK_SIZE, (env->model.square_z + new_z) * CHUNK_SIZE, true) != ERR_NONE)
			return (ERR_MALLOC_FAILED);
		/*else if (cached)
		{
			printf("cached found !\n");
			memcpy(news[i], cached, sizeof(t_chunk));
			if ((code = gen_chunk(env, news[i], news[i]->x_start, news[i]->z_start, true)))
				return (code);
		}*/
	}

	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (north)
			fix_south_border(&env->model.chunks[i][new_z - 1], &env->model.chunks[i][new_z]);
		else
			fix_south_border(&env->model.chunks[i][new_z], &env->model.chunks[i][new_z + 1]);
		if (i < SQUARE_SIZE - 1)
			fix_east_border(&env->model.chunks[i][new_z], &env->model.chunks[i + 1][new_z]);

		ft_memset(&mesh, 0, sizeof(t_mesh));
		mesh.vertices = news[i]->stride;
		mesh.x_start = news[i]->x_start;
		mesh.z_start = news[i]->z_start;

		if (init_mesh(env, &mesh) != ERR_NONE || dynarray_push(&env->model.meshs, &mesh, true) < 0)
			return (ERR_MALLOC_FAILED); // should be replaced by returning code (just as other returns statements in this function)
	}

	//printf("%d meshs\nsquare_x : %d\nsquare_z : %d\npos : %f %f %f\n",
	//env->model.meshs.nb_cells, env->model.square_x * CHUNK_SIZE, env->model.square_z * CHUNK_SIZE, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);

	//exit(0);
	return (ERR_NONE);
}

static unsigned char	move_square_on_x(t_env *env, int trigger_id)
{
	unsigned char	code;
	bool			west = trigger_id == TRIGGER_WEST;
	t_mesh			mesh;
	t_chunk			*news[SQUARE_SIZE];
	//t_chunk			*cached;
	int				new_x;

	new_x = west ? SQUARE_SIZE - 1 : 0;
	//printf("remove line\n");
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (west && !remove_chunk_mesh(env, &env->model.chunks[0][i]))
		{
			//cache_chunk(env, &env->model.chunks[i][0]);
			for (int j = 0; j < SQUARE_SIZE - 1; j++)
				env->model.chunks[j][i] = env->model.chunks[j + 1][i];
		}

		else if (!remove_chunk_mesh(env, &env->model.chunks[SQUARE_SIZE - 1][i]))
		{
			//cache_chunk(env, &env->model.chunks[i][SQUARE_SIZE - 1]);
			for (int j = SQUARE_SIZE - 1; j > 0; j--)
				env->model.chunks[j][i] = env->model.chunks[j - 1][i];
		}
	}

	env->model.square_x += west ? 1 : -1;
	//printf("add new chunks\n");
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		news[i] = &env->model.chunks[new_x][i];
		memset(news[i], 0, sizeof(t_chunk));

//		cached = get_cached_chunk(env, env->model.square_x + new_x, env->model.square_z + i);
		if ((code = gen_chunk(env, news[i], (env->model.square_x + new_x) * CHUNK_SIZE, (env->model.square_z + i) * CHUNK_SIZE, true)) != ERR_NONE)
			return (code);
/*		else if (cached)
		{
			printf("cached found !\n");
			memcpy(news[i], cached, sizeof(t_chunk));
			if ((code = gen_chunk(env, news[i], news[i]->x_start, news[i]->z_start, true)))
				return (code);
		}*/
	}

	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (west)
			fix_east_border(&env->model.chunks[new_x - 1][i], &env->model.chunks[new_x][i]);
		else
			fix_east_border(&env->model.chunks[new_x][i], &env->model.chunks[new_x + 1][i]);

		if (i < SQUARE_SIZE - 1)
			fix_south_border(&env->model.chunks[new_x][i], &env->model.chunks[new_x][i + 1]);

		ft_memset(&mesh, 0, sizeof(t_mesh));
		mesh.vertices = news[i]->stride;
		mesh.x_start = news[i]->x_start;
		mesh.z_start = news[i]->z_start;

		if (init_mesh(env, &mesh) || dynarray_push(&env->model.meshs, &mesh, true) < 0)
			return (ERR_MALLOC_FAILED);
	}

	//printf("%d meshs\nsquare_x : %d\nsquare_z : %d\npos : %f %f %f\n",
//		env->model.meshs.nb_cells, env->model.square_x * CHUNK_SIZE, env->model.square_z * CHUNK_SIZE, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);
	//exit(0);
	return (ERR_NONE);
}

static unsigned char	move_square(t_env *env, int trigger_id)
{
	/*t_ms_params	params =	{
								env,
								trigger_id,
								thread_id
							};*/

	char		*strs[TRIGGER_MAX] = {
		"north",
		"south",
		"west",
		"east"};

	//printf("trigger %s\n", strs[trigger_id]);
	//printf("%d meshs\n", env->model.meshs.nb_cells);
	//printf("square_x : %d square_y : %d\n", env->model.square_x, env->model.square_z);

	if (trigger_id == TRIGGER_NORTH || trigger_id == TRIGGER_SOUTH)
		return (move_square_on_z(env, trigger_id));
	else
		return (move_square_on_x(env, trigger_id));

	//printf("triggered %s | %d meshs in scene\n", strs[trigger_id], env->model.meshs.nb_cells);
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
	int					trigger_id = 0;
	unsigned char		code;

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
