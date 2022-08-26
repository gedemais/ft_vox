#include "../../include/main.h"


static unsigned char	cube(t_dynarray *vertices)
{
	int			i;
	t_stride	list_strides[36] = {
		// FRONT ---------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		// BACK --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } },
		// TOP --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } },
		// BOTTOM --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } },
		// RIGHT --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ -0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ -0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } },
		// LEFT --------------------------------------------------------------------
		(t_stride){ (vec3){ 0.5f, -0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, 0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 0.0f, 1.0f } },
		(t_stride){ (vec3){ 0.5f, -0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 0.0f } },
		(t_stride){ (vec3){ 0.5f, 0.5f, -0.5f, 1.0f }, (vec3){ 0.0f, 0.0f, 0.0f, 1.0f }, (t_vt){ 1.0f, 1.0f } }
	};

	for (i = 0; i < 36; i++)
		if (dynarray_push(vertices, &list_strides[i], false) < 0)
			return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

static void				set_mesh_center(t_mesh *mesh)
{
	int			i;
	vec3		sum = (vec3){};
	t_stride	*vertices;

	i = -1;
	while (++i < mesh->vertices.nb_cells) {
		vertices = dyacc(&mesh->vertices, i);
		if (vertices == NULL)
			continue ;
		sum = vec_add(sum, vertices->v);
	}
	mesh->center = vec_fdiv(sum, i);
	i = -1;
	while (++i < mesh->vertices.nb_cells) {
		vertices = dyacc(&mesh->vertices, i);
		if (vertices == NULL)
			continue ;
		vertices->v = vec_add(vertices->v, vec_fmult(mesh->center, -1));
		vertices->v.w = 1;
	}
}

unsigned char			model(t_env *env)
{
	unsigned char	code;
	int				i, mesh_max;
	t_mesh			*mesh;

	mesh_max = 1;
	env->model.center = (vec3){};
	if (dynarray_init(&env->model.meshs, sizeof(t_mesh), mesh_max) < 0)
		return (-1);
	i = -1;
	while (++i < mesh_max) {
		mesh = dyacc(&env->model.meshs, i);
		if (mesh == NULL)
			continue ;

		if (dynarray_init(&mesh->vertices, sizeof(t_stride), 36) < 0)
			return (ERR_MALLOC_FAILED);

		if ((code = cube(&mesh->vertices)) != ERR_NONE)
			return (code);

		set_mesh_center(mesh);
		env->model.center = vec_add(env->model.center, mesh->center);

		// print_fv(&mesh->vertices);

		if (dynarray_push(&env->model.meshs, mesh, true) < 0)
			return (ERR_MALLOC_FAILED);
	}
	env->model.center = (vec3){ env->model.center.x / i, env->model.center.y / i, env->model.center.z / i, 1 };
	env->model.scale = 1;
	return (ERR_NONE);
}