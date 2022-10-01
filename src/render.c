#include "../include/main.h"


static void				set_gl_options(bool skybox)
{
	// we kept a static to know when to display in count clock wise order
	static bool	display_front = true;

	if (skybox) {
		// hide front faces
		glCullFace(GL_FRONT);
		// Passes if the incoming depth value is less than or equal to the stored depth value.
    	glDepthFunc(GL_LEQUAL);
		display_front = true;
	} else if (display_front == true) {
		// hide back faces
		glCullFace(GL_BACK);
		// Passes if the incoming depth value is less than the stored depth value.
		glDepthFunc(GL_LESS);
		display_front = false;
	}
}

// ====================================================================

static void				render_mesh(t_mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);
	glBindVertexArray(0);
}

static void				render_depth(t_env *env, t_mesh *mesh)
{
	// use program before set uniforms
	glUseProgram(env->model.program_depth);
	// update depth matrices
	glUniformMatrix4fv(glGetUniformLocation(env->model.program_depth, "view"), 1, GL_FALSE, env->model.depthview[0]);
	glUniformMatrix4fv(glGetUniformLocation(env->model.program_depth, "projection"), 1, GL_FALSE, env->model.depthproj[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, mesh->fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	render_mesh(mesh);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static unsigned char	render_scene(t_env *env)
{
	t_mesh		*mesh;
	int			i, e = 1;
	bool		skybox;

#ifdef __APPLE__
	e = 2;
#endif

	// reset viewport
	glViewport(0, 0, env->window.w * e, env->window.h * e);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	i = -1;
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);
		skybox = i == env->model.meshs.nb_cells - 1;
		set_gl_options(skybox);
		// first  - render depth
		if (skybox == false  && env->light.shadow == true)
			render_depth(env, mesh);
		// update all the uniforms
		set_uniforms(env, skybox);
		// second - render mesh
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

	// LIGHT
	// we update the flaslight position
	env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
	env->light.sources[LIGHT_SOURCE_PLAYER].dir = env->camera.zaxis;
	// player's height
	env->light.sources[LIGHT_SOURCE_PLAYER].pos.y -= 2.0f;

	// SHADOWS
	vec3	light_pos, light_dir;
	int		i;

	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		light_pos = env->light.sources[i].pos;
		light_dir = env->light.sources[i].dir;
		mat4_lookat(env->model.depthview[i], light_pos, vec_add(light_pos, light_dir), (vec3){ 0, 1, 0 });
		mat4_inverse(env->model.depthview[i]);
		mat4_projection(env->model.depthproj[i], 90.0f, env->camera.near, env->camera.far, env->camera.ratio);
	}
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

		if ((code = update_world(env)) != ERR_NONE)
			return (code);

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
