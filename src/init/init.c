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
	const char	*path[TEXTURE_MAX] = {
		"./resources/texture.bmp",
		"./resources/nyan.bmp",
		"./resources/darksouls.bmp"
	};
	t_texture	*texture;
	int			i;

	i = -1;
	while (++i < TEXTURE_MAX) {
		texture = &env->model.textures[i];
		loadBMP(path[i], &texture->ptr, &texture->w, &texture->h); // Load image from path
		if (texture->ptr == NULL)
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

static unsigned char	init_env(t_env *env)
{
	unsigned char	code;

	if ((code = images(env)) != ERR_NONE) // load the textures
		return (code);

	env->gl.window.fullscreen = false;

	model(env);
	light(env); // light after model because we set the sun's light pos with the model center

	return (ERR_NONE);
}

unsigned char			init(t_env *env, int argc, char **argv)
{
	unsigned char code;

	if ((code = load_settings(env)) != ERR_NONE // Loads settings data from Settings.toml
		|| (code = init_display(env)) != ERR_NONE // Inits display with glad and glfw3
		|| (code = init_env(env)) != ERR_NONE // init main env
		|| (code = init_shaders(env)) != ERR_NONE) // init shaders after model because we need to buffer each mesh
		return (code);

	(void)argv;
	(void)argc;
	//if (argc != 2) // Arguments number check
	//	return (ERR_INVALID_ARGC);

	bind_actions_to_keys(env); // Attribute action functions to keys loaded from settings file

	camera(env); // camera after load settings cause we need ww and wh

	return (ERR_NONE);
}
