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

float randbycoords(int x, int y) {
  /* mix around the bits in x: */
  x = x * 3266489917 + 374761393;
  x = (x << 17) | (x >> 15);

  /* mix around the bits in y and mix those into x: */
  x += y * 3266489917;

  /* Give x a good stir: */
  x *= 668265263;
  x ^= x >> 15;
  x *= 2246822519;
  x ^= x >> 13;
  x *= 3266489917;
  x ^= x >> 16;

  /* trim the result and scale it to a float in [0,1): */
  return (x & 0x00ffffff) * (1.0f / 0x1000000);
}

unsigned char	init_worley_points(t_env *env, t_chunk *chunk, unsigned int size)
{
	(void)env;

	int	seed = (int)(*map_seed() * randbycoords(chunk->x_start, chunk->z_start));

	srand(seed);

	printf("%d %d : %d\n", chunk->x_start, chunk->z_start, seed);
	for (unsigned int i = 0; i < NB_WORLEY_POINTS; i++)
	{
		chunk->wpoints[i].x = rand() % size;
		chunk->wpoints[i].y = rand() % size;
		chunk->wpoints[i].z = rand() % size;

		chunk->wpoints[i].x += chunk->x_start;
		chunk->wpoints[i].z += chunk->z_start;
		//printf("%d %d %d\n", chunk->wpoints[i].x, chunk->wpoints[i].y, chunk->wpoints[i].z);
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


static void				get_three_lowests(t_dynarray *distances, vec3 *res)
{
	float	tmp;
	float	*d;

	*res = (vec3){INFINITY, INFINITY, INFINITY};
	for (int i = 0; i < distances->nb_cells; i++)
	{
		d = dyacc(distances, i);

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

static unsigned char	get_points(t_env *env, t_chunk *chunk, t_dynarray *points)
{
	t_chunk	*tmp;
	int		current_x, current_z;
	(void)chunk;
/*
	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
		{
			tmp = &env->model.chunks[x][z];
			if (tmp->x_start == chunk->x_start && tmp->z_start == chunk->z_start)
			{
				current_x = x;
				current_z = z;
			}
		}*/

//	for (int x = current_x - 1; x <= current_x + 1; x++)
//		for (int z = current_z - 1; z <= current_z + 1; z++)
	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
		{
			for (int i = 0; i < NB_WORLEY_POINTS; i++)
			{
				printf("%d %d %d\n", env->model.chunks[x][z].wpoints[i].x, env->model.chunks[x][z].wpoints[i].y, env->model.chunks[x][z].wpoints[i].z);
				if (dynarray_push(points, &env->model.chunks[x][z].wpoints[i], false))
					return (ERR_MALLOC_FAILED);
			}
		}
	return (ERR_NONE);
}

static unsigned char	worley_noise(t_env *env, t_chunk *chunk, t_dynarray *points, t_dynarray *distances, t_3dpoint current, uint8_t *res)
{
	t_3dpoint	*wpoint;
	float		min_dist = INFINITY;
	float		distance;
	vec3		lowests;

	(void)env;
	(void)chunk;
	current.x += chunk->x_start;
	current.z += chunk->z_start;
	for (int i = 0; i < points->nb_cells; i++)
	{
		wpoint = dyacc(points, i);
		distance = dist(*wpoint, current);
		if (dynarray_push(distances, &distance, false))
			return (ERR_MALLOC_FAILED);

		min_dist = fmin(distance, min_dist);
	}

	get_three_lowests(distances, &lowests);

	if (lowests.x / lowests.z > WORLEY_THRESHOLD)
		*res = BT_STONE;

	distances->nb_cells = 0; // Empty the array

	return (ERR_NONE);
}

unsigned char			generate_cave_map(t_env *env, t_chunk *chunk, unsigned int size)
{
	t_dynarray		points;
	t_dynarray		distances;
	unsigned char	code;

	if (dynarray_init(&distances, sizeof(float), NB_WORLEY_POINTS * SQUARE_SIZE * SQUARE_SIZE))
		return (ERR_MALLOC_FAILED);
	if (dynarray_init(&points, sizeof(t_3dpoint), SQUARE_SIZE * SQUARE_SIZE * NB_WORLEY_POINTS))
		return (ERR_MALLOC_FAILED);

	get_points(env, chunk, &points);

	if (!(chunk->cave_map = allocate_cave_map(size))
		|| init_worley_points(env, chunk, size) != ERR_NONE)
		return (ERR_MALLOC_FAILED);

	//return (ERR_NONE);
	for (unsigned int x = 0; x < size; x++)
	{
		for (unsigned int y = 0; y < size; y++)
		{
			for (unsigned int z = 0; z < size; z++)
				if ((code = worley_noise(env, chunk, &points, &distances, (t_3dpoint){x, y, z}, &chunk->cave_map[x][y][z])))
					return (code);
		}
		
	}

	dynarray_free(&points);
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
