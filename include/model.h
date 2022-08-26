#ifndef SCENE_H
# define SCENE_H


# include "./main.h"


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
	vec3		v;
	vec3		n;
	t_vt		t;
}				t_stride;

typedef struct	s_texture
{
	unsigned char	*ptr;
	unsigned int	id, w, h;
	char			pad[4];
}				t_texture;

typedef struct	s_mesh
{
	t_dynarray		vertices;
	vec3			center;
	GLuint			vao, vbo;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	mat4		model;
	vec3		center;
	float		scale;
	t_texture	texture;
}				t_model;


#endif
