#ifndef MODEL_H
# define MODEL_H


# include "main.h"
# include "gen.h"

# define MODEL_SCALE	1

enum			e_texture
{
	TEXTURE_HD_WATER,
	TEXTURE_HD_SAND,
	TEXTURE_HD_GRASS,
	TEXTURE_HD_GROUND,
	TEXTURE_HD_STONE,
	TEXTURE_HD_SNOW,
	TEXTURE_HD_GRASS_SIDE,
	TEXTURE_HD_STONE_SIDE,
	TEXTURE_LD_WATER,
	TEXTURE_LD_SAND,
	TEXTURE_LD_GRASS,
	TEXTURE_LD_GROUND,
	TEXTURE_LD_STONE,
	TEXTURE_LD_SNOW,
	TEXTURE_LD_GRASS_SIDE,
	TEXTURE_LD_STONE_SIDE,
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

typedef struct	s_stride
{
	vec3	v;
	t_vt	t;
	float	n, id;
}				t_stride;

typedef struct	s_texture
{
	unsigned char	*ptr;
	unsigned int	w, h;
}				t_texture;

typedef struct	s_uniform
{
	GLint	texturesHD, texturesLD;
	GLint	skybox, campos;
	GLint	light_active, light_gamma, light[LIGHT_SOURCE_MAX][LIGHT_MAX];
	GLint	model, view, projection;
}				t_uniform;

typedef struct	s_gltools
{
	GLuint		vao, vbo;
	GLuint		shader_program;
	GLuint		shader_vertex, shader_fragment;
	t_uniform	uniform;
}				t_gltools;

typedef struct	s_mesh
{
	t_dynarray	vertices;
	t_gltools	gl;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	float		**biomes;
	t_dynarray	chunks;
	mat4		model;
	mat4		mvp;
	float		scale;
	GLuint		gl_textures[TEXTURE_MAX];	// gl's textures' id
	GLuint		gl_tskybox;					// gl's texture for skybox
	t_texture	textures[TEXTURE_SB_MAX];	// textures' ptr
}				t_model;

#endif
