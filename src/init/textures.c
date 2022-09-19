#include "../../include/main.h"


static unsigned char	get_textures_uniforms(t_gltools *gl, int i)
{
	char	target[256], *ai;

	if ((ai = ft_itoa(i)) == NULL)
		return (ERR_MALLOC_FAILED);
	ft_memset(target, 0, sizeof(char) * 256);
	ft_strcat(target, "vTexture_");
	ft_strcat(target, ai);
	gl->uniform.textures[i] = glGetUniformLocation(gl->program, target);
	ft_strdel(&ai);
	return (ERR_NONE);
}

unsigned char			textures_uniforms(t_mesh *mesh)
{
	unsigned char	code;
	int				i;

	i = -1;
	while (++i < TEXTURE_MAX + 1) {
		// get uniforms
		if ((code = get_textures_uniforms(&mesh->gl, i)) != ERR_NONE)
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

static void			load_textures(t_env *env)
{
	t_texture	*texture;
	int			i;

	// model's textures
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
			load_textures(env);
			load_depthmap(env);
			break ;
		case (1):		// skybox
			glGenTextures(1, &env->model.gl_tskybox);
			load_skybox(env);
			break ;
	}
	return (ERR_NONE);
}
