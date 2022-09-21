#ifndef GEN_H
# define GEN_H

# define CHUNK_SIZE			 64 // Size of chunk blocks in cubes
# define MAP_SIZE			 256 // Size of map chunk matrix in chunks
# define BIOME_SIZE			 2 // Size of individual biome matrix in chunks
# define SQUARE_SIZE		 12 // Size of visible chunks matrix for the player (max_limit)
# define SQUARE_TRIGGER_SIZE 2 // Size of visible chunks matrix for the player (max_limit)
# define CACHE_SIZE			 32 // Size of chunks matrix available in the absolute world at startup

enum	e_side_orientation
{
	PO_NORTH,
	PO_SOUTH,
	PO_EST,
	PO_WEST,
	PO_MAX
};

enum	e_bloc_types
{
	BT_WATER,
	BT_SAND,
	BT_GRASS,
	BT_GROUND,
	BT_STONE,
	BT_SNOW,
	BT_GRASS_SIDE,
	BT_SNOW_SIDE,
	BT_MAX
};

enum	e_bloc_type_bounds
{
	BTB_CAVE = 48,
	BTB_BEACH = 56,
	BTB_SOIL = 60,
	BTB_PLAIN = 76,
	BTB_MOUTAIN = 84,
	BTB_HIGH_MOUTAINS = 128,
	BTB_MAX
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
	float		y_offset; // Height offset between surface map and sub map
}				t_biome_params;

typedef struct	s_chunk
{
	t_dynarray		stride;
	uint8_t			**surface_hmap;
	uint8_t			**sub_hmap;
	int				x_start, z_start;
}				t_chunk;

// Biomes Generation Parameters
static const t_biome_params	bgp[TP_MAX] = {
	[TP_SEA] = {0.002f, 6.0f, 0.0f, 0.0f},
	[TP_DESERT] = {0.006f, 6.0f, 0.25f, 0.0f},
	[TP_PLAINS] = {0.01f, 6.0f, 0.5f, 0.0f},
	[TP_FOREST] = {0.015f, 6.0f, 0.75f, 0.0f},
	[TP_MOUNTAINS] = {0.03f, 6.0f, 1.0f, 0.0f}
};

#endif
