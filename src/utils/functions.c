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

void		printfv(t_dynarray *vertices)
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
