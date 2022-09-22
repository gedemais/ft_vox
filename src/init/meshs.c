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
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);
	// VBO -- Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
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

unsigned char	init_mesh(t_env *env, t_mesh *mesh)
{
	unsigned char	code;

	if ((code = gl_buffers(env, mesh, false)) != ERR_NONE
			|| (code = load_uniforms(env, false)) != ERR_NONE
			|| (code = mount_shadows(env, mesh)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}

/*
	we set one shader program for model, skybox and depth
	=> all mesh got their own buffers :: vao, vbo, fbo
*/
unsigned char			init_meshs(t_env *env)
{
	unsigned char	code;
	t_mesh			*mesh;
	int				i;

	// on load model program, skybox program and depth program
	if ((code = mount_shaders(&env->model.program, env->shaders[SHADER_VERTEX], env->shaders[SHADER_FRAGMENT])) != ERR_NONE
			|| (code = mount_shaders(&env->model.program_depth, env->shaders[SHADER_DEPTH_VERTEX], env->shaders[SHADER_DEPTH_FRAGMENT])) != ERR_NONE
			|| (code = mount_shaders(&env->model.program_skybox, env->shaders[SHADER_SB_VERTEX], env->shaders[SHADER_SB_FRAGMENT])) != ERR_NONE)
		return (code);

	// ===================================================================

	// last mesh is all time the skybox
	// setting the uniforms for each shaders' programs
	// MODEL
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		if ((code = init_mesh(env, dyacc(&env->model.meshs, i))) != ERR_NONE)
			return (code);
	}
	// SKYBOX
	mesh = dyacc(&env->model.meshs, env->model.meshs.nb_cells - 1);
	if ((code = gl_buffers(env, mesh, true)) != ERR_NONE
			|| (code = load_uniforms(env, false)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}
