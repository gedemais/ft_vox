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
static unsigned char	remove_chunk_mesh(t_env *env, t_chunk *chunk, t_dynarray *olds)
{
	bool	found = false;
	t_mesh	*m;

	dynarray_free(&chunk->stride);

	for (unsigned int i = 0; i < CHUNK_SIZE; i++)
	{
		free(chunk->surface_hmap[i]); // To remove if cache system comes back
		free(chunk->sub_hmap[i]);
	}
	free(chunk->surface_hmap); // To remove if cache system comes back
	free(chunk->sub_hmap);

	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		m = dyacc(&env->model.meshs, i);
		if (m->x_start == chunk->x_start && m->z_start == chunk->z_start)
		{
			if ((olds->arr == NULL && dynarray_init(olds, sizeof(GLuint), SQUARE_SIZE))
				|| dynarray_push(olds, &m->vbo, false))
				return (ERR_MALLOC_FAILED);
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

typedef struct	s_ms_params
{
	t_env		*env;
	int			trigger_id;
	pthread_t	*thread_id;
	t_chunk		*news[SQUARE_SIZE];
	t_dynarray	olds;
}				t_ms_params;

static unsigned char move_square_on_z(t_env *env, t_ms_params *params)
{
	int				trigger_id = params->trigger_id;
	bool			north = (trigger_id == TRIGGER_NORTH);
	unsigned char	code;
	t_mesh			mesh;
	int				new_z;

	new_z = north ? SQUARE_SIZE - 1 : 0;
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (north && !remove_chunk_mesh(env, &env->model.chunks[i][0], &params->olds))
			for (int j = 0; j < SQUARE_SIZE - 1; j++)
				env->model.chunks[i][j] = env->model.chunks[i][j + 1];

		else if (!remove_chunk_mesh(env, &env->model.chunks[i][SQUARE_SIZE - 1], &params->olds))
			for (int j = SQUARE_SIZE - 1; j > 0; j--)
				env->model.chunks[i][j] = env->model.chunks[i][j - 1];
	}

	env->model.square_z += north ? 1 : -1;
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		params->news[i] = &env->model.chunks[i][new_z];
		memset(params->news[i], 0, sizeof(t_chunk));
		if (gen_chunk(env, params->news[i], (env->model.square_x + i) * CHUNK_SIZE, (env->model.square_z + new_z) * CHUNK_SIZE, true) != ERR_NONE)
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

static unsigned char	move_square_on_x(t_env *env, t_ms_params *params)
{
	int				trigger_id = params->trigger_id;
	bool			west = trigger_id == TRIGGER_WEST;
	unsigned char	code;
	t_mesh			mesh;
	int				new_x;

	new_x = west ? SQUARE_SIZE - 1 : 0;
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		if (west && !remove_chunk_mesh(env, &env->model.chunks[0][i], &params->olds))
			for (int j = 0; j < SQUARE_SIZE - 1; j++)
				env->model.chunks[j][i] = env->model.chunks[j + 1][i];

		else if (!remove_chunk_mesh(env, &env->model.chunks[SQUARE_SIZE - 1][i], &params->olds))
			for (int j = SQUARE_SIZE - 1; j > 0; j--)
				env->model.chunks[j][i] = env->model.chunks[j - 1][i];
	}

	env->model.square_x += west ? 1 : -1;
	for (int i = 0; i < SQUARE_SIZE; i++)
	{
		params->news[i] = &env->model.chunks[new_x][i];
		memset(params->news[i], 0, sizeof(t_chunk));
		if ((code = gen_chunk(env, params->news[i], (env->model.square_x + new_x) * CHUNK_SIZE, (env->model.square_z + i) * CHUNK_SIZE, true)) != ERR_NONE)
			return (code);
	}
	return (ERR_NONE);
}

static unsigned char	update_square(t_env *env, t_ms_params *params)
{
	t_mesh			mesh;
	GLuint			*vbo;
	unsigned char	code;

	for (unsigned int x = 0; x < SQUARE_SIZE; x++)
	{
		// Water generation
		if ((code = generate_water(params->news[x])) != ERR_NONE)
			return (code);

		// Borders fix
		for (unsigned int z = 0; z < SQUARE_SIZE; z++)
			if ((code = fix_chunk_borders(env, x, z)) != ERR_NONE
				|| (code = update_chunk_mesh(env, x, z)))
				return (code);
	}

	for (unsigned int i = 0; i < SQUARE_SIZE; i++)
	{
		// Water generation

		// Initialization of the newly created mesh
		ft_memset(&mesh, 0, sizeof(t_mesh));
		mesh.vertices = params->news[i]->stride;
		mesh.x_start = params->news[i]->x_start;
		mesh.z_start = params->news[i]->z_start;

		// Addition of the mesh to the chunks pool
		if (init_mesh(env, &mesh) || dynarray_push(&env->model.meshs, &mesh, true) < 0)
			return (ERR_MALLOC_FAILED);

		// Free vbo for disappeared chunks
		glDeleteBuffers(1, dyacc(&params->olds, (int)i));
	}

	dynarray_free(&params->olds);

	return (ERR_NONE);
}

static void	*move_square(void *params)
{
	t_env		*env;
	int			trigger_id;
	t_chunk		*news[SQUARE_SIZE];

	env = ((t_ms_params*)params)->env;
	trigger_id = ((t_ms_params*)params)->trigger_id;

	if (trigger_id == TRIGGER_NORTH || trigger_id == TRIGGER_SOUTH)
		move_square_on_z(env, params);
	else
		move_square_on_x(env, params);

	*((t_ms_params*)params)->thread_id = (pthread_t)1;
	return (NULL);
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
	static t_ms_params	params;
	static pthread_t	thread_id = (pthread_t)0;
	int					trigger_id = 0;


	if (thread_id == (pthread_t)1)
	{
		update_square(env, &params);
		thread_id = (pthread_t)0;
		return (ERR_NONE);
	}

	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
			if (check_player_presence(env->camera.pos, env->model.chunks[x][z])
				&& check_trigger(x, z, &trigger_id))
				if (thread_id == (pthread_t)0)
				{
					params = (t_ms_params){env, trigger_id, &thread_id, {}, {}};
					pthread_create(&thread_id, NULL, &move_square, &params);
					pthread_detach(thread_id);
					return (ERR_NONE);
				}

	return (ERR_NONE);
}
