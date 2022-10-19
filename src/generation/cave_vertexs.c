#include "main.h"
/*
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
}*/

unsigned char			generate_cave_column(t_chunk *chunk, unsigned int x, unsigned int z,
													 unsigned int x_start, unsigned int z_start)
{
	unsigned int	y = 0;
	uint8_t			current;
	unsigned char	code;
	vec3			top_plane[6];

	current = chunk->cave_map[x][y][z];
	while (y < CHUNK_SIZE)
	{
		while (y < CHUNK_SIZE && chunk->cave_map[x][y][z] == current)
			y++;

		if (y == CHUNK_SIZE)
			break;

		if (current == BT_STONE)
		{
			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane)))
				return (code);

			current = 0;
			return (ERR_NONE);
		}
		else
			current = BT_STONE;

		y++;
	}
	return (ERR_NONE);
}
