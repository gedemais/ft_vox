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
	gl->uniform.light[i][LIGHT_POSITION] = glGetUniformLocation(gl->program, target);
	build_target(target, "].dir", ai);
	gl->uniform.light[i][LIGHT_DIRECTION] = glGetUniformLocation(gl->program, target);
	build_target(target, "].color", ai);
	gl->uniform.light[i][LIGHT_COLOR] = glGetUniformLocation(gl->program, target);
	build_target(target, "].ambient", ai);
	gl->uniform.light[i][LIGHT_AMBIENT] = glGetUniformLocation(gl->program, target);
	build_target(target, "].diffuse", ai);
	gl->uniform.light[i][LIGHT_DIFFUSE] = glGetUniformLocation(gl->program, target);
	build_target(target, "].specular", ai);
	gl->uniform.light[i][LIGHT_SPECULAR] = glGetUniformLocation(gl->program, target);
	build_target(target, "].intensity", ai);
	gl->uniform.light[i][LIGHT_INTENSITY] = glGetUniformLocation(gl->program, target);
	ft_strdel(&ai);
	return (ERR_NONE);
}

unsigned char			light_uniforms(t_mesh *mesh, t_light *light)
{
	unsigned char	code;
	int				i;

	// get uniforms
	mesh->gl.uniform.light_active = glGetUniformLocation(mesh->gl.program, "light.is_active");
	mesh->gl.uniform.light_gamma = glGetUniformLocation(mesh->gl.program, "light.gamma");
	mesh->gl.uniform.shadow = glGetUniformLocation(mesh->gl.program, "light.shadow");
	// consume uniforms
	glUniform1i(mesh->gl.uniform.light_active, light->is_active);
	glUniform1f(mesh->gl.uniform.light_gamma, light->gamma);
	glUniform1f(mesh->gl.uniform.shadow, light->shadow);
	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		// get uniforms
		if ((code = get_light_uniforms(&mesh->gl, i)) != ERR_NONE)
			return (code);
		// consume uniforms
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_POSITION], 1, (GLfloat *)&light->sources[i].pos);
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_DIRECTION], 1, (GLfloat *)&light->sources[i].dir);
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_COLOR], 1, (GLfloat *)&light->sources[i].color);
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_AMBIENT], 1, (GLfloat *)&light->sources[i].ambient);
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_DIFFUSE], 1, (GLfloat *)&light->sources[i].diffuse);
		glUniform3fv(mesh->gl.uniform.light[i][LIGHT_SPECULAR], 1, (GLfloat *)&light->sources[i].specular);
		glUniform1f(mesh->gl.uniform.light[i][LIGHT_INTENSITY], light->sources[i].intensity);
	}
	return (ERR_NONE);
}

// ====================================================================

static void				init_player(t_light_source *source)
{
	source->pos			= (vec3){};
	source->dir			= (vec3){ 1, 1, 1 };
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
	source->dir			= vec_normalize(env->camera.pos);
	source->color		= (vec3){ 1, 1, 1 };
	source->ambient		= (vec3){ 0.66f, 0.66f, 0.66f };
	source->diffuse		= (vec3){ 0.66f, 0.66f, 0.66f };
	source->specular	= (vec3){ 0.33f, 0.25f, 0.33f };

	printf("light dir : %f %f %f\n", source->dir.x , source->dir.y, source->dir.z);
}

void					light(t_env *env)
{
	t_light	*light;

	light = &env->light;
	light->is_active = false;
	light->shadow = false;
	init_player(&light->sources[LIGHT_SOURCE_PLAYER]);
	init_sun(env, &light->sources[LIGHT_SOURCE_SUN]);
}
