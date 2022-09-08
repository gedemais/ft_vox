#include "../../include/main.h"


static unsigned char	cube(t_dynarray *vertices, vec3 o, unsigned int tid, bool skybox)
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
		if (skybox)
			list_strides[i].v = vec_mult(list_strides[i].v, o);
		else
			list_strides[i].v = vec_add(list_strides[i].v, o);
		if (dynarray_push(vertices, &list_strides[i], false) < 0)
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

unsigned char	push_world(t_env *env)
{
	unsigned char	code;
	t_mesh			*mesh;
	t_chunk			*chunk;

	mesh = dyacc(&env->model.meshs, 0);

	if (dynarray_init(&mesh->vertices, sizeof(t_stride),
		env->model.chunks.nb_cells * CHUNK_SIZE * CHUNK_SIZE *  6 * sizeof(t_stride)) < 0)
		return (ERR_MALLOC_FAILED);

	for (int c = 0; c < env->model.chunks.nb_cells; c++)
	{
		chunk = dyacc(&env->model.chunks, c);
		for (int i = 0; i < chunk->stride.nb_cells; i++)
			if (dynarray_push(&mesh->vertices, dyacc(&chunk->stride, i), false))
				return (ERR_MALLOC_FAILED);
	}

	//print_fv(&mesh->vertices);

	if (dynarray_push(&env->model.meshs, mesh, true) < 0)
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

unsigned char	push_skybox(t_env *env)
{
	unsigned char	code;
	t_mesh			*mesh;

	mesh = dyacc(&env->model.meshs, env->model.meshs.nb_cells);
	if (dynarray_init(&mesh->vertices, sizeof(t_stride), 36) < 0)
		return (ERR_MALLOC_FAILED);
	if ((code = cube(&mesh->vertices, (vec3){ 100, 100, 100 }, 0, true)) != ERR_NONE)
		return (code);

	// print_fv(&mesh->vertices);

	if (dynarray_push(&env->model.meshs, mesh, true) < 0)
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

unsigned char			model(t_env *env)
{
	unsigned char	code;

	if (dynarray_init(&env->model.meshs, sizeof(t_mesh), 2) < 0)
		return (ERR_MALLOC_FAILED);

	if ((code = push_world(env)) != ERR_NONE)
		//|| (code = push_skybox(env)) != ERR_NONE)
		return (code);

	// push_skybox(env);

	env->model.scale = 1;
	return (ERR_NONE);
}
