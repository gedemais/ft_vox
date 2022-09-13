#include "main.h"

enum	e_trigger_ids
{
	TRIGGER_NORTH,
	TRIGGER_SOUTH,
	TRIGGER_WEST,
	TRIGGER_EAST,
	TRIGGER_MAX
};

static unsigned char	move_square(t_env *env, int trigger_id, int x, int z)
{
	char	*strs[TRIGGER_MAX] = {
		"north",
		"south",
		"west",
		"east"};

	//printf("%s\n", strs[trigger_id]);
	(void)env;
	(void)x;
	(void)z;
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
				&& (code = move_square(env, trigger_id, x, z)))
				return (code);
		}
	return (ERR_NONE);
}
