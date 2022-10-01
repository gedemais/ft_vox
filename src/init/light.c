#include "../../include/main.h"

static void				build_target(char target[256], char *str, char *ai)
{
	ft_memset(target, 0, sizeof(char) * 256);
	ft_strcat(target, "light_sources[");
	ft_strcat(target, ai);
	ft_strcat(target, str);
}

static unsigned char	get_light_uniforms(t_env *env, int i)
{
	char	target[256], *ai;

	if ((ai = ft_itoa(i)) == NULL)
		return (ERR_MALLOC_FAILED);
	build_target(target, "].pos", ai);
	env->model.uniforms.light[i][LIGHT_POSITION] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].dir", ai);
	env->model.uniforms.light[i][LIGHT_DIRECTION] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].color", ai);
	env->model.uniforms.light[i][LIGHT_COLOR] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].ambient", ai);
	env->model.uniforms.light[i][LIGHT_AMBIENT] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].diffuse", ai);
	env->model.uniforms.light[i][LIGHT_DIFFUSE] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].specular", ai);
	env->model.uniforms.light[i][LIGHT_SPECULAR] = glGetUniformLocation(env->model.program, target);
	build_target(target, "].intensity", ai);
	env->model.uniforms.light[i][LIGHT_INTENSITY] = glGetUniformLocation(env->model.program, target);
	ft_strdel(&ai);
	return (ERR_NONE);
}

unsigned char			light_uniforms(t_env *env)
{
	t_light			*light = &env->light;
	unsigned char	code;
	int				i;

	// get uniforms
	env->model.uniforms.light_active = glGetUniformLocation(env->model.program, "light.is_active");
	env->model.uniforms.light_gamma = glGetUniformLocation(env->model.program, "light.gamma");
	env->model.uniforms.shadow = glGetUniformLocation(env->model.program, "light.shadow");
	// consume uniforms
	glUniform1i(env->model.uniforms.light_active, light->is_active);
	glUniform1f(env->model.uniforms.light_gamma, light->gamma);
	glUniform1f(env->model.uniforms.shadow, light->shadow);
	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		// get uniforms
		if ((code = get_light_uniforms(env, i)) != ERR_NONE)
			return (code);
		// consume uniforms
		glUniform3fv(env->model.uniforms.light[i][LIGHT_POSITION], 1, (GLfloat *)&light->sources[i].pos);
		glUniform3fv(env->model.uniforms.light[i][LIGHT_DIRECTION], 1, (GLfloat *)&light->sources[i].dir);
		glUniform3fv(env->model.uniforms.light[i][LIGHT_COLOR], 1, (GLfloat *)&light->sources[i].color);
		glUniform3fv(env->model.uniforms.light[i][LIGHT_AMBIENT], 1, (GLfloat *)&light->sources[i].ambient);
		glUniform3fv(env->model.uniforms.light[i][LIGHT_DIFFUSE], 1, (GLfloat *)&light->sources[i].diffuse);
		glUniform3fv(env->model.uniforms.light[i][LIGHT_SPECULAR], 1, (GLfloat *)&light->sources[i].specular);
		glUniform1f(env->model.uniforms.light[i][LIGHT_INTENSITY], light->sources[i].intensity);
	}
	return (ERR_NONE);
}

// ====================================================================

static void				init_player(t_env *env, t_light_source *source)
{
	source->pos			= env->camera.pos;
	source->dir			= env->camera.zaxis;
	// ----------------------------------
	source->color		= (vec3){ 0.33f, 0.33f, 0.33f };
	source->ambient		= (vec3){ 0.25f, 0.25f, 0.25f };
	source->diffuse		= (vec3){ 0.5f, 0.5f, 0.5f };
	source->specular	= (vec3){ 0.33f, 0.25f, 0.33f };
}

static void				init_sun(t_env *env, t_light_source *source)
{
	float	e;

	e = env->camera.far / 2.0f;
	source->pos			= (vec3){ 0.85f, 0, -1 };
	source->pos			= (vec3){ source->pos.x * e, 16, source->pos.z * e };
	source->base_pos	= source->pos;
	source->dir			= (vec3){ -source->pos.x, -source->pos.y, -source->pos.z };
	source->dir			= vec_sub(env->camera.pos, source->dir);
	source->base_dir	= source->dir;
	// ----------------------------------
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.66f, 0.66f, 0.66f };
	source->diffuse		= (vec3){ 0.66f, 0.66f, 0.66f };
	source->specular	= (vec3){ 0.33f, 0.25f, 0.33f };
}

void					light(t_env *env)
{
	t_light	*light;

	light = &env->light;
	light->is_active = false;
	light->shadow = false;
	init_player(env, &light->sources[LIGHT_SOURCE_PLAYER]);
	init_sun(env, &light->sources[LIGHT_SOURCE_SUN]);
}
