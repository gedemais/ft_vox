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
	unsigned int	north_in, east_in, north_out, east_out = 0;
	unsigned int	y = 0;
	unsigned char	current, code;
	vec3			a, b;
	vec3	top_plane[6];

	while (y < CHUNK_SIZE)
	{
		north_in = 0;
		north_out = 0;
		east_in = 0;
		east_out = 0;
		current = chunk->cave_map[x][y][z];
		while (y < CHUNK_SIZE && chunk->cave_map[x][y][z] == current)
		{
			if (current == BT_STONE)
			{
				north_out += (z < CHUNK_SIZE - 1 && chunk->cave_map[x][y][z + 1] != BT_STONE) ? 1 : 0;
				east_out += (x < CHUNK_SIZE - 1 && chunk->cave_map[x + 1][y][z] != BT_STONE) ? 1 : 0;
			}
			else
			{
				north_in += (z < CHUNK_SIZE - 1 && chunk->cave_map[x][y][z + 1] == BT_STONE) ? 1 : 0;
				east_in += (x < CHUNK_SIZE - 1 && chunk->cave_map[x + 1][y][z] == BT_STONE) ? 1 : 0;
			}
			y++;
		}

		if (y == CHUNK_SIZE)
			break;
	//	printf("%u | %u | %u | %u\n", north_in, east_in, north_out, east_out);

		if (north_out > 0)
		{
			a = (vec3){x + x_start, y, z + z_start + 1};
			b = (vec3){x + x_start + 1, y, z + z_start + 1};
			if ((code = generate_deep_fall(chunk, a, b, PO_NORTH, north_out, y)))
				return (code);
			north_out = 0;
		}

		if (east_out > 0)
		{
			a = (vec3){x + x_start + 1, y, z + z_start + 1};
			b = (vec3){x + x_start + 1, y, z + z_start};
			if ((code = generate_deep_fall(chunk, a, b, PO_EST, east_out, y)))
				return (code);
			east_out = 0;
		}

		if (north_in > 0)
		{
			a = (vec3){x + x_start, y, z + z_start + 1};
			b = (vec3){x + x_start + 1, y, z + z_start + 1};
			if ((code = generate_deep_fall(chunk, a, b, PO_SOUTH, north_in, y)))
				return (code);
			north_in = 0;
		}

		if (east_in > 0)
		{
			a = (vec3){x + x_start + 1, y, z + z_start + 1};
			b = (vec3){x + x_start + 1, y, z + z_start};
			if ((code = generate_deep_fall(chunk, a, b, PO_WEST, east_in, y)))
				return (code);
			east_in = 0;
		}

		if ((code = generate_top_plane(chunk, x_start + x, y, z_start + z, top_plane)))
			return (code);

		//y++;
	}

	return (ERR_NONE);
}
