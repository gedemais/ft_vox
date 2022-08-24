#include "main.h"

__attribute__((noreturn)) void	exit_vox(t_env *env, int key)
{
	(void)key;
	free_env(env);
	exit(EXIT_SUCCESS);
}
