#include "../../include/main.h"


// v -0.500000 -0.500000 0.500000
// v 0.500000 -0.500000 0.500000
// v -0.500000 0.500000 0.500000
// v 0.500000 0.500000 0.500000
// v -0.500000 0.500000 -0.500000
// v 0.500000 0.500000 -0.500000
// v -0.500000 -0.500000 -0.500000
// v 0.500000 -0.500000 -0.500000

// vt 0.000000 0.000000
// vt 1.000000 0.000000
// vt 0.000000 1.000000
// vt 1.000000 1.000000

// f 1/1 2/2 3/3
// f 3/3 2/2 4/4
// f 3/1 4/2 5/3
// f 5/3 4/2 6/4
// f 5/4 6/3 7/2
// f 7/2 6/3 8/1
// f 7/1 8/2 1/3
// f 1/3 8/2 2/4
// f 2/1 8/2 4/3
// f 4/3 8/2 6/4
// f 7/1 1/2 5/3
// f 5/3 1/2 3/4

static void		cube(t_dynarray *vertices)
{
	dynarray_push(vertices, &(t_stride){
		(vec3) { 0.0f, 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 0.0f, 0.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 1.0f, 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 1.0f, 0.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 1.0f, 1.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 1.0f, 1.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 1.0f, 1.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 1.0f, 1.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 0.0f, 1.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 0.0f, 1.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 0.0f, 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 0.0f, 0.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 0.0f, 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 0.0f, 0.0f }, (uint32_t){ 0 } }, false);
	dynarray_push(vertices, &(t_stride){
		(vec3) { 0.0f, 0.0f, 0.0f, 1.0f }, (vec3) {0.0f, 0.0f, -1.0f, 1.0f },
		(t_vt){ 0.0f, 0.0f }, (uint32_t){ 0 } }, false);
	// ---------------------------------------------------------------------
}

static void	print_fv(t_dynarray *vertices)
{
	t_stride	*elt;
	int			i;

	vec3	v, n;
	t_vt	t;

	for (i = 0; i < vertices->nb_cells; i++) {
		elt = dyacc(vertices, i);

		v = elt->v;
		n = elt->n;
		t = elt->t;

		printf("v: [%.1f, %.1f, %.1f, %.1f] n: [%.1f, %.1f, %.1f, %.1f] t: [%.1f, %.1f]\n",
			v.x, v.y, v.z, v.w,
			n.x, n.y, n.z, n.w,
			t.u, t.v
		);
	}
}

// static void	print_faces(t_dynarray *faces)
// {
// 	t_face			*elt;
// 	unsigned int	i;

// 	for (i = 0; i < faces->nb_cells; i++) {
// 		elt = dyacc(faces, i);

// 		printf("{ %d %d %d -- %d %d %d }\n",
// 			elt->a, elt->b, elt->c,
// 			elt->va, elt->vb, elt->vc);
// 	}
// }

static void		set_mesh_center(t_mesh *mesh)
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

unsigned char	model(t_env *env)
{
	int		i, mesh_max;
	t_mesh	*mesh;

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
		cube(&mesh->vertices);

		set_mesh_center(mesh);
		env->model.center = vec_add(env->model.center, mesh->center);

		print_fv(&mesh->vertices);

		if (dynarray_push(&env->model.meshs, mesh, true) < 0)
			return (ERR_MALLOC_FAILED);
	}
	env->model.center = (vec3){ env->model.center.x / i, env->model.center.y / i, env->model.center.z / i, 1 };
	env->model.scale = 1;
	return (0);
}