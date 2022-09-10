#ifndef LIGHT_H
# define LIGHT_H


# include "./main.h"

enum			e_light_sources
{
	LIGHT_SOURCE_PLAYER,
	LIGHT_SOURCE_SUN,
	LIGHT_SOURCE_MAX
};

enum			e_light_tools
{
	LIGHT_POSITION,
	LIGHT_DIRECTION,
	LIGHT_COLOR,
	LIGHT_AMBIENT,
	LIGHT_DIFFUSE,
	LIGHT_SPECULAR,
	LIGHT_INTENSITY,
	LIGHT_MAX
};

typedef struct	s_light_source
{
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
	float	intensity;
}				t_light_source;

typedef struct	s_light
{
	bool			is_active;
	float			gamma;
	t_light_source	sources[LIGHT_SOURCE_MAX];
}				t_light;


#endif
