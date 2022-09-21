#include "../../include/main.h"


static unsigned char	get_textures_uniforms(t_env *env, t_gltools *gl, int i)
{
	char	target[256], *ai;

	if ((ai = ft_itoa(i)) == NULL)
		return (ERR_MALLOC_FAILED);
	ft_memset(target, 0, sizeof(char) * 256);
	ft_strcat(target, "vTexture_");
	ft_strcat(target, ai);
	gl->uniform.textures[i] = glGetUniformLocation(env->model.program, target);
	ft_strdel(&ai);
	return (ERR_NONE);
}

unsigned char			textures_uniforms(t_env *env, t_mesh *mesh)
{
	unsigned char	code;
	int				i;

	i = -1;
	while (++i < TEXTURE_MAX + 1) {
		// get uniforms
		if ((code = get_textures_uniforms(env, &mesh->gl, i)) != ERR_NONE)
			return (code);
		glUniform1i(mesh->gl.uniform.textures[i], i);
	}
	return (ERR_NONE);
}

// ====================================================================

static void			load_skybox(t_env *env)
{
	t_texture	*texture;
	int			i;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env->model.gl_tskybox);
	i = -1;
	while (++i < 6) {
		texture = &env->model.textures[TEXTURE_SB_PX + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->ptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

static void			load_model(t_env *env)
{
	t_texture	*texture;
	int			i;

	i = -1;
	while (++i < TEXTURE_MAX) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, env->model.gl_textures[i]);

		texture = &env->model.textures[i];
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->ptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

static void			load_depthmap(t_env *env)
{
	// depthmap at last position
	glActiveTexture(GL_TEXTURE0 + TEXTURE_MAX);
	glBindTexture(GL_TEXTURE_2D, env->model.gl_textures[TEXTURE_MAX]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		env->window.w, env->window.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float	border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

unsigned char		mount_textures(t_env *env, char type)
{
	switch (type) {
		case (0):		// model + depthmap
			// +1 for depthmap :: we stock all textures' id in gl_textures
			glGenTextures(TEXTURE_MAX + 1, env->model.gl_textures);
			load_model(env);
			load_depthmap(env);
			break ;
		case (1):		// skybox
			glGenTextures(1, &env->model.gl_tskybox);
			load_skybox(env);
			break ;
	}
	return (ERR_NONE);
}

// ====================================================================

const char				*textures_paths[TEXTURE_SB_MAX] = {
	// TEXTURES HD
	[TEXTURE_WATER]			= "./resources/textures/water.png",
	[TEXTURE_SAND]			= "./resources/textures/sand.png",
	[TEXTURE_GRASS]			= "./resources/textures/grass.png",
	[TEXTURE_GROUND]		= "./resources/textures/ground.png",
	[TEXTURE_STONE]			= "./resources/textures/stone.png",
	[TEXTURE_SNOW]			= "./resources/textures/snow.png",
	[TEXTURE_GRASS_SIDE]	= "./resources/textures/grass_side.png",
	[TEXTURE_SNOW_SIDE]		= "./resources/textures/stone_side.png",
	// TEXTURES SKYBOX
	[TEXTURE_SB_PX]			= "./resources/skybox/px.png",
	[TEXTURE_SB_PY]			= "./resources/skybox/py.png",
	[TEXTURE_SB_PZ]			= "./resources/skybox/pz.png",
	[TEXTURE_SB_NX]			= "./resources/skybox/nx.png",
	[TEXTURE_SB_NY]			= "./resources/skybox/ny.png",
	[TEXTURE_SB_NZ]			= "./resources/skybox/nz.png"
};

unsigned char			load_textures(t_env *env)
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
