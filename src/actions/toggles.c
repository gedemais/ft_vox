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

	env->light.shadow = false;
	env->light.is_active = !env->light.is_active;
	i = -1;
	while (++i < env->model.meshs.nb_cells -1) {
		mesh = dyacc(&env->model.meshs, i);
		glUseProgram(mesh->gl.program);
		glUniform1i(mesh->gl.uniform.light_active, env->light.is_active);
	}
}

void		event_shadow(t_env *env, int key)
{
	(void)key;

	if (env->light.is_active == false) {
		env->light.shadow = false;
		return ;	
	}
	env->light.shadow = !env->light.shadow;
}
