#include "../../include/main.h"


static void				bind_actions_to_keys(t_env *env)
{
	// Function pointers array used to synthetize actions function
	static void	(*keys_fts[NB_KEYS])(t_env *env, int key) = {
		[KEY_EXIT] = exit_vox,
		[KEY_MOVE_CAM_FORWARD] = move_cam,
		[KEY_MOVE_CAM_BACKWARD] = move_cam,
		[KEY_MOVE_CAM_UP] = move_cam,
		[KEY_MOVE_CAM_DOWN] = move_cam,
		[KEY_MOVE_CAM_LEFT] = move_cam,
		[KEY_MOVE_CAM_RIGHT] = move_cam,
		[KEY_LIGHT] = event_light,
		[KEY_TEXTURE] = event_texture
	};

	// Assignation of actions functions in respect with handled key indices.
	for (unsigned int i = 0; i < KEY_MAX; i++)
		env->keybinds_fts[env->settings.keys[i]] = keys_fts[i];
}

static unsigned char	images(t_env *env)
{
	const char	path[256] = "./resources/nyan.bmp";
	t_texture	*texture;

	texture = &env->model.texture;
	if ((texture->ptr = load_bmp(path, &texture->w, &texture->h)) == NULL) // Load image from path
		return (ERR_MALLOC_FAILED);
	return (ERR_NONE);
}

unsigned char			init(t_env *env, int argc, char **argv)
{
	unsigned char code;

	// init env ?
	env->gl.window.fullscreen = false;

	if ((code = images(env)) != ERR_NONE)
		return (code);

	model(env);

	(void)argv;
	(void)argc;
	//if (argc != 2) // Arguments number check
	//	return (ERR_INVALID_ARGC);

	if ((code = load_settings(env)) != ERR_NONE // Loads settings data from Settings.toml
		|| (code = init_display(env)) != ERR_NONE // Inits display with glad and glfw3
		|| (code = init_shaders(env)) != ERR_NONE) // Load shader programs from .glsl files
		return (code);

	bind_actions_to_keys(env); // Attribute action functions to keys loaded from settings file

	camera(env);

	return (ERR_NONE);
}
