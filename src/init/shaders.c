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
	{
		close(fd);
		return (ERR_READING_FILE);
	}
	close(fd);
	return (ERR_NONE);
}

static unsigned char	compile_shader(unsigned int shader_id, const GLchar *source, size_t size)
{
	(void)size;

	char			info_log[4096];
	int				success;

	// Give shader source code to OpenGL
	glShaderSource(shader_id, 1, &source, NULL);
	// Compile the loaded shader source
	glCompileShader(shader_id);

	// Check for more informations about compilation
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	// If shader's compilation failed
	if (success == GL_FALSE)
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

static unsigned char	build_shader(t_mesh *mesh, GLenum shader_type, const char *path)
{
	const GLchar	*shader_source;
	size_t			shader_size;
	unsigned char	code;
	unsigned int	shader_id;

	// Map shader source file content in memory
	if ((code = load_shader_source(path, &shader_source, &shader_size)))
		return (code);
	// Create new shader object
	shader_id = glCreateShader(shader_type);
	// Compile shader with its source code
	if ((code = compile_shader(shader_id, shader_source, shader_size)) != ERR_NONE)
		return (code); 
	// Assign shaders in mesh
	if (shader_type == GL_VERTEX_SHADER)
		mesh->gl.shader_vertex = shader_id;
	else if (shader_type == GL_FRAGMENT_SHADER)
		mesh->gl.shader_fragment = shader_id;
	return (ERR_NONE);
}

static unsigned char	link_shader_program(t_mesh *mesh)
{
	char	info_log[4096];
	int		success;

	mesh->gl.shader_program = glCreateProgram(); // Create new program object
	glAttachShader(mesh->gl.shader_program, mesh->gl.shader_vertex); // Attach vertex shader to the program
	glAttachShader(mesh->gl.shader_program, mesh->gl.shader_fragment); // Attach fragment shader to the program
	glLinkProgram(mesh->gl.shader_program); // Link the final program

	// Checks for more informations about compilation.
	glGetProgramiv(mesh->gl.shader_program, GL_LINK_STATUS, &success);
	// If compilation failed
	if (!success)
	{ // Then display the error log message before to exit
		glGetProgramInfoLog(mesh->gl.shader_program, 4096, NULL, info_log);
		ft_putendl_fd(info_log, 2);
		return (ERR_FAILED_TO_LINK_SHADER_PROGRAM);
	}
	return (ERR_NONE);
}

unsigned char		mount_shaders(t_mesh *mesh, const char *svertex_path, const char *sfragment_path)
{
	unsigned char	code;

	if ((code = build_shader(mesh, GL_VERTEX_SHADER, svertex_path)) != ERR_NONE
			|| (code = build_shader(mesh, GL_FRAGMENT_SHADER, sfragment_path)) != ERR_NONE
			|| (code = link_shader_program(mesh)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}
