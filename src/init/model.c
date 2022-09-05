#include "../../include/main.h"


static unsigned char	cube(t_dynarray *vertices, vec3 o, unsigned int tid)
{
	int			i;
	t_stride	list_strides[36] = {
		// FRONT ----------------------------------------
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 1, 1 }, 0, tid },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 0, 1 }, 0, tid },
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 1, 0 }, 0, tid },
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 1, 0 }, 0, tid },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 0, 1 }, 0, tid },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 0, 0 }, 0, tid },
		// BACK -----------------------------------------
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 0 }, 1, tid },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 0 }, 1, tid },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 1 }, 1, tid },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 1 }, 1, tid },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 0 }, 1, tid },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 1 }, 1, tid },
		// TOP ------------------------------------------
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 0, 0 }, 2, tid },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 0 }, 2, tid },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 2, tid },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 2, tid },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 1, 0 }, 2, tid },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 1, 1 }, 2, tid },
		// BOTTOM ---------------------------------------
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 0, 0 }, 3, tid },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 3, tid },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 1 }, 3, tid },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 0, 1 }, 3, tid },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 3, tid },
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 1, 1 }, 3, tid },
		// RIGHT ----------------------------------------
		(t_stride){ (vec3){ 0, 0, 0 }, (t_vt){ 0, 0 }, 4, tid },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 1, 0 }, 4, tid },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 4, tid },
		(t_stride){ (vec3){ 0, 1, 0 }, (t_vt){ 0, 1 }, 4, tid },
		(t_stride){ (vec3){ 0, 0, 1 }, (t_vt){ 1, 0 }, 4, tid },
		(t_stride){ (vec3){ 0, 1, 1 }, (t_vt){ 1, 1 }, 4, tid },
		// LEFT -----------------------------------------
		(t_stride){ (vec3){ 1, 0, 1 }, (t_vt){ 0, 0 }, 5, tid },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 5, tid },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 0, 1 }, 5, tid },
		(t_stride){ (vec3){ 1, 1, 1 }, (t_vt){ 0, 1 }, 5, tid },
		(t_stride){ (vec3){ 1, 0, 0 }, (t_vt){ 1, 0 }, 5, tid },
		(t_stride){ (vec3){ 1, 1, 0 }, (t_vt){ 1, 1 }, 5, tid }
	};

	i = -1;
	while (++i < 36) {
		list_strides[i].v = vec_add(list_strides[i].v, o);
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
	int				x, y, z, i, max = 100;

	i = 1;
	x = -1;
	while (++x < max) {
		z = -1;
		while (++z < max) {
			y = -1;
			(void)y;
			// while (++y < max) {
				if ((code = cube(&mesh->vertices, (vec3){ x, 0, z }, 0)) != ERR_NONE)
					return (code);
			// }
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