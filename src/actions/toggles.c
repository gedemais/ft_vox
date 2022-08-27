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
	env->light.is_active = !env->light.is_active;
	//printf("%d\n", env->light.is_active);
	glUniform1i(env->gl.uniform.light[LIGHT_ACTIVE], env->light.is_active);
}

void		event_texture(t_env *env, int key)
{
	(void)env;
	(void)key;
	printf("event texture\n");
}
