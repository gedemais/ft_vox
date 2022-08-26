#include "../../include/main.h"


static unsigned char	load_shader_source(const char *path, const GLchar **ptr, size_t *file_size)
{
	int		fd;

	// Open file descriptor on shader file
	if ((fd = open(path, O_RDONLY)) == -1)
	{
		// errno message
		ft_putstr_fd(path, 2);
		ft_putstr_fd(" : ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (ERR_OPENING_SHADER_FILE);
	}

	// Read shader source file content
	if (!(*ptr = read_file(fd, file_size)))
		return (ERR_READING_FILE);

	return (ERR_NONE);
}

static unsigned char	compile_shader(t_env *env, GLenum type, const GLchar *source, size_t size)
{
	char			info_log[4096];
	int				success;
	unsigned int	shader_id;

	// Create new shader object
	shader_id = glCreateShader(type);

	// Stores shader identifier
	if (type == GL_VERTEX_SHADER)
		env->gl.shader_vertex = shader_id;
	else if (type == GL_FRAGMENT_SHADER)
		env->gl.shader_fragment = shader_id;

	// Give shader source code to OpenGL
	glShaderSource(shader_id, 1, &source, NULL);
	// Compile the loaded shader source
	glCompileShader(shader_id);

	// Check for more informations about compilation
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) // If shader's compilation failed
	{ // Then display error log message before to exit
		ft_memset(info_log, 0, sizeof(info_log));
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		ft_putstr_fd(info_log, 2);
		munmap((void*)source, size);
		return (ERR_FAILED_TO_COMPILE_SHADER);
	}
	munmap((void*)source, size); // Free memory mapping used for shader source file

	return (ERR_NONE);
}

static unsigned char	build_shader(t_env *env, unsigned int id, const char *path)
{
	const GLchar	*shader_source;
	size_t			shader_size;
	GLenum			shader_type;
	unsigned char	code;

	// Map shader source file content in memory
	if ((code = load_shader_source(path, &shader_source, &shader_size)))
		return (code);

	// Get new shader's type
	if (id == SHADER_VERTEX)
		shader_type = GL_VERTEX_SHADER;
	else if (id == SHADER_FRAGMENT)
		shader_type = GL_FRAGMENT_SHADER;
	else
		shader_type = 0;

	// Compile shader with its source code
	return (compile_shader(env, shader_type, shader_source, shader_size));
}

static unsigned char	link_shader_program(t_env *env)
{
	char	info_log[4096]; // Error log message buffer
	int		success;

	env->gl.shader_program = glCreateProgram(); // Create new program object
	glAttachShader(env->gl.shader_program, env->gl.shader_vertex); // Attach vertex shader to the program
	glAttachShader(env->gl.shader_program, env->gl.shader_fragment); // Attach fragment shader to the program
	glLinkProgram(env->gl.shader_program); // Link the final program

	// Checks for more informations about compilation.
	glGetProgramiv(env->gl.shader_program, GL_LINK_STATUS, &success);

	// If compilation failed
	if (!success)
	{ // Then display the error log message before to exit
		glGetProgramInfoLog(env->gl.shader_program, 4096, NULL, info_log);
		ft_putendl_fd(info_log, 2);
		return (ERR_FAILED_TO_LINK_SHADER_PROGRAM);
	}

	return (ERR_NONE);
}

static void				set_layouts()
{
	// Specifies the disposition of components in vertexs
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(t_stride),
		(void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(t_stride),
		(void *)sizeof(vec3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t_stride),
		(void *)(sizeof(vec3) * 2));
	glEnableVertexAttribArray(2);
}

static unsigned char	init_buffers(t_mesh *mesh)
{
	// VAO -- Create Vertex Array Object
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);
	// VBO -- Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

	GLsizeiptr	size;

	size = (GLsizeiptr)sizeof(t_stride) * mesh->vertices.nb_cells;
	glBufferData(GL_ARRAY_BUFFER, size, mesh->vertices.arr, GL_STATIC_DRAW);

	// EBO -- Create an Elements Buffer Object
	glGenBuffers(1, &mesh->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);

	size = (GLsizeiptr)mesh->faces.nb_cells * (GLsizeiptr)sizeof(uint32_t) * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, mesh->faces.arr, GL_STATIC_DRAW);

	set_layouts();
	glBindVertexArray(0);

	return (ERR_NONE);
}

static void				textures(t_env *env)
{
	(void)env;
	// t_image	image;
	// int		i = -1;

	// glGenTextures(1, &env->gl.texture);
	// while (++i < 1) {
	// 	glActiveTexture(GL_TEXTURE0 + i);
	// 	glBindTexture(GL_TEXTURE_2D, env->gl.texture);

	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// 	image = env->model.texture;
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.w, image.h, 0, GL_BGR, GL_UNSIGNED_BYTE, image.ptr);
	// 	glGenerateMipmap(GL_TEXTURE_2D);
	// }
}

static void				set_uniforms(t_env *env)
{
	// get uniforms
	env->gl.uniform.model = glGetUniformLocation(env->gl.shader_program, "model");
	env->gl.uniform.view = glGetUniformLocation(env->gl.shader_program, "view");
	env->gl.uniform.projection = glGetUniformLocation(env->gl.shader_program, "projection");

	// consume uniforms
	// glUniform1i(env->gl.uniform.texture, 0);
}

unsigned char			init_shaders(t_env *env)
{
	// Paths array to shaders source files
	const char		*shaders_path[SHADER_MAX] = {
		"src/shaders/vertex.glsl",
		"src/shaders/fragment.glsl"
	};
	unsigned char	code;

	// Iterate through shaders ids to build them
	for (unsigned int i = 0; i < SHADER_MAX; i++)
		if ((code = build_shader(env, i, shaders_path[i])))
			return (code);

	// Links shaders into an usable program
	if ((code = link_shader_program(env)) != ERR_NONE)
		return (code);

	set_uniforms(env);

	t_mesh	*mesh;
	int		i = -1;

	// Initializes buffers and data structures for rendering
	while (++i < env->model.meshs.nb_cells) {
		mesh = dyacc(&env->model.meshs, i);

		if ((code = init_buffers(mesh)) != ERR_NONE)
			return (code);
	}

	textures(env);

	//  DEPTH BUFFER
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// CULLING : we only draw front face in clock-wise order
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glFrontFace(GL_CCW);

	return (ERR_NONE);
}
