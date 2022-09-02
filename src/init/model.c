#include "../../include/main.h"


static void				update_vertex(t_stride *vertex, vec3 o, float step)
{
	vertex->v = vec_add(vertex->v, o);
	(void)step;
	// vertex->t.u = (vertex->t.u / (float)TEXTURE_MAX) + step;
}

static unsigned char	cube(t_dynarray *vertices, vec3 o, unsigned int tid)
{
	int			i;
	float		step = (1 / (float)(TEXTURE_MAX)) * tid;
	t_stride	list_strides[36] = {
		// FRONT ----------------------------------------
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 1, 1 }, 0, 0 },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 0, 1 }, 0, 0 },
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 1, 0 }, 0, 0 },
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 1, 0 }, 0, 0 },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 0, 1 }, 0, 0 },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 0, 0 }, 0, 0 },
		// BACK -----------------------------------------
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 0 }, 1, 1 },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 0 }, 1, 1 },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 1 }, 1, 1 },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 1 }, 1, 1 },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 0 }, 1, 1 },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 1 }, 1, 1 },
		// TOP ------------------------------------------
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 0 }, 2, 2 },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 0 }, 2, 2 },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 2, 2 },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 2, 2 },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 0 }, 2, 2 },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 1, 1 }, 2, 2 },
		// BOTTOM ---------------------------------------
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 0, 0 }, 3, 0 },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 3, 0 },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 1 }, 3, 0 },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 1 }, 3, 0 },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 3, 0 },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 1 }, 3, 0 },
		// RIGHT ----------------------------------------
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 0, 0 }, 4, 1 },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 1, 0 }, 4, 1 },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 4, 1 },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 4, 1 },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 1, 0 }, 4, 1 },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 1, 1 }, 4, 1 },
		// LEFT -----------------------------------------
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 0, 0 }, 5, 2 },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 5, 2 },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 0, 1 }, 5, 2 },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 0, 1 }, 5, 2 },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 5, 2 },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 1, 1 }, 5, 2 }
	};

	i = -1;
	while (++i < 36) {
		update_vertex(&list_strides[i], o, step);
		if (dynarray_push(vertices, &list_strides[i], false) < 0)
			return (ERR_MALLOC_FAILED);
	}
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
	}
}

static unsigned char	many_cubes(t_mesh *mesh)
{
	unsigned char	code;
	int				x, y, z, i, max = 10;

	i = 1;
	x = -1;
	while (++x < max) {
		z = -1;
		while (++z < max) {
			y = -1;
			while (++y < max) {
				if ((code = cube(&mesh->vertices, (vec3){ x, y % i, z }, 0)) != ERR_NONE)
					return (code);
			}
		}
		++i;
	}
	printf("cubes : %d || vertices %d\n", mesh->vertices.nb_cells / 36, mesh->vertices.nb_cells);
	return (ERR_NONE);
}

unsigned char			model(t_env *env)
{
	unsigned char	code;
	int				i, mesh_max = 1;
	t_mesh			*mesh;

	env->model.center = (vec3){};
	if (dynarray_init(&env->model.meshs, sizeof(t_mesh), mesh_max) < 0)
		return (ERR_MALLOC_FAILED);
	i = -1;
	while (++i < mesh_max) {
		mesh = dyacc(&env->model.meshs, i);
		if (mesh == NULL)
			continue ;

		if (dynarray_init(&mesh->vertices, sizeof(t_stride), CUBE_SIZE) < 0)
			return (ERR_MALLOC_FAILED);

		if ((code = many_cubes(mesh)) != ERR_NONE)
			return (code);

		set_mesh_center(mesh);
		env->model.center = vec_add(env->model.center, mesh->center);

		// print_fv(&mesh->vertices);

		if (dynarray_push(&env->model.meshs, mesh, true) < 0)
			return (ERR_MALLOC_FAILED);
	}
	env->model.center = (vec3){ env->model.center.x / i, env->model.center.y / i, env->model.center.z / i };
	env->model.scale = 1;
	return (ERR_NONE);
}