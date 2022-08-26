#include "../../include/main.h"


void		print_fv(t_dynarray *vertices)
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

void		print_faces(t_dynarray *faces)
{
	int		i;
	t_face	*elt;

	for (i = 0; i < faces->nb_cells; i++) {
		elt = dyacc(faces, i);

		printf("{ %d %d %d -- %d %d %d }\n",
			elt->a, elt->b, elt->c,
			elt->va, elt->vb, elt->vc);
	}
}
