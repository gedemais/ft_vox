#include "main.h"


static void		bind_actions_to_keys(t_env *env)
{
	// Function pointers array used to synthetize actions function
	static void	(*keys_fts[NB_KEYS])(t_env *env, int key) = {
		[KEY_EXIT] = exit_vox,
		[KEY_MOVE_CAM_FORWARD] = move_cam,
		[KEY_MOVE_CAM_BACKWARD] = move_cam,
		[KEY_MOVE_CAM_UP] = move_cam,
		[KEY_MOVE_CAM_DOWN] = move_cam,
		[KEY_MOVE_CAM_LEFT] = move_cam,
		[KEY_MOVE_CAM_RIGHT] = move_cam
	};

	// Assignation of actions functions in respect with handled key indices.
	for (unsigned int i = 0; i < KEY_MAX; i++)
		env->keybinds_fts[env->settings.keys[i]] = keys_fts[i];
}

unsigned char	init(t_env *env, int argc, char **argv)
{
	unsigned char code;

	// init env ?
	env->gl.window.fullscreen = false;

	(void)argv;
	(void)argc;
	//if (argc != 2) // Arguments number check
	//	return (ERR_INVALID_ARGC);

	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);
	gen_chunk(env, 0, 0, 128);

	exit(0);
	if ((code = load_settings(env)) != ERR_NONE // Loads settings data from Settings.toml
		|| (code = init_display(env)) != ERR_NONE // Inits display with glad and glfw3
		|| (code = init_shaders(env)) != ERR_NONE) // Load shader programs from .glsl files
		return (code);

	bind_actions_to_keys(env); // Attribute action functions to keys loaded from settings file

	camera(env);

	return (ERR_NONE);
}
