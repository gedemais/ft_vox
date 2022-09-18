#include "../../include/main.h"


__attribute__((noreturn)) void	exit_vox(t_env *env, int key)
{
	(void)key;

	free_env(env);
	exit(EXIT_SUCCESS);
}

void		event_light(t_env *env, int key)
{
	(void)key;

	t_mesh	*mesh;
	int		i;

	// we reset shadow if light is on or off
	env->light.shadow = false;
	env->light.is_active = !env->light.is_active;
	i = -1;
	while (++i < env->model.meshs.nb_cells -1) {
		mesh = dyacc(&env->model.meshs, i);
		// use program before update uniforms
		glUseProgram(mesh->gl.program);
		// we update the uniforms
		glUniform1i(mesh->gl.uniform.shadow, env->light.shadow);
		glUniform1i(mesh->gl.uniform.light_active, env->light.is_active);
	}
}

void		event_shadow(t_env *env, int key)
{
	(void)key;

	t_mesh	*mesh;
	int		i;

	// si les lights sont désactivées alors shadow est mis à false
	env->light.shadow = env->light.is_active ? !env->light.shadow : false;
	i = -1;
	while (++i < env->model.meshs.nb_cells -1) {
		mesh = dyacc(&env->model.meshs, i);
		// use program before update uniforms
		glUseProgram(mesh->gl.program);
		// we update the uniforms
		glUniform1i(mesh->gl.uniform.shadow, env->light.shadow);
	}
}
