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
	E_PLAINS,
	E_MOUNTAINS,
	E_DESERT,
	E_SEA,
	E_CAVE,
	E_MAX
};

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
	t_block			***block_map;
	uint8_t			**surface_hmap;
	uint8_t			**sub_hmap;
	int				x_pos, y_pos; // Chunk position (in chunks matrix)
}				t_chunk;

#endif
