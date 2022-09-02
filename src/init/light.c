#include "../../include/main.h"

static void				build_target(char target[256], char *str, char *ai)
{
	ft_memset(target, 0, sizeof(char) * 256);
	ft_strcat(target, "light_sources[");
	ft_strcat(target, ai);
	ft_strcat(target, str);
}

static unsigned char	get_light_uniforms(t_gltools *gl, int i)
{
	char	target[256], *ai;

	if ((ai = ft_itoa(i)) == NULL)
		return (ERR_MALLOC_FAILED);
	build_target(target, "].pos", ai);
	gl->uniform.light[i][LIGHT_POSITION] = glGetUniformLocation(gl->shader_program, target);
	build_target(target, "].dir", ai);
	gl->uniform.light[i][LIGHT_DIRECTION] = glGetUniformLocation(gl->shader_program, target);
	build_target(target, "].color", ai);
	gl->uniform.light[i][LIGHT_COLOR] = glGetUniformLocation(gl->shader_program, target);
	build_target(target, "].ambient", ai);
	gl->uniform.light[i][LIGHT_AMBIENT] = glGetUniformLocation(gl->shader_program, target);
	build_target(target, "].diffuse", ai);
	gl->uniform.light[i][LIGHT_DIFFUSE] = glGetUniformLocation(gl->shader_program, target);
	build_target(target, "].specular", ai);
	gl->uniform.light[i][LIGHT_SPECULAR] = glGetUniformLocation(gl->shader_program, target);
	ft_strdel(&ai);
	return (ERR_NONE);
}

unsigned char			light_uniforms(t_env *env)
{
	unsigned char	code;
	int				i;

	// get uniforms
	env->gl.uniform.light_active = glGetUniformLocation(env->gl.shader_program, "light.is_active");
	env->gl.uniform.light_gamma = glGetUniformLocation(env->gl.shader_program, "light.gamma");
	// consume uniforms
	glUniform1i(env->gl.uniform.light_active, env->light.is_active);
	glUniform1f(env->gl.uniform.light_gamma, env->light.gamma);
	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		// get uniforms
		if ((code = get_light_uniforms(&env->gl, i)) != ERR_NONE)
			return (code);
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
	source->pos			= (vec3){ 5, 2, 0 };				// z is reverse
	source->dir			= (vec3){ 0, 0, 1 };				// unused yet
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	source->diffuse		= (vec3){ 0.75f, 0.75f, 0.75f };
	source->specular	= (vec3){ .5, .5, .5 };
}

 static void			init_sun(t_light_source *source)
{
	source->pos			= (vec3){ 10, 100, 10 };				// z is reverse
	source->dir			= (vec3){ 0, 0, 1 };				// unused yet
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	// source->diffuse		= (vec3){ 10000, 10000, 10000 };
	source->diffuse		= (vec3){};
	source->specular	= (vec3){ 1, 1, 1 };
}

void					light(t_light *light)
{
	light->is_active	= false;
	light->gamma		= 2.0f;
	init_player(&light->sources[LIGHT_SOURCE_PLAYER]);
	init_sun(&light->sources[LIGHT_SOURCE_SUN]);
}
