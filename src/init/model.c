#include "main.h"

static unsigned char	cube(t_dynarray *vertices, vec3 o)
{
	int		i;
	vec3	list_strides[36] = {
		(vec3){ -1,  1, -1 }, (vec3){ -1, -1, -1 }, (vec3){  1, -1, -1 },
		(vec3){  1, -1, -1 }, (vec3){  1,  1, -1 }, (vec3){ -1,  1, -1 },
		(vec3){ -1, -1,  1 }, (vec3){ -1, -1, -1 }, (vec3){ -1,  1, -1 },
		(vec3){ -1,  1, -1 }, (vec3){ -1,  1,  1 }, (vec3){ -1, -1,  1 },
		(vec3){  1, -1, -1 }, (vec3){  1, -1,  1 }, (vec3){  1,  1,  1 },
		(vec3){  1,  1,  1 }, (vec3){  1,  1, -1 }, (vec3){  1, -1, -1 },
		(vec3){ -1, -1,  1 }, (vec3){ -1,  1,  1 }, (vec3){  1,  1,  1 },
		(vec3){  1,  1,  1 }, (vec3){  1, -1,  1 }, (vec3){ -1, -1,  1 },
		(vec3){ -1,  1, -1 }, (vec3){  1,  1, -1 }, (vec3){  1,  1,  1 },
		(vec3){  1,  1,  1 }, (vec3){ -1,  1,  1 }, (vec3){ -1,  1, -1 },
		(vec3){ -1, -1, -1 }, (vec3){ -1, -1,  1 }, (vec3){  1, -1, -1 },
		(vec3){  1, -1, -1 }, (vec3){ -1, -1,  1 }, (vec3){  1, -1,  1 }
	};

	i = -1;
	while (++i < 36) {
		list_strides[i] = vec_mult(list_strides[i], o);
		if (dynarray_push(vertices, &list_strides[i], false) < 0)
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

static void			set_mesh_center(t_mesh *mesh)
{
	int			i;
	vec3		sum = (vec3){};
	vec3		*vertices;

	i = -1;
	while (++i < mesh->vertices.nb_cells) {
		vertices = dyacc(&mesh->vertices, i);
		sum = vec_add(sum, *vertices);
	}
	sum = vec_fdiv(sum, i);
	i = -1;
	while (++i < mesh->vertices.nb_cells) {
		vertices = dyacc(&mesh->vertices, i);
		*vertices = vec_sub(*vertices, sum);
	}
}

static unsigned char	push_world(t_env *env)
{
	unsigned char	code;
	t_mesh			*mesh;
	t_chunk			*chunk;

	mesh = dyacc(&env->model.meshs, 0);

	if (dynarray_init(&mesh->vertices, sizeof(t_stride), CHUNK_SIZE *  6 * sizeof(t_stride)) < 0)
		return (ERR_MALLOC_FAILED);

	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int y = 0; y < SQUARE_SIZE; y++)
		{
				chunk = &env->model.chunks[x][y];
				for (int i = 0; i < chunk->stride.nb_cells; i++)
					if (dynarray_push(&mesh->vertices, dyacc(&chunk->stride, i), false))
						return (ERR_MALLOC_FAILED);
		}

	if (dynarray_push(&env->model.meshs, mesh, true) < 0)
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

static unsigned char	push_skybox(t_env *env)
{
	unsigned char	code;
	t_mesh			*mesh;
	float			size_skybox;

	mesh = dyacc(&env->model.meshs, env->model.meshs.nb_cells);
	if (dynarray_init(&mesh->vertices, sizeof(vec3), 36) < 0)
		return (ERR_MALLOC_FAILED);
	size_skybox = env->camera.far / 2.0f;
	if ((code = cube(&mesh->vertices, (vec3){ size_skybox, size_skybox, size_skybox })) != ERR_NONE)
		return (code);

	set_mesh_center(mesh);

	if (dynarray_push(&env->model.meshs, mesh, false) < 0)
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

unsigned char			model(t_env *env)
{
	unsigned char	code;

	if (dynarray_init(&env->model.meshs, sizeof(t_mesh), 2) < 0)
		return (ERR_MALLOC_FAILED);

	if ((code = push_world(env)) != ERR_NONE
		|| (code = push_skybox(env)) != ERR_NONE)
		return (code);

	env->model.scale = MODEL_SCALE;
	return (ERR_NONE);
}
