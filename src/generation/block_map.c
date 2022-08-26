# include "main.h"

static t_block	***allocate_block_map(unsigned int size)
{
	t_block	***block_map;

	if (!(block_map = (t_block***)malloc(sizeof(t_block**) * size)))
		return (NULL);

	for (unsigned int x = 0; x < size; x++)
	{

		if (!(block_map[x] = (t_block**)malloc(sizeof(t_block*) * size)))
			return (NULL);

		for (unsigned int y = 0; y < size; y++)
			if (!(block_map[x][y] = (t_block*)malloc(sizeof(t_block) * size)))
				return (NULL);
	}
	return (block_map);
}

static int	switch_block_type(unsigned int z)
{
	if (z < 20)
		return (BT_ROCK);
	else if (z >= 20 && z < 40)
		return (BT_SAND);
	else if (z >= 40 && z < 60)
		return (BT_GROUND);
	else if (z >= 60 && z < 80)
		return (BT_GRASS);
	else if (z >= 80 && z < 100)
		return (BT_SNOW);
	else if (z >= 100)
		return (BT_AIR);
	else
		return (0);
}

static void		gen_blocks(t_chunk *chunk, unsigned int size)
{
	unsigned int	z;

	for (unsigned int x = 0; x < size; x++)
		for (unsigned int y = 0; y < size; y++)
		{
			z = chunk->surface_hmap[x][y];
			chunk->block_map[x][y][z].type = switch_block_type(z);

			z = chunk->sub_hmap[x][y];
			chunk->block_map[x][y][z].type = switch_block_type(z);
		}
}

unsigned char	gen_block_map(t_chunk *chunk, int x_start, int y_start, unsigned int size)
{
	if (!(chunk->block_map = allocate_block_map(size)))
		return (ERR_MALLOC_FAILED);

	(void)x_start;
	(void)y_start;
	gen_blocks(chunk, size);
	return (ERR_NONE);
}
