#ifndef LIGHT_H
# define LIGHT_H


# include "./main.h"

enum			e_light
{
	LIGHT_ACTIVE,
	LIGHT_GAMMA,
	LIGHT_POSITION,
	LIGHT_DIRECTION,
	LIGHT_COLOR,
	LIGHT_AMBIENT,
	LIGHT_DIFFUSE,
	LIGHT_SPECULAR,
	LIGHT_MAX
};

typedef struct	s_light
{
	bool	is_active;
	float	gamma;
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
}				t_light;


#endif
