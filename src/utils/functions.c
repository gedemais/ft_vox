#include "../../include/main.h"


// static vec3	get_normals(unsigned int n)
// {
// 	vec3	normal = (vec3){};

// 	switch (n) {
// 		case (0): // front
// 			normal = (vec3){ 0, 0, 1, 1 };
// 			break;
// 		case (1): // back
// 			normal = (vec3){ 0, 0 , -1, 1 };
// 			break;
// 		case (2): // top
// 			normal = (vec3){ 0, 1, 0, 1};
// 			break;
// 		case (3): // bottom
// 			normal = (vec3){ 0, -1, 0, 1 };
// 			break;
// 		case (4): // right
// 			normal = (vec3){ 1, 0, 0, 1 };
// 			break;
// 		case (5): // left
// 			normal = (vec3){ -1, 0, 0, 1 };
// 			break;
// 	}
// 	return (normal);
// }

void		print_fv(t_dynarray *vertices)
{
	t_stride	*elt;
	int			i;

	vec3			v, n;
	t_vt			t;
	unsigned int	tid;

	for (i = 0; i < vertices->nb_cells; i++) {
		elt = dyacc(vertices, i);

		v	= elt->v;
		t	= elt->t;
		tid	= elt->tid;

		n	= elt->n;
		// n	= get_normals(elt->n);

		printf("v: [%.1f, %.1f, %.1f] t: [%.1f, %.1f] tid: [%d] n: [%.1f, %.1f, %.1f]\n",
			v.x, v.y, v.z,
			t.u, t.v, tid,
			n.x, n.y, n.z
		);
	}
}
