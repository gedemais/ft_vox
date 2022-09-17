#include "../../include/main.h"


static void			load_skybox(t_env *env)
{
	t_texture	*texture;
	int			i;

	glGenTextures(1, &env->model.gl_tskybox);
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

static void			load_textures(t_env *env)
{
	t_texture	*texture;
	int			i;

	// model's textures
	glGenTextures(TEXTURE_MAX + 1, env->model.gl_textures);
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
			load_textures(env);
			load_depthmap(env);
			break ;
		case (1):		// skybox
			load_skybox(env);
			break ;
	}
	return (ERR_NONE);
}
