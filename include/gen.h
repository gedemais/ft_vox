#ifndef GEN_H
# define GEN_H

enum	e_bloc_types
{
	BT_WATER,
	BT_SAND,
	BT_GRASS,
	BT_GROUND,
	BT_STONE,
	BT_SNOW,
	BT_MAX
};

enum e_topography_type
{
	TP_SEA,
	TP_DESERT,
	TP_PLAINS,
	TP_FOREST,
	TP_MOUNTAINS,
	TP_MAX
};

typedef struct	s_biome_params
{
	float		frequency; // Mean delta between each block
	float		depth; // Granularity of generation (kind of a zoom)
	float		bound; // 0-1 value used to LERP the biomes params
}				t_biome_params;

typedef struct	s_block
{
	uint8_t		type : 3;
}				t_block;

typedef	struct	s_group_unit
{
	unsigned char	x;
	unsigned char	y;
	unsigned char	z;
}				t_group_unit;

typedef struct	s_chunk
{
	t_dynarray		stride;
	uint8_t			**surface_hmap;
	uint8_t			**sub_hmap;
	int				x_pos, y_pos; // Chunk position (in chunks matrix)
}				t_chunk;

// Biomes Generation Parameters
static const t_biome_params	bgp[TP_MAX] = {
	[TP_SEA] = {0.002f, 6.0f, 0.0f},
	[TP_DESERT] = {0.006f, 6.0f, 0.25f},
	[TP_PLAINS] = {0.01f, 6.0f, 0.5f},
	[TP_FOREST] = {0.015f, 6.0f, 0.75f},
	[TP_MOUNTAINS] = {0.03f, 6.0f, 1.0f}
};

#endif
