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

	neighbours[index++] = N_MAX;

	return (index);
}
/*
static unsigned char	cave_sides(t_chunk *chunk, unsigned char orientation, unsigned int x,
																			  unsigned int y,
																			  unsigned int z)
{
	unsigned char	code;

	switch (orientation)
	{
		case N_NORTH:
		break;
		case N_SOUTH:
		break;
		case N_EAST:
		break;
		case N_WEST:
		break;
		case N_:
		break;
		default:
		(void)code;
		break;
	}
	return (ERR_NONE);
}*/

static void	get_sides_heights(t_chunk *chunk, unsigned int heights[N_MAX], unsigned int x, unsigned int *y, unsigned int z)
{
	unsigned char	neighbours[N_MAX] = {};

	while (*y < CHUNK_SIZE && chunk->cave_map[x][*y][z] != BT_STONE)
	{
		if (check_neighbours(chunk, neighbours, x, *y, z) > 0)
			for (unsigned int i = 0; neighbours[i] != N_MAX; i++)
				heights[neighbours[i]]++;
		*y += 1;
	}
}

static unsigned char	generate_cave_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int y, float depth)
{
	const vec3	c = vec_add(a, vec_fmult((vec3){0, 1.0f, 0}, depth));
	const vec3	d = vec_add(b, vec_fmult((vec3){0, 1.0f, 0}, depth));

	const vec3	side_plane_ccw[6]	= {c, a, b, c, b, d};
	// apparement à n'utiliser que quand c'est left side
	const vec3	side_plane_cw[6]	= {c, b, a, c, d, b};

	(void)y;
	return (push_plane(chunk, side_plane_ccw,
		index, BTB_CAVE - 1, depth, true, false));
}

static unsigned char	complete_column(t_chunk *chunk, unsigned int x,
														unsigned int *y,
														unsigned int z,
														vec3 top_plane[6])
{
	unsigned int	heights[N_MAX] = {};
	const int		neighbours[4][2] =	{{0, 1},
										 {0, -1},
										 {1, 0},
										 {-1, 0}};
	// Based on cbacbd order
	const uint8_t	fall_planes[4][2] =	{{2, 1},
										 {0, 5},
										 {2, 5},
										 {1, 0}};
	vec3			a, b;
	unsigned char	code = ERR_NONE;

	get_sides_heights(chunk, heights, x, y, z);

	for (unsigned int i = 0; i < N_MAX - 2; i++)
	{
		if (heights[i])
		{
			a = top_plane[fall_planes[i][0]];
			b = top_plane[fall_planes[i][1]];
			code = generate_cave_fall(chunk, a, b, i, *y, (float)heights[i]);
		}
	}

	return (code);
}


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
			if ((code = generate_top_plane(chunk, x + x_start, y, z + z_start, top_plane))
				|| (code = complete_column(chunk, x, &y, z, top_plane)))
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
