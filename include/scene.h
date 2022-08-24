#ifndef SCENE_H
# define SCENE_H

# define OBJ_SPEED 0.01f

typedef float mat4[16];

typedef	struct	s_mesh
{
	t_dynarray	faces;
	t_vec3d		o;
	char		*name;
}				t_mesh;

typedef	struct		s_color
{
	float	r;
	float	g;
	float	b;
	float	a;
}					t_color;

typedef	struct		s_vertex_texture
{
	float	u;
	float	v;
}					t_vt;

typedef struct		s_stride
{
	t_vec3d		v;
	t_color		c;
	t_vt		t;
}					t_stride;

typedef struct		s_texture
{
	unsigned char	*img_data;
	unsigned int	gl_id;
	int				w;
	int				h;
	char			pad[4];
}					t_texture;

typedef	struct		s_material
{
	t_texture		texture;
	t_color			color;
	char			*name;
}					t_mtl;

typedef	struct	s_face
{
	uint32_t		a;
	uint32_t		b;
	uint32_t		c;
	uint32_t		va;
	uint32_t		vb;
	uint32_t		vc;
}				t_face;

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

typedef struct	s_scene
{
	t_cam		cam;
	t_dynarray	meshs;
	t_dynarray	vertexs;
	t_dynarray	vertexs_txt;
	t_dynarray	faces;
	t_dynarray	used_mtls;
	t_dynarray	mtls;
	float		ct; // Color / Texture ratio
	char		pad[4];
}				t_scene;

#endif
