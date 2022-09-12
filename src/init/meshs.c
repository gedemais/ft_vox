#include "../../include/main.h"


static void				set_layouts(bool skybox)
{
	// Specifies the disposition of components in vertexs
	// id ptr, size, GL_type, GL_FALSE, totalsize, start pos

	// vec3 : position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		skybox ? sizeof(vec3) : sizeof(t_stride), (void *)0);
	glEnableVertexAttribArray(0);

	if (skybox == false) {
		// t_vt : texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			sizeof(t_stride), (void *)sizeof(vec3));
		glEnableVertexAttribArray(1);

		// int : normal
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
			sizeof(t_stride), (void *)(sizeof(vec3) + sizeof(t_vt)));
		glEnableVertexAttribArray(2);

		// float : tid
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
			sizeof(t_stride), (void *)(sizeof(vec3) + sizeof(t_vt) + sizeof(float)));
		glEnableVertexAttribArray(3);
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
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(t_stride) * mesh->vertices.nb_cells,
		mesh->vertices.arr, GL_STATIC_DRAW);
	set_layouts(skybox);
	glBindVertexArray(0);
	return (ERR_NONE);
}

static void				load_skybox(t_env *env)
{
	t_texture	*texture;
	int			i;

	glGenTextures(1, &env->model.gl_tskybox);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env->model.gl_tskybox);
	i = -1;
	while (++i < 6) {
		texture = &env->model.textures[TEXTURE_SB_PX + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->ptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

static void				gl_textures(t_env *env)
{
	t_texture	*texture;
	int			i;

	// model's textures
	glGenTextures(TEXTURE_MAX, env->model.gl_textures);
	i = -1;
	while (++i < TEXTURE_MAX) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, env->model.gl_textures[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		texture = &env->model.textures[i];
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->ptr);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	// skybox's texture
	load_skybox(env);
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

	// we mount the textures we will use
	gl_textures(env);
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
