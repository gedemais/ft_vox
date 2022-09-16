#include "../include/main.h"


static void				light_and_shadows_uniforms(t_env *env, t_mesh *mesh, mat4 m)
{
	vec3	tmp;

	// LIGHT PLAYER
	glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].pos);
	// SUNLIGHT
	glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_PLAYER][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].dir);
	// sunlight follow the sun's texture
	tmp = mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].pos);
	glUniform3fv(mesh->gl.uniform.light[LIGHT_SOURCE_SUN][LIGHT_POSITION], 1, (GLfloat *)&tmp);
	// SHADOWS :: update depth matrices
	glUniformMatrix4fv(mesh->gl.uniform.depth_view, 1, GL_FALSE, env->model.depthview);
	glUniformMatrix4fv(mesh->gl.uniform.depth_projection, 1, GL_FALSE, env->model.depthproj);
}

static void				set_uniforms(t_env *env, t_mesh *mesh, bool skybox)
{
	mat4	m;

	mat4_identity(m);
	mat4_yrotation(m, (env->fps.current_seconds * SB_ROT_SPEED) / 100.0f);
	mat4_translate(m, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);

	// use shader program before set the uniforms
	glUseProgram(mesh->gl.program);
	if (skybox == true) {
		// skybox's rotation around the player
		mat4_multiply(env->model.model, m);
	} else {
		// update time
		glUniform1f(mesh->gl.uniform.time, env->fps.current_seconds);
		// update campos
		glUniform3fv(mesh->gl.uniform.campos, 1, (GLfloat *)&env->camera.pos);
		if (env->light.is_active == true)
			light_and_shadows_uniforms(env, mesh, m);
	}
	// update matrices
	glUniformMatrix4fv(mesh->gl.uniform.model, 1, GL_FALSE, env->model.model);
	glUniformMatrix4fv(mesh->gl.uniform.view, 1, GL_FALSE, env->camera.view);
	glUniformMatrix4fv(mesh->gl.uniform.projection, 1, GL_FALSE, env->camera.projection);
}

static void				set_gl_options(bool skybox)
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

// ====================================================================

static void				render_mesh(t_mesh *mesh)
{
	glBindVertexArray(mesh->gl.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);
	glBindVertexArray(0);
}

static void				render_depth(t_env *env, t_mesh *mesh, bool skybox)
{
	if (env->light.is_active == false || skybox == true)
		return ;

	// update depth matrices
	glUseProgram(mesh->gl.depth_program);
	glUniformMatrix4fv(glGetUniformLocation(mesh->gl.depth_program, "view"), 1, GL_FALSE, env->model.depthview);
	glUniformMatrix4fv(glGetUniformLocation(mesh->gl.depth_program, "projection"), 1, GL_FALSE, env->model.depthproj);

	// glBindFramebuffer(GL_FRAMEBUFFER, mesh->gl.fbo);

	// glClear(GL_DEPTH_BUFFER_BIT);

	render_mesh(mesh);

	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static unsigned char	render_scene(t_env *env)
{
	t_mesh		*mesh;
	int			i;
	bool		skybox;

	// reset viewport
	glViewport(0, 0, env->window.w, env->window.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	i = -1;
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);
		skybox = i == env->model.meshs.nb_cells - 1;

		set_gl_options(skybox);

		// first  - render depth
		render_depth(env, mesh, skybox);
		// second - render mesh
		set_uniforms(env, mesh, skybox);
		render_mesh(mesh);
	}
	return (ERR_NONE);
}

// ====================================================================

static inline void		mat4_view(t_camera *camera)
{
	if (camera->ground_fixed == true)
		camera->pos.y = 0;
	mat4_lookat(camera->view, camera->pos, vec_add(camera->pos, camera->zaxis), camera->yaxis);
	mat4_inverse(camera->view);
}

static inline void		mat4_model(t_model *model)
{
	mat4_identity(model->model);
	mat4_scale(model->model, model->scale);
}

static void				update_data(t_env *env)
{
	// MODEL
	mat4_model(&env->model);
	mat4_view(&env->camera);
	mat4_projection(env->camera.projection, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);

	env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
	env->light.sources[LIGHT_SOURCE_PLAYER].dir = env->camera.zaxis;

	// SHADOWS
	vec3		light_pos, light_dir;

	light_pos = env->light.sources[LIGHT_SOURCE_PLAYER].pos;
	light_dir = env->light.sources[LIGHT_SOURCE_PLAYER].dir;

	mat4_lookat(env->model.depthview, light_pos, vec_add(light_pos, light_dir), (vec3){ 0, 1, 0 });
	mat4_inverse(env->model.depthview);

	mat4_projection(env->model.depthproj, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);

	// mat4		bias;

	// mat4_bias(bias, 0.05f);
	// mat4_multiply(env->model.depthproj, bias);
}

// ====================================================================

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(DEFAULT_COLOR.x, DEFAULT_COLOR.y, DEFAULT_COLOR.z, 1);
	while (!glfwWindowShouldClose(env->window.ptr))
	{
		// display fps
		fps(&env->fps, true);
		// handle inputs
		processInput(env->window.ptr);
		// update data
		update_data(env);
		// render scene
		if ((code = render_scene(env)) != ERR_NONE)
			return (code);
		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(env->window.ptr);
		glfwPollEvents();
	}
	glfwTerminate();
	return (ERR_NONE);
}
