#include "main.h"


const char				*textures_paths[TEXTURE_SB_MAX] = {
	// TEXTURES HD
	[TEXTURE_HD_WATER]		= "./resources/HD/gravel.png",
	[TEXTURE_HD_SAND]		= "./resources/HD/sand.png",
	[TEXTURE_HD_GRASS]		= "./resources/HD/grass.png",
	[TEXTURE_HD_GROUND]		= "./resources/HD/ground.png",
	[TEXTURE_HD_STONE]		= "./resources/HD/stone.png",
	[TEXTURE_HD_SNOW]		= "./resources/HD/snow.png",
	[TEXTURE_HD_GRASS_SIDE]	= "./resources/HD/grass_side.png",
	[TEXTURE_HD_STONE_SIDE]	= "./resources/HD/stone_side.png",
	// TEXTURES LD
	[TEXTURE_LD_WATER]		= "./resources/LD/gravel.png",
	[TEXTURE_LD_SAND]		= "./resources/LD/sand.png",
	[TEXTURE_LD_GRASS]		= "./resources/LD/grass.png",
	[TEXTURE_LD_GROUND]		= "./resources/LD/ground.png",
	[TEXTURE_LD_STONE]		= "./resources/LD/stone.png",
	[TEXTURE_LD_SNOW]		= "./resources/LD/snow.png",
	[TEXTURE_LD_GRASS_SIDE]	= "./resources/LD/grass_side.png",
	[TEXTURE_LD_STONE_SIDE]	= "./resources/LD/stone_side.png",
	// TEXTURES SKYBOX
	[TEXTURE_SB_PX]			= "./resources/skybox/px.png",
	[TEXTURE_SB_PY]			= "./resources/skybox/py.png",
	[TEXTURE_SB_PZ]			= "./resources/skybox/pz.png",
	[TEXTURE_SB_NX]			= "./resources/skybox/nx.png",
	[TEXTURE_SB_NY]			= "./resources/skybox/ny.png",
	[TEXTURE_SB_NZ]			= "./resources/skybox/nz.png"
};

static unsigned char	load_textures(t_env *env)
{
	t_texture		*txt;
	unsigned int	err;
	int				i;

	i = -1;
	while (++i < TEXTURE_SB_MAX) {
		txt = &env->model.textures[i];
		err = lodepng_decode32_file(&txt->ptr, &txt->w, &txt->h, textures_paths[i]);
		if (err) {
			ft_putendl_fd(lodepng_error_text(err), 2);
			return (ERR_TEXTURE_LOADING_FAILED);
		}
	}
	return (ERR_NONE);
}

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
	};

	// Assignation of actions functions in respect with handled key indices.
	for (unsigned int i = 0; i < KEY_MAX; i++)
		env->keybinds_fts[env->settings.keys[i]] = keys_fts[i];
}

static void				gl_options(void)
{
	//  DEPTH BUFFER
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// CULLING : we only draw front face in clock-wise order
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);

	// GAMA CORRECTION
	// glEnable(GL_FRAMEBUFFER_SRGB);

	// ANTI ALIASING
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

	// BLENDING => for water cube WIP
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

unsigned char			init(t_env *env, int argc, char **argv)
{
	unsigned char code;

	if ((code = load_settings(env)) != ERR_NONE // Loads settings data from Settings.toml
		|| (code = load_textures(env)) != ERR_NONE
		|| (code = init_world(env, argc, argv)) != ERR_NONE
		|| (code = init_display(env)) != ERR_NONE // Inits display with glad and glfw3
		|| (code = init_meshs(env)) != ERR_NONE)  // init shaders after model because we need to buffer each mesh
		return (code);


	bind_actions_to_keys(env); // Attribute action functions to keys loaded from settings file

	camera(env); // camera after load settings cause we need ww and wh

	gl_options();

	return (ERR_NONE);
}
