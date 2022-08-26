#include "main.h"

static	uint8_t	**allocate_height_map(unsigned int size)
{
	uint8_t	**new;

	if (!(new = (uint8_t**)malloc(sizeof(uint8_t*) * size)))
		return (NULL);

	for (unsigned int i = 0; i < size; i++)
		if (!(new[i] = (uint8_t*)malloc(sizeof(uint8_t) * size)))
			return (NULL);

	return (new);
}

uint8_t	**generate_height_map(unsigned int x_start, unsigned int y_start, unsigned int size)
{
	uint8_t	**hmap;
	unsigned int	xbound, ybound;

	if (!(hmap = allocate_height_map(size)))
		return (NULL);

	xbound = x_start + size;
	ybound = y_start + size;

	for (unsigned int x = x_start; x < xbound; x++)
		for (unsigned int y = y_start; y < ybound; y++)
			hmap[x][y] = (uint8_t)(perlin2d_a((float)x, (float)y, 0.2f, 6.0f) * size);

	return (hmap);
}
