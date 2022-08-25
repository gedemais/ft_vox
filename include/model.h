#ifndef SCENE_H
# define SCENE_H

# define OBJ_SPEED 0.01f

# include "./main.h"


typedef	struct	s_vertex_texture
{
	float	u;
	float	v;
}				t_vt;

typedef struct	s_stride
{
	vec3		v;
	vec3		n;
	t_vt		t;
	uint32_t	txt;
}				t_stride;

typedef struct	s_texture
{
	unsigned char	*img_data;
	unsigned int	gl_id;
	int				w;
	int				h;
	char			pad[4];
}				t_texture;

typedef struct	s_camera
{
	mat4		view, projection;
	float		pitch, yaw;
	float		fov, near, far, ratio;
	vec3		pos, yaxis, zaxis;
	float		speed, tspeed;
	bool		ground_fixed;
}				t_camera;

typedef struct	s_mesh
{
	t_dynarray		vertices;
	t_dynarray		faces;
	vec3			center;
	unsigned int	texture;
	GLuint			ebo, vao, vbo;
}				t_mesh;

typedef struct	s_model
{
	t_dynarray	meshs;
	mat4		model;
	vec3		center;
	vec3		trans, rot;
	float		scale, strans, srot;
}				t_model;


#endif
