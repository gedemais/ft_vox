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

	env->light.is_active = !env->light.is_active;
	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		mesh = dyacc(&env->model.meshs, i);
		glUseProgram(mesh->gl.shader_program);
		glUniform1i(mesh->gl.uniform.light_active, env->light.is_active);
	}
}
