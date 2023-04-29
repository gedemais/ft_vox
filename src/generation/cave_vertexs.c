#include "main.h"

static const vec3	n[6] =	{
								[N_NORTH] = (vec3){0, 0, 1},
								[N_SOUTH] = (vec3){0, 0, -1},
								[N_WEST] = (vec3){-1, 0, 0},
								[N_EAST] = (vec3){1, 0, 0},
								[N_UP] = (vec3){0, 1, 0},
								[N_DOWN] = (vec3){0, -1, 0}
								};

static unsigned int		check_neighbours(t_chunk *chunk, unsigned char neighbours[N_MAX],
																					unsigned int x,
																					unsigned int y,
																					unsigned int z)
{
	// neighbours coordinates
	unsigned int	index = 0;
	unsigned int	tx, ty, tz;

	ft_memset(neighbours, 0, sizeof(unsigned char) * 6);
	for (unsigned int i = 0; i < N_MAX; i++)
	{
		tx = x + n[i].x;
		ty = y + n[i].y;
		tz = z + n[i].z;

		if (tx >= CAVE_DEPTH
			|| ty >= CAVE_DEPTH
			|| tz >= CAVE_DEPTH)
			continue;

		if (chunk->cave_map[tx][ty][tz] == BT_STONE)
			neighbours[index++] = (unsigned char)i;
	}

	return (index);
}

unsigned char			generate_cave_column(t_chunk *chunk, unsigned int x, unsigned int z,
												unsigned int x_start, unsigned int z_start)
{
	unsigned		plain_neighbours = 0;
	unsigned char	neighbours[N_MAX];
	unsigned char	code;

	vec3			a, b;

	for (unsigned int y = 0; y < CAVE_DEPTH; y++)
	{
		if (chunk->cave_map[x][y][z] != BT_STONE)
		{
			plain_neighbours = check_neighbours(chunk, neighbours, x, y, z);
			for (unsigned int i = 0; i < plain_neighbours; i++)
			{
				if (neighbours[i] == N_UP)
				{
					vec3 top_plane[6];
					if ((code = generate_top_plane(chunk, x_start + x, y + 1, z_start + z, top_plane)))
						return (code);
				}

				else if (neighbours[i] == N_DOWN)
				{
					vec3 top_plane[6];
					if ((code = generate_top_plane(chunk, x_start + x, y, z_start + z, top_plane)))
						return (code);
				}

				else if (neighbours[i] == N_WEST)
				{
					a = (vec3){x_start + x, y + 1, z_start + z};
					b = vec_add(a, (vec3){0, 0, 1});
					if ((code = generate_fall(chunk, a, b, neighbours[i], y + 1, 1.0f)))
						return (code);
				}

				else if (neighbours[i] == N_EAST)
				{
					a = (vec3){x_start + x + 1, y + 1, z_start + z};
					b = vec_add(a, (vec3){0, 0, 1});
					if ((code = generate_fall(chunk, a, b, neighbours[i], y + 1, 1.0f)))
						return (code);
				}

				else if (neighbours[i] == N_NORTH)
				{
					a = (vec3){x_start + x, y + 1, z_start + z + 1};
					b = vec_add(a, (vec3){1, 0, 0});
					if ((code = generate_fall(chunk, a, b, neighbours[i], y + 1, 1.0f)))
						return (code);
				}

				else if (neighbours[i] == N_SOUTH)
				{
					a = (vec3){x_start + x, y + 1, z_start + z};
					b = vec_add(a, (vec3){1, 0, 0});
					if ((code = generate_fall(chunk, a, b, neighbours[i], y + 1, 1.0f)))
						return (code);
				}

			}
		}
	}
	return (ERR_NONE);
}
