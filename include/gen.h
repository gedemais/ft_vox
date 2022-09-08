#ifndef GEN_H
# define GEN_H

enum	e_bloc_types
{
	BT_NONE,
	BT_ROCK,
	BT_SAND,
	BT_GROUND,
	BT_GRASS,
	BT_SNOW,
	BT_WATER,
	BT_AIR,
	BT_CACTUS,
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
	[TP_SEA] = {0.001f, 6.0f, 0.0f},
	[TP_DESERT] = {0.003f, 6.0f, 0.25f},
	[TP_PLAINS] = {0.005f, 6.0f, 0.5f},
	[TP_FOREST] = {0.008f, 6.0f, 0.75f},
	[TP_MOUNTAINS] = {0.015f, 6.0f, 1.0f}
};

#endif
