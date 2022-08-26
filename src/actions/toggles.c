#include "main.h"

__attribute__((noreturn)) void	exit_vox(t_env *env, int key)
{
	(void)key;
	free_env(env);
	exit(EXIT_SUCCESS);
}

void		event_light(t_env *env, int key)
{
	(void)env;
	(void)key;
	printf("event light\n");
}

void		event_texture(t_env *env, int key)
{
	(void)env;
	(void)key;
	printf("event texture\n");
}
