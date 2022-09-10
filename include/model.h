#ifndef MODEL_H
# define MODEL_H


# include "gen.h"

enum			e_texture
{
	TEXTURE_DEFAULT,
	TEXTURE_NYAN,
	TEXTURE_DARKSOULS,
	TEXTURE_MAX
};

enum			e_skybox
{
	TEXTURE_SB_FRONT = TEXTURE_MAX,
	TEXTURE_SB_BACK,
	TEXTURE_SB_TOP,
	TEXTURE_SB_BOTTOM,
	TEXTURE_SB_RIGHT,
	TEXTURE_SB_LEFT,
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

typedef struct	s_stride
{
	vec3	v;
	t_vt	t;
	int		n;
	float	id;
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
	t_texture	textures[TEXTURE_MAX];		// textures' ptr
}				t_model;

#endif
