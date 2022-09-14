#include "../include/main.h"

static void				set_uniforms(t_env *env, t_mesh *mesh, bool skybox)
{
	mat4	m;
	vec3	tmp;

	mat4_identity(m);
	mat4_yrotation(m, (env->fps.current_seconds * SB_ROT_SPEED) / 100);
	mat4_translate(m, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);

	glUseProgram(mesh->gl.shader_program);
	if (skybox == true) {
		// skybox's rotation around the player
		mat4_multiply(env->model.model, m);
	} else {
		// update time
		glUniform1f(mesh->gl.uniform.time, env->fps.current_seconds);

		// update campos
		glUniform3fv(mesh->gl.uniform.campos, 1, (GLfloat *)&env->camera.pos);

		// update lightpos
		// LIGHT PLAYER
		env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].pos);
		// SUNLIGHT
		env->light.sources[LIGHT_SOURCE_PLAYER].dir = env->camera.zaxis;
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].dir);
		// sunlight follow the sun's texture
		tmp = mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].pos);
		glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_SUN][LIGHT_POSITION], 1, (GLfloat *)&tmp);

		// update depth matrices
		glUniformMatrix4fv(mesh->gl.uniform.depth_mvp, 1, GL_FALSE, env->model.shadows.projection);
	}
	// update matrices in shaders
	glUniformMatrix4fv(mesh->gl.uniform.model, 1, GL_FALSE, env->model.model);
	glUniformMatrix4fv(mesh->gl.uniform.view, 1, GL_FALSE, env->camera.view);
	glUniformMatrix4fv(mesh->gl.uniform.projection, 1, GL_FALSE, env->camera.projection);
}

static void				gl_options(bool skybox)
{
	if (skybox) {
		// glCullFace(GL_FRONT);
    	glDepthFunc(GL_LEQUAL);
	} else {
		// glCullFace(GL_BACK);
		// glFrontFace(GL_CW);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
	}
}

static void				draw_mesh(t_env *env)
{
	t_mesh		*mesh;
	GLsizeiptr	size;
	int			i, j;
	bool		skybox;

	i = -1;
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);
		if (mesh == NULL)
			continue ;
		skybox = i == env->model.meshs.nb_cells - 1;

		gl_options(skybox);

		set_uniforms(env, mesh, skybox);

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

static void				update_matrices(t_env *env)
{
	// MODEL
	mat4_model(&env->model);
	mat4_view(&env->camera);
	mat4_projection(env->camera.projection, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);
	// SHADOWS
	t_shadows	*shadows	= &env->model.shadows;
	vec3		light_pos, light_dir, tmp;

	// mat4	bias;

	// bias[0] = 0.5f;
	// bias[1] = 0;
	// bias[2] = 0;
	// bias[3] = 0;
	// bias[4] = 0;
	// bias[5] = 0.5f;
	// bias[6] = 0;
	// bias[7] = 0;
	// bias[8] = 0;
	// bias[9] = 0;
	// bias[10] = 0.5f;
	// bias[11] = 0;
	// bias[12] = 0.5f;
	// bias[13] = 0.5f;
	// bias[14] = 0.5f;
	// bias[15] = 0.5f;

	light_pos = env->light.sources[LIGHT_SOURCE_PLAYER].pos;
	light_dir = env->light.sources[LIGHT_SOURCE_PLAYER].dir;
	// mat4_lookat(shadows->view, light_pos, vec_add(light_pos, light_dir), (vec3){ 0, 1, 0 });
	(void)light_dir;
	mat4_lookat(shadows->view, light_pos, (vec3){ 0, 0, 0 }, (vec3){ 0, 1, 0 });
	mat4_inverse(shadows->view);

	mat4_orthogonal(shadows->projection, 10, 10, -10, -10, env->camera.near, env->camera.far);
	// mat4_identity(shadows->mvp);
	// mat4_multiply(shadows->mvp, env->camera.projection);

	mat4_multiply(shadows->projection, shadows->view);

	// mat4_multiply(shadows->mvp, bias);
}

static unsigned char	render_scene(t_env *env)
{
	// reset viewport
	// glViewport(0, 0, env->window.w, env->window.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fps(&env->fps, true);
	draw_mesh(env);
	return (ERR_NONE);
}

static unsigned char	render_depth(t_env *env)
{
	return (ERR_NONE);
	t_shadows	*shadows = &env->model.shadows;

	glBindFramebuffer(GL_FRAMEBUFFER, shadows->fbo);
	// glViewport(0, 0, env->window.w, env->window.h);
	glClear(GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_MAX);
	glBindTexture(GL_TEXTURE_2D, env->model.shadows.depthmap);
	// renderScene(simpleDepthShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return (ERR_NONE);
}

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(DEFAULT_COLOR.x, DEFAULT_COLOR.y, DEFAULT_COLOR.z, 1);
	while (!glfwWindowShouldClose(env->window.ptr))
	{
		// input
		// -----
		processInput(env->window.ptr);
		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// update matrices
		// ---------------
		update_matrices(env);
		// 1. render depth of scene to texture
		// -----------------------------------
		if ((code = render_depth(env)) != ERR_NONE)
			return (code);
		// 2. render scene using the generated depth/shadow map  
		// ----------------------------------------------------
		if ((code = render_scene(env)) != ERR_NONE)
			return (code);
		// 3. glfw: swap buffers and poll IO events
		// ----------------------------------------
		glfwSwapBuffers(env->window.ptr);
		glfwPollEvents();
	}
	glfwTerminate();
	return (ERR_NONE);
}
