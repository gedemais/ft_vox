#include "../../include/main.h"


static void				light_and_shadows_uniforms(t_env *env, mat4 m)
{
	vec3	tmp;

	// LIGHT PLAYER
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_PLAYER][LIGHT_POSITION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].pos);
	// SUNLIGHT
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_PLAYER][LIGHT_DIRECTION], 1, (GLfloat *)&env->light.sources[LIGHT_SOURCE_PLAYER].dir);
	// sunlight follow the sun's texture
	tmp = mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].pos);
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_SUN][LIGHT_POSITION], 1, (GLfloat *)&tmp);
	tmp = vec_normalize(mat4_x_vec3(m, env->light.sources[LIGHT_SOURCE_SUN].dir));
	glUniform3fv(env->model.uniforms.light[LIGHT_SOURCE_SUN][LIGHT_DIRECTION], 1, (GLfloat *)&tmp);
	// SHADOWS :: update depth matrices
	glUniformMatrix4fv(env->model.uniforms.depth_view, 1, GL_FALSE, env->model.depthview[0]);
	glUniformMatrix4fv(env->model.uniforms.depth_projection, 1, GL_FALSE, env->model.depthproj[0]);
}

static void				update_uniforms(t_env *env, bool skybox, mat4 m)
{
	GLuint	program = skybox ? env->model.program_skybox : env->model.program;

	// use shader program before set the uniforms
	glUseProgram(program);
	if (skybox == true) {
		// skybox's rotation around the player
		mat4_multiply(env->model.model, m);
	} else {
		// update time
		glUniform1f(env->model.uniforms.time, env->fps.current_seconds);
		// update campos
		glUniform3fv(env->model.uniforms.campos, 1, (GLfloat *)&env->camera.pos);
		if (env->light.is_active == true)
			light_and_shadows_uniforms(env, m);
	}
	// update matrices
	glUniformMatrix4fv(env->model.uniforms.model, 1, GL_FALSE, env->model.model);
	glUniformMatrix4fv(env->model.uniforms.view, 1, GL_FALSE, env->camera.view);
	glUniformMatrix4fv(env->model.uniforms.projection, 1, GL_FALSE, env->camera.projection);
}

void					set_uniforms(t_env *env, bool skybox)
{
	mat4	m;

	// rotation and translation around player (for sunlight and skybox for exemple)
	mat4_identity(m);
	mat4_yrotation(m, (env->fps.current_seconds * SB_ROT_SPEED) / 100.0f);
	mat4_translate(m, env->camera.pos.x, env->camera.pos.y, env->camera.pos.z);

	update_uniforms(env, skybox, m);
}

// =======================================================================

/*
	we stock uniform's id for later use (matrices, ...)
	and we consume what we need (textures, light, ...)
*/
unsigned char			load_uniforms(t_env *env, bool skybox)
{
	unsigned char	code;
	GLuint			program = skybox ? env->model.program_skybox : env->model.program;

	// use program before set uniforms
	glUseProgram(program);
	// matrices
	env->model.uniforms.model = glGetUniformLocation(program, "model");
	env->model.uniforms.view = glGetUniformLocation(program, "view");
	env->model.uniforms.projection = glGetUniformLocation(program, "projection");
	if (skybox == true) {
		// skybox's texture
		env->model.uniforms.skybox = glGetUniformLocation(program, "vSkybox");
		glUniform1i(env->model.uniforms.skybox, 0);
	} else {
		// time
		env->model.uniforms.time = glGetUniformLocation(program, "u_time");
		// campos
		env->model.uniforms.campos = glGetUniformLocation(program, "campos");
		// textures and light
		if ((code = textures_uniforms(env)) != ERR_NONE
				|| (code = light_uniforms(env)) != ERR_NONE)
			return (code);
		// depth matrix
		env->model.uniforms.depth_view = glGetUniformLocation(program, "depth_view");
		env->model.uniforms.depth_projection = glGetUniformLocation(program, "depth_projection");
	}
	return (ERR_NONE);
}
