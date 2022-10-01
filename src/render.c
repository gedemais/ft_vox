#include "../include/main.h"


static void				render_mesh(t_mesh *mesh);

// ====================================================================

static void				update_options(t_env *env, char type)
{
	switch (type) {
		case (0): // depth
			reset_viewport(DEPTHMAP_W, DEPTHMAP_H);
			glClear(GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT);
			// Passes if the incoming depth value is less than the stored depth value.
			glDepthFunc(GL_LESS);
			// update uniforms
			set_uniforms(env, 0);
			break;
		case (1): // model
			reset_viewport(env->window.w, env->window.h);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK);
			// Passes if the incoming depth value is less than the stored depth value.
			glDepthFunc(GL_LESS);
			// update uniforms
			set_uniforms(env, 1);
			break;
		case (2): // skybox
			glCullFace(GL_FRONT);
			// Passes if the incoming depth value is less than or equal to the stored depth value.
			glDepthFunc(GL_LEQUAL);
			// update uniforms
			set_uniforms(env, 2);
			break;
	}
}

// ====================================================================

static void				pass_depth(t_env *env)
{
	int		i;
	t_mesh	*mesh;

	glBindFramebuffer(GL_FRAMEBUFFER, env->model.fbo);

	update_options(env, 0);
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		mesh = dyacc(&env->model.meshs, i);
		render_mesh(mesh);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void				pass_scene(t_env *env)
{
	int		i;
	bool	skybox;
	t_mesh	*mesh;

	update_options(env, 1);
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		mesh = dyacc(&env->model.meshs, i);
		render_mesh(mesh);
	}
}

static inline void		pass_skybox(t_env *env)
{
	update_options(env, 2);
	render_mesh(dyacc(&env->model.meshs, env->model.meshs.nb_cells - 1));
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
	// FPS
	// display :: true / false
	fps(&env->fps, true);

	// MODEL
	mat4_model(&env->model);
	mat4_view(&env->camera);
	mat4_projection(env->camera.projection, env->camera.fov, env->camera.near, env->camera.far, env->camera.ratio);

	// LIGHT
	// we update the flaslight position
	env->light.sources[LIGHT_SOURCE_PLAYER].pos = env->camera.pos;
	env->light.sources[LIGHT_SOURCE_PLAYER].dir = env->camera.zaxis;
	// player's height
	env->light.sources[LIGHT_SOURCE_PLAYER].pos.y -= MODEL_SCALE / 2.0f;

	// SHADOWS
	vec3	light_pos, light_dir;
	int		i;

	i = -1;
	while (++i < LIGHT_SOURCE_MAX) {
		light_pos = env->light.sources[i].pos;
		light_dir = env->light.sources[i].dir;
		light_dir = (vec3){ -light_dir.x, -light_dir.y, -light_dir.z };
		mat4_lookat(env->model.depthview[i], light_pos, vec_sub(light_pos, light_dir), (vec3){ 0, 1, 0 });
		mat4_inverse(env->model.depthview[i]);
		mat4_projection(env->model.depthproj[i],
			SHADOW_TARGET == 0 ? 90.0f : 180.0f,
			env->camera.near,
			SHADOW_TARGET == 0 ? CHUNK_SIZE / 2.0f : env->camera.far * env->camera.far,
			env->camera.ratio);
	}
}

// ====================================================================

static void				render_mesh(t_mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.nb_cells);
	glBindVertexArray(0);
}

static unsigned char	render_scene(t_env *env)
{
	unsigned char	code;

	if ((code = update_world(env)) != ERR_NONE)
		return (code);
	// update data
	update_data(env);
	// first  - render depth
	pass_depth(env);
	// second - render scene
	pass_scene(env);
	// third  - render skybox
	pass_skybox(env);
	return (ERR_NONE);
}

// ====================================================================

unsigned char			display_loop(t_env *env)
{
	unsigned char	code;

	glClearColor(DEFAULT_COLOR.x, DEFAULT_COLOR.y, DEFAULT_COLOR.z, 1);
	while (!glfwWindowShouldClose(env->window.ptr))
	{
		// handle inputs
		processInput(env->window.ptr);
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
