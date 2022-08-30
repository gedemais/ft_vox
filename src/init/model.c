#include "../../include/main.h"


unsigned char	cube(t_dynarray *vertices, vec3 o, unsigned int tid)
{
	int			i;
	t_stride	list_strides[CUBE_SIZE] = {
		(t_stride){ (vec3){ 0 + o.x, 1 + o.y, 1 + o.z }, (t_vt){ 0, 0 }, tid, (vec3){ 1, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 1 + o.y, 1 + o.z }, (t_vt){ 1, 0 }, tid, (vec3){ 1, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 0 + o.y, 1 + o.z }, (t_vt){ 0, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 0 + o.y, 1 + o.z }, (t_vt){ 1, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 0 + o.y, 0 + o.z }, (t_vt){ 1, 0 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 1 + o.y, 1 + o.z }, (t_vt){ 0, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 1 + o.y, 0 + o.z }, (t_vt){ 0, 0 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 1 + o.y, 1 + o.z }, (t_vt){ 1, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 1 + o.y, 0 + o.z }, (t_vt){ 1, 0 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 0 + o.y, 1 + o.z }, (t_vt){ 0, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 0 + o.y, 0 + o.z }, (t_vt){ 0, 0 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 0 + o.y, 0 + o.z }, (t_vt){ 1, 0 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 0 + o.x, 1 + o.y, 0 + o.z }, (t_vt){ 0, 1 }, tid, (vec3){ 0, 0, 0} },
		(t_stride){ (vec3){ 1 + o.x, 1 + o.y, 0 + o.z }, (t_vt){ 1, 1 }, tid, (vec3){ 0, 0, 0} }
	};

	i = -1;
	while (++i < CUBE_SIZE)
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
		return (ERR_MALLOC_FAILED);
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
	env->model.center = (vec3){ env->model.center.x / i, env->model.center.y / i, env->model.center.z / i };
	env->model.scale = 1;
	return (ERR_NONE);
}