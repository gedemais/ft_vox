#include "main.h"

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
		env->vertex_shader_id = shader_id;
	else if (type == GL_FRAGMENT_SHADER)
		env->fragment_shader_id = shader_id;

	// Give shader source code to OpenGL
	glShaderSource(shader_id, 1, &source, NULL);
	// Compile the loaded shader source
	glCompileShader(shader_id);

	// Check for more informations about compilation
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) // If shader's compilation failed
	{ // Then display error log message before to exit
		bzero(info_log, sizeof(info_log));
		glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
		ft_putstr_fd(info_log, 2);
		munmap((const void*)source, size);
		return (ERR_FAILED_TO_COMPILE_SHADER);
	}
	munmap((const void*)source, size); // Free memory mapping used for shader source file

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

	env->shader_program = glCreateProgram(); // Create new program object
	glAttachShader(env->shader_program, env->vertex_shader_id); // Attach vertex shader to the program
	glAttachShader(env->shader_program, env->fragment_shader_id); // Attach fragment shader to the program
	glLinkProgram(env->shader_program); // Link the final program

	// Checks for more informations about compilation.
	glGetProgramiv(env->shader_program, GL_LINK_STATUS, &success);

	// If compilation failed
	if (!success)
	{ // Then display the error log message before to exit
		glGetProgramInfoLog(env->shader_program, 4096, NULL, info_log);
		ft_putendl_fd(info_log, 2);
		return (ERR_FAILED_TO_LINK_SHADER_PROGRAM);
	}

	return (ERR_NONE);
}

static unsigned char	init_buffers(t_env *env)
{
	GLsizeiptr		size;

	// Generate OpenGL buffers
	glGenBuffers(1, &env->vbo); // Vertex Buffer Object
	glGenVertexArrays(1, &env->vao); // Vertex Attribute Object
	glGenBuffers(1, &env->ebo); // Element Buffer Object

	glBindBuffer(GL_ARRAY_BUFFER, env->vbo); // Bind vbo buffer
	glBindVertexArray(env->vao); // Bind vao array

	// Configurate vertexs buffer
	size = (GLsizeiptr)sizeof(t_stride) * env->scene.vertexs.nb_cells;
	// Copies vertexs data into buffer
	glBufferData(GL_ARRAY_BUFFER, size, env->scene.vertexs.c, GL_STATIC_DRAW);

	// Specifies the disposition of components in vertexs
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(t_stride), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(t_stride), (void*)sizeof(t_vec3d));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(t_stride), (void*)(sizeof(t_vec3d) + sizeof(t_color)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, env->ebo); // Bind ebo buffer

	// Copies faces indices data in ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)env->scene.faces.nb_cells * (GLsizeiptr)sizeof(uint32_t) * 3, env->scene.faces.c, GL_STATIC_DRAW);

//	glGenTextures(1, &env->txt);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	t_texture *txt = &((t_mtl*)(dyacc(&env->scene.mtls, 0)))->texture;
	if (txt)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txt->w, txt->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, txt->img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return (ERR_NONE);
}

unsigned char			init_shaders(t_env *env)
{
	// Paths array to shaders source files
	const char		*shaders_path[SHADER_MAX] = {"src/shaders/vertex.glsl",
												 "src/shaders/fragment.glsl"};
	unsigned char	code;

	// Iterate through shaders ids to build them
	for (unsigned int i = 0; i < SHADER_MAX; i++)
		if ((code = build_shader(env, i, shaders_path[i])))
			return (code);

	// Links shaders into an usable program
	if ((code = link_shader_program(env)) != ERR_NONE
	// Initializes buffers and data structures for rendering
		|| (code = init_buffers(env)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}
