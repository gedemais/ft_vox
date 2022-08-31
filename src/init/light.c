#include "../../include/main.h"


void			light(t_light *light)
{
	light->is_active	= false;
	light->gamma		= 2.0f;
	light->pos			= (vec3){ 2, 2, 2 };				// z is reverse
	light->dir			= (vec3){ 0, 0, 1 };				// unused yet
	light->color		= (vec3){ 1, 1, 1 };
	light->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	light->diffuse		= (vec3){ 0.75f, 0.75f, 0.75f };
	light->specular		= (vec3){ 1, 1, 1 };
}
