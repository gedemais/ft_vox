#include "../../include/main.h"


void		print_fv(t_dynarray *vertices)
{
	t_stride		*elt;
	vec3			v;
	t_vt			t;
	int				i;
	unsigned int	tid;

	for (i = 0; i < vertices->nb_cells; i++) {
		elt = dyacc(vertices, i);

		v	= elt->v;
		t	= elt->t;

		printf("v: [%.1f, %.1f, %.1f]	t: [%.1f, %.1f]\n",
			v.x, v.y, v.z,
			(float)t.u, (float)t.v
		);
	}
}
