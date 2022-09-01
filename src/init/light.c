#include "../../include/main.h"


static char				*build_target(char *str, char *ai)
{
	char	*s1, *s2;

	if ((s1 = ft_strjoin("light_sources[", ai)) == NULL)
		return (NULL);
	s2 = ft_strfjoin(s1, str, 1);
	return (s2);
}

static unsigned char	quit_light_uniforms(char *ai)
{
	ft_strdel(&ai);
	return (ERR_MALLOC_FAILED);
}

unsigned char			light_uniforms(t_env *env)
{
	int		i;
	char	*target, *ai;

	// get uniforms
	env->gl.uniform.light_active = glGetUniformLocation(env->gl.shader_program, "light.is_active");
	env->gl.uniform.light_gamma = glGetUniformLocation(env->gl.shader_program, "light.gamma");

	// consume uniforms
	glUniform1i(env->gl.uniform.light_active, env->light.is_active);
	glUniform1f(env->gl.uniform.light_gamma, env->light.gamma);

	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		if ((ai = ft_itoa(i)) == NULL)
			return (ERR_MALLOC_FAILED);
		
		// get uniforms
		if ((target = build_target("].pos", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_POSITION] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		if ((target = build_target("].dir", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_DIRECTION] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		if ((target = build_target("].color", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_COLOR] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		if ((target = build_target("].ambient", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_AMBIENT] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		if ((target = build_target("].diffuse", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_DIFFUSE] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		if ((target = build_target("].specular", ai)) == NULL)
			return (quit_light_uniforms(ai));
		env->gl.uniform.light[i][LIGHT_SPECULAR] = glGetUniformLocation(env->gl.shader_program, target);
		printf("target : %s\n", target);
		ft_strdel(&target);

		// consume uniforms
		glUniform3fv(env->gl.uniform.light[i][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[i].pos);
		glUniform3fv(env->gl.uniform.light[i][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[i].dir);
		glUniform3fv(env->gl.uniform.light[i][LIGHT_COLOR], 1, (GLfloat *)&env->light.sources[i].color);
		glUniform3fv(env->gl.uniform.light[i][LIGHT_AMBIENT], 1, (GLfloat *)&env->light.sources[i].ambient);
		glUniform3fv(env->gl.uniform.light[i][LIGHT_DIFFUSE], 1, (GLfloat *)&env->light.sources[i].diffuse);
		glUniform3fv(env->gl.uniform.light[i][LIGHT_SPECULAR], 1, (GLfloat *)&env->light.sources[i].specular);
	}

	return (ERR_NONE);
}

static void				init_player(t_light_source *source)
{
	source->pos			= (vec3){ 2, 2, 2 };				// z is reverse
	source->dir			= (vec3){ 0, 0, 1 };				// unused yet
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	source->diffuse		= (vec3){ 0.75f, 0.75f, 0.75f };
	source->specular	= (vec3){ 1, 1, 1 };
}

static void				init_sun(t_light_source *source)
{
	source->pos			= (vec3){ 10, 2, 10 };				// z is reverse
	source->dir			= (vec3){ 0, 0, 1 };				// unused yet
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	source->diffuse		= (vec3){ 0.75f, 0.75f, 0.75f };
	source->specular	= (vec3){ 1, 1, 1 };
}

void					light(t_light *light)
{
	light->is_active	= false;
	light->gamma		= 2.0f;
	init_player(&light->sources[LIGHT_SOURCE_PLAYER]);
	init_sun(&light->sources[LIGHT_SOURCE_SUN]);
}
