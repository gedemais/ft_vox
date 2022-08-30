#ifndef SCENE_H
# define SCENE_H


# include "./main.h"

enum			e_texture
{
	TEXTURE_DEFAULT,
	TEXTURE_DEFAULT2,
	TEXTURE_DEFAULT3,
	TEXTURE_MAX
};

typedef	struct	s_face
{
	uint32_t	a, b, c;
	uint32_t	va, vb, vc;
}				t_face;

typedef	struct	s_vertex_texture
{
	float	u, v;
}				t_vt;

typedef struct	s_stride
{
	vec3			v;
	t_vt			t;
}				t_stride;

typedef struct	s_texture
{
	unsigned char	*ptr;
	int				id, w, h;
	char			pad[4];
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
	GLuint		gl_texture;		// gl's textures' id
	t_texture	texture;		// textures' ptr
}				t_model;

#endif
