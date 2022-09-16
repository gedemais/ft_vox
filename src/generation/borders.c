#include "main.h"

unsigned char	fix_east_border(t_chunk *chunk, t_chunk *neighbour)
{
	vec3 a, b;
	int	delta;

	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		delta = chunk->surface_hmap[CHUNK_SIZE - 1][z] - neighbour->surface_hmap[0][z];
		if (delta == 0)
			continue ;

		if (delta > 0)
		{
			a = (vec3){CHUNK_SIZE + chunk->x_start, chunk->surface_hmap[CHUNK_SIZE - 1][z], chunk->z_start + z + 1};
			b = (vec3){CHUNK_SIZE + chunk->x_start, chunk->surface_hmap[CHUNK_SIZE - 1][z], chunk->z_start + z};
			generate_fall(chunk, a, b, PO_EST, chunk->surface_hmap[CHUNK_SIZE - 1][z], fabs((float)delta));
		}
		else
		{
			a = (vec3){neighbour->x_start, neighbour->surface_hmap[0][z], neighbour->z_start + z + 1};
			b = (vec3){neighbour->x_start, neighbour->surface_hmap[0][z], neighbour->z_start + z};
			generate_fall(neighbour, a, b, PO_WEST, neighbour->surface_hmap[0][z], fabs((float)delta));
		}
	}
	return (ERR_NONE);
}

unsigned char	fix_south_border(t_chunk *chunk, t_chunk *neighbour)
{
	vec3 a, b;
	int	delta;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		delta = chunk->surface_hmap[x][CHUNK_SIZE - 1] - neighbour->surface_hmap[x][0];

		if (delta == 0)
			continue ;

		if (delta > 0)
		{
			a = (vec3){chunk->x_start + x, chunk->surface_hmap[x][CHUNK_SIZE - 1], chunk->z_start + CHUNK_SIZE};
			b = (vec3){chunk->x_start + x + 1, chunk->surface_hmap[x][CHUNK_SIZE - 1], chunk->z_start + CHUNK_SIZE};
			generate_fall(chunk, a, b, PO_SOUTH, chunk->surface_hmap[x][CHUNK_SIZE - 1], fabs((float)delta));
		}
		else
		{
			a = (vec3){neighbour->x_start + x, neighbour->surface_hmap[x][0], neighbour->z_start};
			b = (vec3){neighbour->x_start + x + 1, neighbour->surface_hmap[x][0], neighbour->z_start};
			generate_fall(neighbour, a, b, PO_NORTH, neighbour->surface_hmap[x][0], fabs((float)delta));
		}
	}
	return (ERR_NONE);
}

unsigned char	fix_chunk_borders(t_env *env, int x, int z)
{
	t_chunk			*chunk;
	t_chunk			*neighbour;
	unsigned char	code;

	chunk = &env->model.chunks[x][z];
	if (x + 1 < SQUARE_SIZE)
	{
		printf("fix east border\n");
		fflush(stdout);
		neighbour = &env->model.chunks[x + 1][z];
		if ((code = fix_east_border(chunk, neighbour)))
			return (code);
	}

	if (z + 1 < SQUARE_SIZE)
	{
		printf("fix south border\n");
		fflush(stdout);
		neighbour = &env->model.chunks[x][z + 1];
		if ((code = fix_south_border(chunk, neighbour)))
			return (code);
	}

	return (ERR_NONE);
}
