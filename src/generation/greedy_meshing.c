# include "main.h"

// Find contigous areas of blocks by iterating over the whole map.
// If a block top is not contained in an already repertoried area and form with
// its neighbours an area superior to sqrt(size) blocks, then it is considered
// as a new area and stored into areas.

static bool				is_in_group(t_dynarray *group, unsigned int x, unsigned int y)
{
	t_group_unit	*u;

	for (int i = 0; i < group->nb_cells; i++)
	{
		u = dyacc(group, i);
		if (u->x == (unsigned char)x && (unsigned char)u->y == y)
			return (true);
	}
	return (false);
}

static unsigned char	explore_neighbours(t_chunk *chunk, t_dynarray *group, unsigned int x, unsigned int y, unsigned int size)
{
	t_group_unit	g;
	unsigned int	n_x, n_y;
	unsigned int	neighbours[8][2] = {
		{-1, 0},
		{1, 0},
		{0, -1},
		{0, 1},
		{-1, -1},
		{1, 1},
		{0, -1},
		{0, 1},
	};

	g = (t_group_unit){(unsigned char)x, (unsigned char)y, (unsigned char)chunk->surface_hmap[x][y]};

	if (dynarray_push(group, &g, false))
		return (ERR_MALLOC_FAILED);

	for (unsigned int i = 0; i < 8; i++)
	{
		n_x = x + neighbours[i][0];
		n_y = y + neighbours[i][1];

		if (n_x >= size || n_y >= size)
			continue ;

		if (chunk->surface_hmap[x][y] == chunk->surface_hmap[n_x][n_y] && is_in_group(group, n_x, n_y) == false)
			explore_neighbours(chunk, group, n_x, n_y, size);
	}
	return (ERR_NONE);
}

static unsigned char	find_top_areas(t_chunk *chunk, t_dynarray *top_areas, unsigned int size)
{
	t_dynarray		group;
	unsigned int	z;

	if (dynarray_init(top_areas, sizeof(t_dynarray), size))
		return (ERR_MALLOC_FAILED);

	for (unsigned int x = 1; x < size - 1; x += 2)
		for (unsigned int y = 1; y < size - 1; y += 2)
		{
			if (dynarray_init(&group, sizeof(t_group_unit), size))
				return (ERR_MALLOC_FAILED);

			//if (is_in_group(top_areas, x, y))
			//	continue ;
			explore_neighbours(chunk, &group, x, y, size);

			//if (group.nb_cells < sqrt(size))
			//	continue ;

			dynarray_free(&group);

			if (group.nb_cells == 2 || group.nb_cells >= 4)
			if (dynarray_push(top_areas, &group, false))
				return (ERR_MALLOC_FAILED);
			//chunk->block_map[x][y][z].type;
		}
	return (ERR_NONE);
}

unsigned char			greedy_meshing(t_chunk *chunk, int x_start, int y_start, unsigned int size)
{
	unsigned char	code;
	t_dynarray		top_areas;

	(void)x_start;
	(void)y_start;

	if ((code = find_top_areas(chunk, &top_areas, size)))
		return (code);

	return (ERR_NONE);
}
