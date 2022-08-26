#ifndef GEN_H
# define GEN_H

enum	e_bloc_types
{
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

typedef struct	s_block
{
	uint8_t		type : 3;
}				t_block;

typedef struct	s_chunk
{
	t_block			***block_map;
	uint8_t			**height_map;
	int				x_pos, y_pos; // Chunk position (in chunks matrix)
}				t_chunk;

#endif
