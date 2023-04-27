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

static unsigned char	init_worley_points(t_3dpoint points[NB_WORLEY_DOTS], unsigned int size)
{
	for (unsigned int i = 0; i < NB_WORLEY_DOTS; i++)
	{
		points[i].x = rand() % size;
		points[i].y = rand() % size;
		points[i].z = rand() % size;
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

void	sort_distances(float arr[NB_WORLEY_DOTS], int n)
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
}

static unsigned char	worley_noise(t_3dpoint points[NB_WORLEY_DOTS], float distances[NB_WORLEY_DOTS], t_3dpoint current, uint8_t *res)
{
	float	min_dist = INFINITY;

	for (unsigned int i = 0; i < NB_WORLEY_DOTS; i++)
	{
		distances[i] = dist(points[i], current);
		min_dist = fmin(distances[i], min_dist);
	}

	sort_distances(distances, NB_WORLEY_DOTS);

	if (distances[0] / distances[2] > WORLEY_THRESHOLD)
		*res = BT_STONE;

	return (ERR_NONE);
}

unsigned char			generate_cave_map(t_chunk *chunk, unsigned int size)
{
	t_3dpoint		points[NB_WORLEY_DOTS];
	float			distances[NB_WORLEY_DOTS];
	unsigned char	code;

	if (!(chunk->cave_map = allocate_cave_map(size))
		|| init_worley_points(points, size) != ERR_NONE)
		return (ERR_MALLOC_FAILED);

	//return (ERR_NONE);
	for (unsigned int x = 0; x < size; x++)
		for (unsigned int y = 0; y < size; y++)
			for (unsigned int z = 0; z < size; z++)
				if ((code = worley_noise(points, distances, (t_3dpoint){x, y, z}, &chunk->cave_map[x][y][z])))
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
		usleep(100000);
		printf("\n");
	}

	fflush(stdout);*/

	return (ERR_NONE);
}
