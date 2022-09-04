#ifndef SCENE_H
# define SCENE_H


# include "./main.h"

# define CUBE_SIZE	36

enum			e_texture
{
	TEXTURE_DEFAULT,
	TEXTURE_NYAN,
	TEXTURE_DARKSOULS,
	TEXTURE_MAX
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
	t_texture	textures[TEXTURE_MAX];		// textures' ptr
}				t_model;

#endif
