#include "../../include/main.h"


static void				set_layouts(bool skybox)
{
	// Specifies the disposition of components in vertexs
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

static unsigned char	gl_buffers(t_mesh *mesh, bool skybox)
{
	// VAO -- Create Vertex Array Object
	glGenVertexArrays(1, &mesh->gl.vao);
	glBindVertexArray(mesh->gl.vao);
	// VBO -- Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &mesh->gl.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->gl.vbo);
	if (skybox) {
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(vec3) * mesh->vertices.nb_cells,
			mesh->vertices.arr, GL_STATIC_DRAW);
	} else {
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(t_stride) * mesh->vertices.nb_cells,
			mesh->vertices.arr, GL_STATIC_DRAW);
	}
	set_layouts(skybox);
	glBindVertexArray(0);
	return (ERR_NONE);
}

static unsigned char	set_uniforms(t_mesh *mesh, t_light *light, bool skybox)
{
	glUseProgram(mesh->gl.shader_program);
	mesh->gl.uniform.model = glGetUniformLocation(mesh->gl.shader_program, "model");
	mesh->gl.uniform.view = glGetUniformLocation(mesh->gl.shader_program, "view");
	mesh->gl.uniform.projection = glGetUniformLocation(mesh->gl.shader_program, "projection");

	if (skybox == true) {
		mesh->gl.uniform.skybox = glGetUniformLocation(mesh->gl.shader_program, "vSkybox");
		glUniform1i(mesh->gl.uniform.skybox, 0);
	} else {
		mesh->gl.uniform.time = glGetUniformLocation(mesh->gl.shader_program, "u_time");
		mesh->gl.uniform.campos = glGetUniformLocation(mesh->gl.shader_program, "campos");

		mesh->gl.uniform.texturesHD = glGetUniformLocation(mesh->gl.shader_program, "vTexturesHD");
		mesh->gl.uniform.texturesLD = glGetUniformLocation(mesh->gl.shader_program, "vTexturesLD");
		int	samplersHD[TEXTURE_MAX / 2] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		int	samplersLD[TEXTURE_MAX / 2] = { 8, 9, 10, 11, 12, 13, 14, 15 };
		glUniform1iv(mesh->gl.uniform.texturesHD, TEXTURE_MAX / 2, samplersHD);
		glUniform1iv(mesh->gl.uniform.texturesLD, TEXTURE_MAX / 2, samplersLD);
		light_uniforms(mesh, light);
	}
	return (ERR_NONE);
}

/*
	we set a shader program / mesh
	=> all mesh got his own buffers and shader program
*/
unsigned char			init_meshs(t_env *env)
{
	// Paths array to shaders source files
	const char		*shaders_path[SHADER_MAX] = {
		[SHADER_VERTEX]			= "src/shaders/vertex.glsl",
		[SHADER_FRAGMENT]		= "src/shaders/fragment.glsl",
		[SHADER_SB_VERTEX]		= "src/shaders/skybox_vertex.glsl",
		[SHADER_SB_FRAGMENT]	= "src/shaders/skybox_fragment.glsl"
	};
	unsigned char	code;
	int				i;
	t_mesh			*mesh;

	// Initializes buffers, shaders and data structures for rendering
	// last mesh is all time the skybox
	// MODEL
	i = -1;
	while (++i < env->model.meshs.nb_cells - 1) {
		mesh = dyacc(&env->model.meshs, i);
		if ((code = mount_shaders(mesh, shaders_path[SHADER_VERTEX], shaders_path[SHADER_FRAGMENT])) != ERR_NONE
				|| (code = gl_buffers(mesh, false)) != ERR_NONE
				|| (code = set_uniforms(mesh, &env->light, false)) != ERR_NONE)
			return (code);
	}
	// SKYBOX
	mesh = dyacc(&env->model.meshs, i);
	if ((code = mount_shaders(mesh, shaders_path[SHADER_SB_VERTEX], shaders_path[SHADER_SB_FRAGMENT])) != ERR_NONE
			|| (code = gl_buffers(mesh, true)) != ERR_NONE
			|| (code = set_uniforms(mesh, &env->light, true)) != ERR_NONE)
		return (code);
	return (ERR_NONE);
}
