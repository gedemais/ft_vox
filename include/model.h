#ifndef MODEL_H
# define MODEL_H


# include "gen.h"

enum			e_texture
{
	TEXTURE_HD_WATER,
	TEXTURE_HD_SAND,
	TEXTURE_HD_GRASS,
	TEXTURE_HD_GROUND,
	TEXTURE_HD_STONE,
	TEXTURE_HD_SNOW,
	TEXTURE_HD_GRASS_SIDE,
	TEXTURE_HD_SNOW_SIDE,
	TEXTURE_LD_WATER,
	TEXTURE_LD_SAND,
	TEXTURE_LD_GRASS,
	TEXTURE_LD_GROUND,
	TEXTURE_LD_STONE,
	TEXTURE_LD_SNOW,
	TEXTURE_LD_GRASS_SIDE,
	TEXTURE_LD_SNOW_SIDE,
	TEXTURE_MAX
};

enum			e_skybox
{
	TEXTURE_SB_PX = TEXTURE_MAX,
	TEXTURE_SB_PY,
	TEXTURE_SB_PZ,
	TEXTURE_SB_NX,
	TEXTURE_SB_NY,
	TEXTURE_SB_NZ,
	TEXTURE_SB_MAX
};

enum			e_normals
{
	N_NORTH,
	N_SOUTH,
	N_WEST,
	N_EAST,
	N_UP,
	N_DOWN,
	N_MAX
};

typedef	struct	s_vertex_texture
{
	float	u, v;
}				t_vt;

typedef struct __attribute__((__packed__)) s_stride
{
	short	x : 16;
	short	y : 16;
	short	z : 16;
	uint8_t	uv : 3;
	uint8_t	fs : 6; // fall_size
	uint8_t	n : 3;
	uint8_t	bt : 3;
	uint8_t	pad : 1;
}				t_stride;


typedef struct	s_texture
{
	unsigned char	*ptr;
	unsigned int	w, h;
}				t_texture;

typedef struct	s_mesh
{
	t_dynarray	vertices;
	GLuint		vao, vbo;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	float		**biomes;
	t_chunk		chunks[SQUARE_SIZE][SQUARE_SIZE];
	mat4		model;
	mat4		mvp;
	float		scale;
	GLuint		gl_textures[TEXTURE_MAX];	// gl's textures' id
	GLuint		gl_tskybox;					// gl's texture for skybox
	t_texture	textures[TEXTURE_SB_MAX];	// textures' ptr
}				t_model;

#endif
