#ifndef SCENE_H
# define SCENE_H


# include "./main.h"

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
	int				w, h;
}				t_texture;

typedef struct	s_mesh
{
	t_dynarray	vertices;
	vec3		center;
	GLuint		vao, vbo;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	mat4		model;
	vec3		center;
	float		scale;
	GLuint		gl_textures[TEXTURE_MAX];	// gl's textures' id
	GLuint		gl_tskybox;					// gl's texture for skybox
	t_texture	textures[TEXTURE_MAX];		// textures' ptr
}				t_model;

#endif
