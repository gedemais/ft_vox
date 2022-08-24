#ifndef SCENE_H
# define SCENE_H

# define OBJ_SPEED 0.01f

typedef float mat4[16];

typedef	struct		s_vertex_texture
{
	float	u;
	float	v;
}					t_vt;

typedef struct		s_stride
{
	t_vec3d		v;
	t_vec3d		n;
	t_vt		t;
	uint32_t	txt;
}					t_stride;

typedef struct		s_texture
{
	unsigned char	*img_data;
	unsigned int	gl_id;
	int				w;
	int				h;
	char			pad[4];
}					t_texture;

typedef struct	s_camera_matrices
{
	mat4		model;
	mat4		view;
	mat4		projection;
	mat4		mvp;
}				t_cam_mat;

typedef	struct	s_camera
{
	t_cam_mat	mats;
	t_vec3d		pos;
	t_vec3d		target;
	t_vec3d		dir;
	t_vec3d		up;
	t_vec3d		right;
	float		roll;
	float		pitch;
	float		yaw;
	float		aspect_ratio;
	float		fnear;
	float		ffar;
	float		fdelta;
	float		fovd;
	float		fovr;
	uint8_t		pad[4];
}				t_cam;

#endif
