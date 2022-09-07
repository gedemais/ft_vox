#include "main.h"

static	uint8_t	**allocate_height_map(unsigned int size)
{
	uint8_t	**new;

	if (!(new = (uint8_t**)malloc(sizeof(uint8_t*) * size)))
		return (NULL);

	for (unsigned int i = 0; i < size; i++)
	{
		if (!(new[i] = (uint8_t*)malloc(sizeof(uint8_t) * size)))
			return (NULL);
		ft_memset(new[i], 0, sizeof(uint8_t) * size);
	}

	return (new);
}

uint8_t	**generate_height_map(t_biome_params params, int x_start, int y_start, unsigned int size)
{
	uint8_t	**hmap;
	uint8_t	height;
	unsigned int	xbound, ybound;

	if (!(hmap = allocate_height_map(size)))
		return (NULL);

	xbound = x_start + size;
	ybound = y_start + size;

	for (unsigned int x = x_start; x < xbound; x++)
		for (unsigned int y = y_start; y < ybound; y++)
		{
			height = (uint8_t)(perlin2d_a((float)x, (float)y, params.frequency, params.depth) * size);
			hmap[x - x_start][y - y_start] = height;
		}

	return (hmap);
}
