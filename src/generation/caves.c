#include "main.h"

static	uint8_t	***allocate_cave_map(unsigned int size)
{
	uint8_t	***new;

	if (!(new = (uint8_t***)malloc(sizeof(uint8_t**) * size)))
		return (NULL);

	for (unsigned int i = 0; i < size; i++)
	{
		if (!(new[i] = (uint8_t**)malloc(sizeof(uint8_t*) * size)))
			return (NULL);

		for (unsigned int j = 0; j < size; j++)
		{
			if (!(new[i][j] = (uint8_t*)malloc(sizeof(uint8_t) * size)))
				return (NULL);
			ft_memset(new[i][j], 0, sizeof(uint8_t) * size);
		}

	}
	return (new);
}

static unsigned char	init_worley_points(t_env *env, t_chunk *chunk, unsigned int size)
{
	t_wp	new_point;

	for (unsigned int i = 0; i < NB_WORLEY_POINTS; i++)
	{
		new_point.pos.x = rand() % size;
		new_point.pos.y = rand() % size;
		new_point.pos.z = rand() % size;

		new_point.pos.x += chunk->x_start;
		new_point.pos.z += chunk->z_start;

		new_point.x_start = chunk->x_start;
		new_point.z_start = chunk->z_start;

		if (dynarray_push(&env->model.wpoints, &new_point, false))
			return (ERR_MALLOC_FAILED);
	}

	return (ERR_NONE);
}

static int	dist(t_3dpoint a, t_3dpoint b)
{
	float	dx = b.x - a.x;
	float	dy = b.y - a.y;
	float	dz = b.z - a.z;

	return (sqrtf(dx * dx + dy * dy + dz * dz));
}
/*
void	sort_distances(float arr[NB_WORLEY_POINTS], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}*/


static void				get_three_lowests(t_dynarray distances, vec3 *res)
{
	float	tmp;
	float	*d;

	*res = (vec3){INFINITY, INFINITY, INFINITY};
	for (int i = 0; i < distances.nb_cells; i++)
	{
		d = dyacc(&distances, i);

		if (*d < res->z)
			res->z = *d;

		if (res->z < res->y)
		{
			tmp = res->y;
			res->y = res->z;
			res->z = tmp;
		}

		if (res->y < res->x)
		{
			tmp = res->x;
			res->x = res->y;
			res->y = tmp;
		}
	}
}

static unsigned char	worley_noise(t_env *env, t_dynarray distances, t_3dpoint current, uint8_t *res)
{
	t_wp	*wpoint;
	float	min_dist = INFINITY;
	float	distance;
	vec3	lowests;

	for (int i = 0; i < env->model.wpoints.nb_cells; i++)
	{
		wpoint = dyacc(&env->model.wpoints, i);
		distance = dist(wpoint->pos, current);
		if (dynarray_push(&distances, &distance, false))
			return (ERR_MALLOC_FAILED);

		min_dist = fmin(distance, min_dist);
	}

//	sort_distances(distances, NB_WORLEY_POINTS);
	get_three_lowests(distances, &lowests);

	if (lowests.x / lowests.z > WORLEY_THRESHOLD)
		*res = BT_STONE;

	return (ERR_NONE);
}

unsigned char			generate_cave_map(t_env *env, t_chunk *chunk, unsigned int size)
{
	t_dynarray		distances;
	unsigned char	code;

	if (dynarray_init(&distances, sizeof(float), NB_WORLEY_POINTS * SQUARE_SIZE * SQUARE_SIZE))
		return (ERR_MALLOC_FAILED);

	if (!(chunk->cave_map = allocate_cave_map(size))
		|| init_worley_points(env, chunk, size) != ERR_NONE)
		return (ERR_MALLOC_FAILED);

	//return (ERR_NONE);
	for (unsigned int x = 0; x < size; x++)
		for (unsigned int y = 0; y < size; y++)
			for (unsigned int z = 0; z < size; z++)
				if ((code = worley_noise(env, distances, (t_3dpoint){x, y, z}, &chunk->cave_map[x][y][z])))
					return (code);

	/*
	for (unsigned int x = 0; x < size; x++)
	{
		for (unsigned int y = 0; y < size; y++)
		{
			for (unsigned int z = 0; z < size; z++)
				if (chunk->cave_map[x][y][z] == 0)
					printf(". ");
				else
					printf("0 ");
			printf("\n");
		}
		printf("\n");
		usleep(100000);
	}

	fflush(stdout);*/

	return (ERR_NONE);
}
