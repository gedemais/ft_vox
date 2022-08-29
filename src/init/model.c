#include "../../include/main.h"


/*
	normals :
	front / back : 0 / 1
	top / bottom : 2 / 3
	right/ left : 4 / 5
*/

static unsigned char	cube(t_dynarray *vertices, vec3 o, unsigned int tid)
{
	int			i;
	t_stride	list_strides[36] = {
		// FRONT ---------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 0, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 0, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 0, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 0, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 0, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 0, tid },
		// BACK --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 1, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 1, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 1, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 1, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 1, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 1, tid },
		// TOP --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 2, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 2, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 2, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 2, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 2, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 2, tid },
		// BOTTOM --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 3, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 3, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 3, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 3, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 3, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 3, tid },
		// RIGHT --------------------------------------------------------------------
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 4, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 4, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 4, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 4, tid },
		(t_stride){ (vec3){ -0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 4, tid },
		(t_stride){ (vec3){ -0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 4, tid },
		// LEFT --------------------------------------------------------------------
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 0.0f }, 5, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 5, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 5, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y,  0.5f + o.z, 1.0f }, (t_vt){ 0.0f, 1.0f }, 5, tid },
		(t_stride){ (vec3){  0.5f + o.x, -0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 0.0f }, 5, tid },
		(t_stride){ (vec3){  0.5f + o.x,  0.5f + o.y, -0.5f + o.z, 1.0f }, (t_vt){ 1.0f, 1.0f }, 5, tid }
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

static unsigned char	many_cubes(t_mesh *mesh)
{
	unsigned char	code;

	if ((code = cube(&mesh->vertices, (vec3){}, TEXTURE_NYAN)) != ERR_NONE)
		return (code);
	if ((code = cube(&mesh->vertices, (vec3){ .x = 1 }, TEXTURE_DEFAULT)) != ERR_NONE)
		return (code);
	if ((code = cube(&mesh->vertices, (vec3){ .z = 1 }, TEXTURE_DARKSOULS)) != ERR_NONE)
		return (code);
	if ((code = cube(&mesh->vertices, (vec3){ .x = 1, .z = 1 }, TEXTURE_NYAN)) != ERR_NONE)
		return (code);
	return (ERR_NONE);
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

		if ((code = many_cubes(mesh)) != ERR_NONE)
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