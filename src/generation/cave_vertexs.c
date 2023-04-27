#include "main.h"

static unsigned int		check_neighbours(t_chunk *chunk, unsigned char neighbours[N_MAX],
																					unsigned int x,
																					unsigned int y,
																					unsigned int z)
{
	// neighbours coordinates
	const int		n[6][3] =	{
											[N_NORTH] = {0, 0, 1},
											[N_SOUTH] = {0, 0, -1},
											[N_WEST] = {-1, 0, 0},
											[N_EAST] = {1, 0, 0},
											[N_UP] = {0, 1, 0},
											[N_DOWN] = {0, -1, 0}
								};
	unsigned int	index = 0;

	ft_memset(neighbours, 0, sizeof(unsigned char) * 6);
	for (unsigned int i = 0; i < N_MAX; i++)
	{
		x += n[i][0];
		y += n[i][1];
		z += n[i][2];

		if (x >= CHUNK_SIZE
			|| y >= CHUNK_SIZE
			|| z >= CHUNK_SIZE)
			continue;

		if (chunk->cave_map[x][y][z] == BT_STONE)
			neighbours[index++] = (unsigned char)i;
	}

	neighbours[index] = N_MAX;

	return (index);
}

unsigned char			generate_cave_column(t_chunk *chunk, unsigned int x, unsigned int z,
												unsigned int x_start, unsigned int z_start)
{
	unsigned		plain_neighbours = 0;
	unsigned char	neighbours[N_MAX];

	for (unsigned int y = 0; y < CHUNK_SIZE; y++)
	{
		if (chunk->cave_map[x][y][z] != BT_STONE)
		{
			plain_neighbours = check_neighbours(chunk, neighbours, x, y, z);
			for (unsigned int i = 0; i < plain_neighbours; i++)
			{
				if (push_dynarray)
			}
		}
	}
	return (ERR_NONE);
}
