#include "main.h"

__attribute__((noreturn)) void	exit_vox(t_env *env, int key)
{
	(void)key;
	free_env(env);
	exit(EXIT_SUCCESS);
}

void	toggle_rotation(t_env *env, int key)
{
	(void)key;
	env->settings.rotation = !env->settings.rotation;
}

void	toggle_texture(t_env *env, int key)
{
	(void)key;
	env->settings.textured = !env->settings.textured;
}

void	change_rotation_speed(t_env *env, int key)
{
	if (key == gl_keys_values[env->settings.keys[KEY_INCREASE_ROTATION_SPEED]])
		env->settings.rotation_speed += ROTATION_SPEED_DELTA;
	else
		env->settings.rotation_speed -= ROTATION_SPEED_DELTA;
}
