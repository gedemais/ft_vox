#ifndef MODEL_H
# define MODEL_H


# include "main.h"
# include "gen.h"
# include "camera.h"

# define MODEL_SCALE	1
# define SB_ROT_SPEED	10 // sun's rotation speed

enum			e_texture
{
	TEXTURE_WATER,
	TEXTURE_SAND,
	TEXTURE_GRASS,
	TEXTURE_GROUND,
	TEXTURE_STONE,
	TEXTURE_SNOW,
	TEXTURE_GRASS_SIDE,
	TEXTURE_SNOW_SIDE,
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
	int		uv : 3;
	int		fs : 6; // fall_size
	int		n : 3;
	int		bt : 3;
	int		pad : 1;
}				__attribute__((__packed__)) t_stride;

typedef struct	s_texture
{
	unsigned char	*ptr;
	unsigned int	w, h;
}				t_texture;

typedef struct	s_shadows
{
	GLuint		depthmap;
	GLuint		fbo;
	t_camera	camera;
}				t_shadows;

typedef struct	s_uniform
{
	GLint	time;
	GLint	textures;
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
	t_chunk		chunks[SQUARE_SIZE][SQUARE_SIZE];
	mat4		model;
	mat4		mvp;
	float		scale;
	GLuint		gl_textures[TEXTURE_MAX];	// gl's textures' id
	GLuint		gl_tskybox;					// gl's texture for skybox
	t_texture	textures[TEXTURE_SB_MAX];	// textures' ptr
	t_shadows	shadows;
}				t_model;

#endif
