#include "../../include/main.h"


static void				set_layouts(bool skybox)
{
	// id ptr, size, GL_type, GL_FALSE, totalsize, start pos
	if (skybox) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
		glEnableVertexAttribArray(0);
	}
	else {
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(t_stride), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(t_stride), (void *)sizeof(float));
		glEnableVertexAttribArray(1);
	}

}

static unsigned char	gl_buffers(t_env *env, t_mesh *mesh, bool skybox)
{
	unsigned char	code;
	GLsizeiptr		size;

	// VAO -- Create Vertex Array Object
	glGenVertexArrays(1, &mesh->gl.vao);
	glBindVertexArray(mesh->gl.vao);
	// VBO -- Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &mesh->gl.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->gl.vbo);

	size = (GLsizeiptr)(skybox ? sizeof(vec3) : sizeof(t_stride));
	glBufferData(GL_ARRAY_BUFFER, size * mesh->vertices.nb_cells, mesh->vertices.arr, GL_STATIC_DRAW);
	// Specifies the disposition of components in vertexs
	set_layouts(skybox);
	// load the textures this buffer will use
	if ((code = mount_textures(env, skybox ? 1 : 0)) != ERR_NONE)
		return (code);
	glBindVertexArray(0);
	return (ERR_NONE);
}

/*
	we stock uniform's id for later use (matrices, ...)
	and we consume what we need (textures, light, ...)
*/
static unsigned char	set_uniforms(t_mesh *mesh, t_light *light, bool skybox)
{
	unsigned char	code;

	// use program before set uniforms
	glUseProgram(mesh->gl.program);
	// matrices
	mesh->gl.uniform.model = glGetUniformLocation(mesh->gl.program, "model");
	mesh->gl.uniform.view = glGetUniformLocation(mesh->gl.program, "view");
	mesh->gl.uniform.projection = glGetUniformLocation(mesh->gl.program, "projection");
	if (skybox == true) {
		// skybox's texture
		mesh->gl.uniform.skybox = glGetUniformLocation(mesh->gl.program, "vSkybox");
		glUniform1i(mesh->gl.uniform.skybox, 0);
	} else {
		// time
		mesh->gl.uniform.time = glGetUniformLocation(mesh->gl.program, "u_time");
		// campos
		mesh->gl.uniform.campos = glGetUniformLocation(mesh->gl.program, "campos");
		// textures and light
		if ((code = textures_uniforms(mesh)) != ERR_NONE
				|| (code = light_uniforms(mesh, light)) != ERR_NONE)
			return (code);
		// depth matrix
		mesh->gl.uniform.depth_view = glGetUniformLocation(mesh->gl.program, "depth_view");
		mesh->gl.uniform.depth_projection = glGetUniformLocation(mesh->gl.program, "depth_projection");
	}
	return (ERR_NONE);
}

/*
	we set a shader program / mesh
	=> all mesh got his own buffers and shader program
*/
unsigned char			init_meshs(t_env *env)
{
	unsigned char	code;
	int				i;
	t_mesh			*mesh;

	// Initializes buffers, shaders and data structures for rendering
	// last mesh is all time the skybox
	// MODEL
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		mesh = dyacc(&env->model.meshs, i);
		if ((code = mount_shaders(&mesh->gl.program, env->shaders[SHADER_VERTEX], env->shaders[SHADER_FRAGMENT])) != ERR_NONE
				|| (code = mount_shaders(&mesh->gl.depth_program, env->shaders[SHADER_DEPTH_VERTEX], env->shaders[SHADER_DEPTH_FRAGMENT])) != ERR_NONE
				|| (code = gl_buffers(env, mesh, false)) != ERR_NONE
				|| (code = set_uniforms(mesh, &env->light, false)) != ERR_NONE
				|| (code = mount_shadows(env, mesh)) != ERR_NONE)
			return (code);
	}
	// SKYBOX
	mesh = dyacc(&env->model.meshs, i);
	if ((code = mount_shaders(&mesh->gl.program, env->shaders[SHADER_SB_VERTEX], env->shaders[SHADER_SB_FRAGMENT])) != ERR_NONE
			|| (code = gl_buffers(env, mesh, true)) != ERR_NONE
			|| (code = set_uniforms(mesh, &env->light, true)) != ERR_NONE)
		return (code);
	return (ERR_NONE);
}
