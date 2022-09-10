#include "main.h"

static void				set_uniforms(t_env *env, t_mesh *mesh, bool skybox)
{
	mat4	m;
	vec3	tmp;
	float	e;

	e = (glfwGetTime() * SUN_SPEED) / 100;
	mat4_identity(m);
	mat4_yrotation(m, e);
	mat4_translate(m, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);

	glUseProgram(mesh->gl.shader_program);
	if (skybox == true) {
		mat4_multiply(env->model.model, m);
	} else {
		// update campos in shaders
		glUniform3fv(mesh->gl.uniform.campos, 1, (GLfloat *)&env->camera.pos);

		// update lightpos in shaders
		env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
		env->light.sources[LIGHT_SOURCE_PLAYER].dir = vec_fmult(env->camera.zaxis, -1);
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].pos);
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].dir);
		tmp = mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].pos);
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_SUN][LIGHT_POSITION], 1, (GLfloat *)&tmp);
	}
	// update matrices in shaders
	glUniformMatrix4fv(mesh->gl.uniform.model, 1, GL_FALSE, env->model.model);
	glUniformMatrix4fv(mesh->gl.uniform.view, 1, GL_FALSE, env->camera.view);
	glUniformMatrix4fv(mesh->gl.uniform.projection, 1, GL_FALSE, env->camera.projection);
}

static void				draw_mesh(t_env *env)
{
	t_mesh		*mesh;
	GLsizeiptr	size;
	int			i, j;

	i = -1;
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);
		if (mesh == NULL)
			continue ;

		set_uniforms(env, mesh, i == env->model.meshs.nb_cells - 1);

		glBindVertexArray(mesh->gl.vao);

		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);

		glBindVertexArray(0);
	}
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
	env->camera.sprint = glfwGetKey(env->window.ptr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	fps(&env->fps, true);
	mat4_mvp(env);
	draw_mesh(env);
	return (ERR_NONE);
}

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(0.5f, 0.8f, 1.0f, 1);
	while (!glfwWindowShouldClose(env->window.ptr))
	{
		processInput(env->window.ptr);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if ((code = render_scene(env)) != ERR_NONE)
			return (code);
		glfwSwapBuffers(env->window.ptr);
		glfwPollEvents();
	}
	glfwTerminate();
	return (ERR_NONE);
}
