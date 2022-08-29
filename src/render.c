#include "../include/main.h"


static void				bind_textures(GLuint *textures, int id)
{
	int	i;

	i = -1;
	while (++i < TEXTURE_MAX) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[id]);
	}
}

static void				draw_mesh(t_env *env)
{
	t_mesh		*mesh;
	GLsizeiptr	size;
	int			i;

	i = -1;
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);
		if (mesh == NULL)
			continue ;
		glBindVertexArray(mesh->vao);
		bind_textures(env->model.gl_textures, TEXTURE_DEFAULT);
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);
		glBindVertexArray(0);
	}
}

static void				set_uniforms(t_env *env)
{
	// update matrices in shaders
	glUniformMatrix4fv(env->gl.uniform.model, 1, GL_FALSE, env->model.model);
	glUniformMatrix4fv(env->gl.uniform.view, 1, GL_FALSE, env->camera.view);
	glUniformMatrix4fv(env->gl.uniform.projection, 1, GL_FALSE, env->camera.projection);
}

static void				mat4_view(t_camera *camera)
{
	if (camera->ground_fixed == true)
		camera->pos.y = 0;
	mat4_lookat(camera->view, camera->pos, vec_add(camera->pos, camera->zaxis), camera->yaxis);
	mat4_inverse(camera->view);
}

static void				mat4_model(t_model *model)
{
	mat4_identity(model->model);
	mat4_scale(model->model, model->scale);
}

static void				mat4_mvp(t_env *env)
{
	mat4_model(&env->model);
	mat4_view(&env->camera);
	mat4_projection(env->camera.projection, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);
}

static unsigned char	render_scene(t_env *env)
{
	glUseProgram(env->gl.shader_program);
	fps(&env->fps, true);
	mat4_mvp(env);
	set_uniforms(env);
	draw_mesh(env);
	return (ERR_NONE);
}

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(255, 255, 255, 1);
	while (!glfwWindowShouldClose(env->gl.window.ptr))
	{
		processInput(env->gl.window.ptr);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if ((code = render_scene(env)) != ERR_NONE)
			return (code);
		glfwSwapBuffers(env->gl.window.ptr);
		glfwPollEvents();
	}
	glfwTerminate();
	return (ERR_NONE);
}
